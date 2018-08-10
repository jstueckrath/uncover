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

#include "OldMinorOrder.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace uncover::graphs;

namespace uncover {
	namespace minors {

		OldMinorOrder::OldMinorOrder(){}

		OldMinorOrder::~OldMinorOrder() {}

		bool OldMinorOrder::isLessOrEq(Hypergraph const& smaller, Hypergraph const& bigger) const {

			// abort if 'smaller' can obviously not be smaller
			if(smaller.getEdgeCount() > bigger.getEdgeCount() || smaller.getVertexCount() > bigger.getVertexCount()) {
				return false;
			}

			vector<pair<IDType, string> > occurrencesM;
			map<string, size_t> countOccurrencesM;
			
			// Find occurrences of all edge labels in M
			for (auto it = smaller.cbeginEdges(); it != smaller.cendEdges(); ++it )
			{
				string labelM = it->second.getLabel();
				IDType id = it->first;
				
				occurrencesM.push_back(make_pair(id, labelM));
				
				if (countOccurrencesM.find(labelM) == countOccurrencesM.end())
				{
					countOccurrencesM.insert(make_pair(labelM, 1));
				}
				else
				{
					countOccurrencesM[labelM]++;
				}
			}

			map<string, vector<IDType> > occurrencesG;

			// Find occurrences of all edge labels in G
			for(auto it = bigger.cbeginEdges(); it != bigger.cendEdges(); it++ )
			{
				string labelG = it->second.getLabel();
				IDType idG = it->first;
				
				if (occurrencesG.find(labelG) == occurrencesG.end())
				{
					occurrencesG.insert(make_pair(labelG, vector<IDType>()));
				}
				occurrencesG[labelG].push_back(idG);
			}
			
			// Now we found all occurrences of any label of edges in M in G.
			// In G, there must not be less edges with the same label than in M.
			for (auto it = countOccurrencesM.begin(); it != countOccurrencesM.end(); it++ )
			{
				//More edges with one label in M than in G
				if ( it->second > occurrencesG[it->first].size())
					return false;
			}
			
			map<IDType, IDType> m;
			return mapping(smaller, bigger, occurrencesG, occurrencesM, 0, m);
		}
		
		bool OldMinorOrder::mapping(Hypergraph const& minor,
									Hypergraph const& graph,
									map<string, vector<IDType> >& occurrencesG,
									vector<pair<IDType, string> >& occurrencesM,
									size_t currentI,
									map<IDType, IDType>& mappings) const
		{
			if (currentI == occurrencesM.size())
			{
				return createEdgeTupels(minor,
										graph,
										mappings);
			}
			
			string& label = occurrencesM.at(currentI).second;
			IDType& idM = occurrencesM.at(currentI).first;
			
			for (size_t i = 0; i < occurrencesG[label].size(); i++)
			{
				IDType idG = occurrencesG[label][i];
				mappings.insert(make_pair(idM, idG));
				occurrencesG[label].erase(occurrencesG[label].begin() + i);
				//cout << "try mapping " << idM << " -> " << idG << endl;
				if (mapping(minor, graph, occurrencesG, occurrencesM,
					currentI+1, mappings))
				{
					return true;
				}
				occurrencesG[label].insert(occurrencesG[label].begin()+i, idG);
				mappings.erase(idM);
			}
			return false;
		}
		
