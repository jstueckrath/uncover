/***************************************************************************
 *   Copyright (C) 2009 by Marvin Heumüller                                *
 *   Copyright (C) 2014 by Jan Stückrath <jan.stueckrath@uni-due.de>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110, USA                    *
 ***************************************************************************/

#include <iostream>
#include "IsomorphismChecker.h"
#include "SignatureCreater.h"
#include "../logging/message_logging.h"
#include <set>

using namespace std;
using namespace uncover::graphs;
using namespace uncover::logging;

namespace uncover {
	namespace graphs {

		IsomorphismChecker::IsomorphismChecker() {}

		IsomorphismChecker::~IsomorphismChecker() {}

		bool IsomorphismChecker::areIsomorphic(Hypergraph const& hg1, Hypergraph const& hg2)
		{

			// graphs must have the same number of vertices
			if (hg1.getVertexCount() != hg2.getVertexCount()) return false;

			// graphs must have the same number of edges
			if (hg1.getEdgeCount() != hg2.getEdgeCount()) return false;

			HGSignature sigs1, sigs2;
			PartSig sig1, sig2;
			sig1 = SignatureCreater::createHGSignature(hg1, sigs1);
			sig2 = SignatureCreater::createHGSignature(hg2, sigs2);

			// graphs must have the same signature
			if (sig1 != sig2) return false;
			
			// if nothing helps, start recursive check
			map<IDType, IDType> mapF, mapB;
			map<PartSig, list<IDType> > sigs2rev;
			for (auto it = sigs2.cbegin(); it != sigs2.cend(); ++it)
			{
				sigs2rev[it->second].push_back(it->first);
			}
			
			// try all different mappings
			return mapping(hg1, sigs1, hg2, sigs2, sigs2rev, mapF, mapB, hg1.cbeginEdges(), hg1.cendEdges());
		}

		bool IsomorphismChecker::mapping (
				Hypergraph const& h0,
				HGSignature& sigs0,
				Hypergraph const& h1,
				HGSignature& sigs1,
				map<PartSig, list<IDType> >& sigs1rev,
				map<IDType, IDType>& mappingsFor,
				map<IDType, IDType>& mappingsBack,
				Hypergraph::EMap::const_iterator it,
				Hypergraph::EMap::const_iterator end )
		{
			if ( it == end )
			{
				// end of recursion
				//assert(mappingsFor.size() == mappingsBack.size());
				return true;
			}

			IDType currentID = it->first;
			PartSig currentSig = sigs0[currentID];
			
			// in each function call we iterate over one edge e0 in h0.
			// we try to map one edge e1 of h1 to h0 that has the same signaure like e0
			for ( list<IDType>::iterator
			        fitit = sigs1rev[currentSig].begin(); fitit != sigs1rev[currentSig].end(); ++fitit )
			{

				if (!h1.hasEdge(*fitit)){
					logger.newWarning() << "Found a edges and a vertex with identical signatures." << endLogMessage;
					continue;
				}

				if (it->second.getLabel() != h1.getEdge(*fitit)->getLabel())
				{
					logger.newWarning() << "Found edges with identical signatures and different labels." << endLogMessage;
					continue;
				}

				// try mapping
				// if the potential edge already has been mapped, do not map it again
				if ( mappingsBack.find ( *fitit ) == mappingsBack.end() )
				{
					bool success = true;
					
					// save all temporal mappings so we can delete them in case of no success
					set<IDType> newMappings;
					set<IDType> newMappingsRev;
					auto verts0 = h0.getVerticesOfEdge(currentID);
					auto verts1 = h1.getVerticesOfEdge(*fitit);
					auto i0 = verts0->cbegin();
					auto i1 = verts1->cbegin();

					//Iterate over all incident vertices
					if (verts0->size() != verts1->size())
					{
						logger.newWarning() << "Error in edge detected while checking for isomorphisms. ";
						logger.contMsg() << "Graph (id=" << h0.getID() << ") has edge with label '" << it->second.getLabel();
						logger.contMsg() << "', arity " << verts0->size() << " and signature '" << sigs0[currentID] << "'. ";
						logger.contMsg() << "Graph (id=" << h1.getID() << ") has edge with label '" << h1.getEdge(*fitit)->getLabel();
						logger.contMsg() << "', arity " << verts1->size() << " and signature '" << sigs1[*fitit] << "'." << endLogMessage;
					}

					while (i0 != verts0->cend())
					{
						// vertices to fit must have the same signature
						if ( sigs0[*i0] != sigs1[*i1] )
						{
							success = false;
							break;
						}

						// map the vertex if it has not been mapped in a different way earlier
						success = insertIfValid ( mappingsFor, mappingsBack, *i0, *i1 );
						if ( success )
						{
							newMappings.insert ( *i0 );
							newMappingsRev.insert ( *i1 );
						}
						else
						{
							break;
						}
						i0++;
						i1++;
					}
					
					// success = we have mapped all adjacent edges successfully
					if ( success )
					{
						// success &= we have also mapped our edge successfully
						success = insertIfValid ( mappingsFor, mappingsBack, currentID, *fitit );
						if ( success )
						{
							newMappings.insert ( currentID );
							newMappingsRev.insert ( *fitit );
							Hypergraph::EMap::const_iterator newIt = it;
							newIt++;
							
							// everything of the current edge has been mapped, so let's continue with the next edge.
							success = mapping ( h0, sigs0, h1, sigs1, sigs1rev, mappingsFor, mappingsBack, newIt, end );

							if ( success )
							{
								// recursion came to a happy end
								return true;
							}
						}
					}
					if (!success)
					{
						//there was a problem with one of the steps above, so we have to undo all changed for this step
						for ( set<IDType>::iterator
								del = newMappings.begin(); del != newMappings.end(); ++del )
						{
							mappingsFor.erase ( *del );
						}
						for ( set<IDType>::iterator
								del = newMappingsRev.begin(); del != newMappingsRev.end(); ++del )
						{
							mappingsBack.erase ( *del );
						}
					}
				}
			}
			return false;
		}
		
		
		bool IsomorphismChecker::insertIfValid ( map<IDType, IDType>& forw, map<IDType, IDType>& backw, IDType id0, IDType id1 )
		{
			if ( forw.find ( id0 ) != forw.end() && forw[id0] != id1 ) return false;
			if ( backw.find ( id1 ) != backw.end() && backw[id1] != id0 ) return false;
			forw.insert ( make_pair ( id0, id1 ) );
			backw.insert ( make_pair ( id1, id0 ) );
			return true;
		}

	}
}
