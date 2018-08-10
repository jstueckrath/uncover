/***************************************************************************
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

#ifndef PATHCHECKER_H_
#define PATHCHECKER_H_

#include "Hypergraph.h"

namespace uncover {
	namespace graphs {

		/**
		 * The PathChecker class provides functions to compute whether given graphs have paths of a defined length.
		 * @author Jan Stückrath
		 */
		class PathChecker {

			public:

				/**
				 * Destroys the current instance of PathChecker.
				 */
				virtual ~PathChecker();

				/**
				 * Checks if the given graph has a path with at least the given length.
				 * @param graph the graph to be checked
				 * @param length the length to be used in the check
				 * @return true, iff the given graph contains a path of length 'length'
				 */
				static bool hasPath(Hypergraph const& graph, unsigned int length);

			protected:

				/**
				 * Recursively searches for a path in the given graph which has the given maximal length.
				 * The other parameters are used for the recursion.
				 * @param graph the graph to be checked
				 * @param visited the set of all node and edge IDs of elements of the currently found path (initially only currentVertex)
				 * @param currentVertex the vertex ID from which the currently found path will be extended (initially some vertex ID)
				 * @param currentPath the length of the currently found path (initially zero)
				 * @param maxPath the pathlength to be checked; terminates if reached
				 * return true, iff the given graph contains a path of length 'length'
				 */
				static bool findPath(Hypergraph const& graph,
						unordered_set<IDType>& visited,
						IDType currentVertex,
						unsigned int currentPath,
						unsigned int maxPath);

			private:

				/**
				 * PathChecker cannot be instantiated.
				 */
				PathChecker();

		};

	} /* namespace graphs */
} /* namespace uncover */

#endif /* PATHCHECKER_H_ */
