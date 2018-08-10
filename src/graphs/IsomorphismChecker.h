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

#ifndef ALGORITHMS_HYPERGRAPHISOMORPHISM_CHECKER_H
#define ALGORITHMS_HYPERGRAPHISOMORPHISM_CHECKER_H

#include "Hypergraph.h"
#include <list>

namespace uncover {
	namespace graphs {

		/**
		 * Checks whether two given Hypergraphs are isomorphic or not. The check is performed by the algorithm
		 * described by Arend Rensink in "Isomorphism Checking in GROOVE" using signatures and certificates
		 * as heuristics.
		 * @author Jan Stückrath
		 * @author Marvon Heumüller
		 */
		class IsomorphismChecker
		{

			public:

				/**
				 * Destroys this IsomorphismChecker. Note that this class should not be instantiated.
				 */
				virtual ~IsomorphismChecker();

				/**
				 * Returns true if the two given graphs are isomorphic.
				 * @param hg1 the first graph to check
				 * @param hg2 the second graph to check
				 * @return true if the two given graphs are isomorphic
				 */
				static bool areIsomorphic(Hypergraph const& hg1, Hypergraph const& hg2);

			private:

				/**
				 * This constructor cannot/should not be called.
				 */
				IsomorphismChecker();

				/**
				 * Inserts a pair of the given IDs to the given forward and backward maps if the resulting Mapping is still
				 * bijective.
				 * @param forw the forward map to which (id0,id1) will be added
				 * @param backw the backward map to which (id1,id0) will be added
				 * @param id0 the first ID
				 * @param id1 the second ID
				 * @return true if the pairs where successfully added to the given maps
				 */
				static bool insertIfValid(map<IDType, IDType>& forw, map<IDType, IDType>& backw, IDType id0, IDType id1);

				/**
				 * Checks all possible mappings for the given parameter and returns true, if one is an isomorphism.
				 * @param h0 the first graph
				 * @param sigs0 the signature computed for the first graph
				 * @param h1 the second graph
				 * @param sigs1 the signature comupted for the second graph
				 * @param sigs1rev the inverse of the second graphs signature
				 * @param mappingFor the forward mapping; must be empty initially
				 * @param mappingBak the backward mapping; must be empty initially
				 * @param it must be an iterator to the first element of the edge map of the first graph
				 * @param end must be an iterator beyond the last element of the edge map of the first graph
				 *
				 */
				static bool mapping(
						Hypergraph const& h0,
						HGSignature& sigs0,
						Hypergraph const& h1,
						HGSignature& sigs1,
						map<PartSig, std::list<IDType> >& sigs1rev,
						map<IDType, IDType>& mappingsFor,
						map<IDType, IDType>& mappingsBack,
						Hypergraph::EMap::const_iterator it,
						Hypergraph::EMap::const_iterator end);

		};

	}
}

#endif