		bool OldMinorOrder::createEdgeTupels(
				Hypergraph const& minor,
				Hypergraph const& graph,
				map<IDType, IDType>& mappings) const
		{
			// Goal is, to create a list of edge tupels which are adjecent to
			// the same vertex in M. Then we can easily find mapped vertices in G.
			
			list<set<IDType> > allConnectedVertices;
			
			set<IDType> nodeImage;
			
			// Iterate over all vertices and create edgeTupels.
			for(Hypergraph::VMap::const_iterator it = minor.beginVertices(); it != minor.endVertices(); it++)
			{
				set<IDType> connectedVertices;
				
				auto edges = *(minor.getConnectedEdges(it->first));
				
				// Find all adjecent edges
				for (auto	it2 = edges.begin(); it2 != edges.end(); it2++)
				{
					// Now, again, find our first vertex and save its index
					vector<IDType> const* vertices = minor.getVerticesOfEdge(*it2);
					int i = 0;
					for (vector<IDType>::const_iterator
							it3 = vertices->begin(); it3 != vertices->end(); it3++)
					{
						if (*it3 == it->first) //only one time
						{	
							//assert(mappings.find(*it2) != mappings.end());
							IDType edgeInG = mappings[*it2];
							vector<IDType> const* v = graph.getVerticesOfEdge(edgeInG);
							vector<IDType>::const_iterator it4 = v->begin();
							for (int j = 0; j < i; j++) it4++;
							IDType vertexInG = *it4;
							connectedVertices.insert(vertexInG);
							
							nodeImage.insert(*it4);
						}
						i++;
					}
				}
				// if more than 1 vertices have to be connected, save them
				// to check, if there is a path between them.
				
				
				if (connectedVertices.size() > 0)
					allConnectedVertices.push_back(connectedVertices);
			}
			//Now check, whether there exists an intersection of vertices of
			//different edgetupels. If yes, a minor does not exist. (in the
			//'minor', these edges are not the same but in the graph they are)
			set<IDType> currentSet;
			for (list<set<IDType> >::iterator
				it = allConnectedVertices.begin(); it != allConnectedVertices.end(); it++)
			{
				set<IDType> intersection;
				set_intersection(	currentSet.begin(), currentSet.end(),
									it->begin(), it->end(), 
									inserter(intersection, intersection.begin()));
				if (intersection.size() > 0) return false;
				currentSet.insert(it->begin(), it->end()); //union of all edgetupels which are disjoint
				
			}
			
			int maxIsolatedNodesInG = 0;
			for (Hypergraph::VMap::const_iterator it = graph.beginVertices(); it != graph.endVertices(); ++it)
			{
				if (nodeImage.count(it->first) ==0)
				{
					maxIsolatedNodesInG++;
				}
			}
			
			int isolatedNodesInM = 0;
			for (Hypergraph::VMap::const_iterator it = minor.beginVertices(); it != minor.endVertices(); ++it)
			{
				if (minor.getConnectedEdges(it->first)->size() == 0)
				{
					isolatedNodesInM++;
				}
			}
			
			if (isolatedNodesInM > maxIsolatedNodesInG) return false;
			
			//now find all mapped edges in G
			set<IDType> mappedEdges;
			for(Hypergraph::EMap::const_iterator it = graph.beginEdges(); it != graph.endEdges(); ++it)
			{
				bool found = false;
				for (map<IDType, IDType>::iterator
					itMapping = mappings.begin(); itMapping != mappings.end(); ++itMapping)
				{
					if (it->first == itMapping->second)
					{
						found = true;
						break;
					}
				}
				if (found)
				{
					mappedEdges.insert(it->first);
				}
			}
			
			return findPaths(graph, allConnectedVertices, mappedEdges);
		}
		
		bool OldMinorOrder::findPaths(Hypergraph const& g, list<set<IDType> >& connV, set<IDType> mappedEdges) const
		{
			list<list<set<IDType> >* > allPathCombinations;
			set<IDType> forbidden;
			
			// All vertices which occure in the minor are not allowed to occure
			for (list<set<IDType> >::iterator
				it = connV.begin(); it != connV.end(); it++)
			{
				forbidden.insert(it->begin(), it->end());
				
			}
			forbidden.insert(mappedEdges.begin(), mappedEdges.end());
			
			// Also, no edge which is mapped by minor morphism is allowed to occure on a path
			
			//Iterate over all vertices of all edgeTupels
			for (list<set<IDType> >::iterator
					it = connV.begin(); it != connV.end(); it++)
			{
				//Create PathFinders
				list<PathFinder> pathFinders; //one list for each edge tupel
				
				set<IDType>& edgeVertices = *it;
				if (edgeVertices.size() <= 1) continue; //trivial path
				
				//f contains all forbidden vertices in G for our EdgeTupel
				//...except for those vertices which are in our edgeTupel
				set<IDType> f;
				set_difference(forbidden.begin(), forbidden.end(),
							edgeVertices.begin(), edgeVertices.end(), inserter(f, f.begin()));
				
				set<IDType>::iterator vertIt = edgeVertices.begin();
				IDType last = *vertIt;
				vertIt++;
					
				//Iterate over all vertices of one edgeTupel
				for (; vertIt != edgeVertices.end(); vertIt++)
				{
					pathFinders.push_back(PathFinder(g, last, *vertIt, f));
					last = *vertIt;
				}
				list<set<IDType> >* pathCombinations = new list<set<IDType> >;
				createCombinations(pathFinders, pathFinders.begin(), *pathCombinations);
				allPathCombinations.push_back(pathCombinations);
			}
			set<IDType> empty;
			return combinePaths(allPathCombinations.begin(),
								allPathCombinations.end(),
								empty);
			
		}
		
		bool OldMinorOrder::combinePaths(
							list<list<set<IDType> >* >::iterator it0,
							list<list<set<IDType> >* >::iterator end,
							set<IDType>& nodesUsed) const
		{
			if (it0 == end)
			{
				return true;
			}
			list<list<set<IDType> >* >::iterator it1 = it0;
			it1++;
			for (list<set<IDType> >::iterator it = (*it0)->begin(); it != (*it0)->end(); it++)
			{
				set<IDType> inters;
				set_intersection(nodesUsed.begin(), nodesUsed.end(),
								it->begin(), it->end(),
								inserter(inters, inters.begin()));
				if (inters.size() == 0)
				{
					set<IDType> nodes;
					nodes.insert(nodesUsed.begin(), nodesUsed.end());
					nodes.insert(it->begin(), it->end());
					if (combinePaths(it1, end, nodes))
					{
						return true;
					}
				}
			}
			return false;
		}
	
		void OldMinorOrder::createCombinations(list<PathFinder>& p,
								list<PathFinder>::iterator it,
								list<set<IDType> >& result) const
		{
			if (it == p.end()) {
				set<IDType> thisCombination;
				for (list<PathFinder>::iterator i = p.begin(); i != p.end(); i++)
				{
					const set<IDType>* path = i->getCurrentPath();
					thisCombination.insert(path->begin(), path->end());
				}
				result.push_back(thisCombination);
			}
			else
			{
				const set<IDType>* path = it->getNextPath();
				list<PathFinder>::iterator it2 = it;
				it2++;
				while (path != NULL)
				{
					createCombinations(p, it2, result);
					path = it->getNextPath();
				}
				it->reset();
			}
		}

		// ********************************************************************
		// **************** Implementation of PathFinder Class ****************
		// ********************************************************************

		OldMinorOrder::PathFinder::PathFinder ( Hypergraph const& graph, IDType start, IDType end, set<IDType> forbidden)
				: graph ( graph ), start ( start ), end ( end ), forbidden ( forbidden )
		{
			if ( !graph.hasVertex ( start ) )
			{
				cerr << "OldMinorOrder::PathFinder: id " << start << " has not been found in the graph";
				exit ( -1 );
			}
			if ( !graph.hasVertex ( end ) )
			{
				cerr << "OldMinorOrder::PathFinder: id " << end << " has not been found in the graph";
				exit ( -1 );
			}
			//assert(forbidden.count(start) == 0 && forbidden.count(end) == 0);
			currentPath = -1;
			//Initialisation
			if ( start != end )
			{
				auto multiEdge = *(graph.getConnectedEdges ( start ));
				//we don't need to consider loop-edges twice:
				set<IDType> edge;
				edge.insert(multiEdge.begin(), multiEdge.end());
				for ( set<IDType>::iterator
				        it = edge.begin(); it != edge.end(); it++ )
				{
					if (*it != start && forbidden.count(*it) == 0)
					{
						QueueEl e;
						e.position = *it;
						e.isEdge = true;
						e.innerPath = set<IDType>();
						sq.push ( e );
					}
				}
			}
			else
			{
				if (forbidden.count(start) == 0) //start==end
				{
					QueueEl e;
					e.position = start;
					e.isEdge = false;
					e.innerPath = set<IDType>();
					sq.push(e);
				}
			}

		}

		const set<IDType>* OldMinorOrder::PathFinder::getCurrentPath()
		{
			return &ids[currentPath];
		}
		const set<IDType>* OldMinorOrder::PathFinder::getNextPath()
		{
			currentPath++;
			if ( currentPath > ids.size() ) return NULL;
			if ( currentPath < ids.size() ) //reset() has been executed
			{
				return & ( ids[currentPath] );
			}
			
			bool found = false;
			while ( !found && sq.size() > 0)
			{
				QueueEl e = sq.front();
				if (sq.front().position == end) 
				{
					found = true;
					break;
				}

				if ( e.isEdge )
				{
					vector<IDType> const* vert = graph.getVerticesOfEdge(e.position);
					for ( vector<IDType>::const_iterator it = vert->begin(); it != vert->end(); it++ )
					{
						if ( *it != start && e.innerPath.count ( *it ) == 0 
							&& forbidden.count(*it) == 0 ) //avoid loops and forbidden graph elements
						{
							QueueEl newE;
							newE.position = *it;
							newE.isEdge = false;
							set<IDType> newSet = e.innerPath;
							newSet.insert ( e.position );
							// also insert all vertices which are adjacent to this edge
							vector<IDType> const* adj = graph.getVerticesOfEdge(e.position);
							newSet.insert( adj->begin(), adj->end() );
							newE.innerPath = newSet;
							sq.push(newE);
						}
					}
				}
				else
				{
					auto multiEdges = *(graph.getConnectedEdges ( e.position ));
					//we don't need to consider loop-edges twice:
					set<IDType> edges;
					edges.insert(multiEdges.begin(), multiEdges.end());
					for ( set<IDType>::iterator
					        it = edges.begin(); it != edges.end(); it++ )
					{
						if ( *it != start &&  e.innerPath.count ( *it ) == 0 
							&& forbidden.count(*it) == 0 ) //avoid loops and forbidden graph elements
						{
							QueueEl newE;
							newE.position = *it;
							newE.isEdge = true;
							set<IDType> newSet = e.innerPath;
							newSet.insert ( e.position );
							newE.innerPath = newSet;
							sq.push(newE);
						}
					}
				}
				sq.pop();
			}
			if (found)
			{
				ids.push_back(sq.front().innerPath);
				sq.pop();
				return &ids.back();
			}
			else //no more paths possible
			{
				return NULL;
			}

		}
		const set<IDType>* OldMinorOrder::PathFinder::getNextPath(set<IDType> tmpForbidden)
		{
			const set<IDType>* result = getNextPath();
			while (result != NULL)
			{
				set<IDType> intersection;
				set_intersection(tmpForbidden.begin(), tmpForbidden.end(),
							result->begin(), result->end(), inserter(intersection, intersection.begin()));
				if (intersection.size() == 0) return result;
				result = getNextPath();
			}
			return NULL;
		}

		void OldMinorOrder::PathFinder::reset()
		{
			currentPath = -1;
		}

	}

}
