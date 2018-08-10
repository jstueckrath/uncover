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

#include "PathChecker.h"

namespace uncover {
	namespace graphs {

		// this constructur cannot/should not be called
		PathChecker::PathChecker() {}

		PathChecker::~PathChecker() {}

		bool PathChecker::hasPath(Hypergraph const& graph, unsigned int length) {

			// search for a path beginning from all possible nodes
			unordered_set<IDType> visitedElements;
			for(Hypergraph::VMap::const_iterator it = graph.cbeginVertices(); it != graph.cendVertices(); ++it) {
				visitedElements.insert(it->first);
				if(findPath(graph,visitedElements,it->first,0,length)) {
					return true;
				}
				visitedElements.erase(it->first);
			}
			return false;
		}

		bool PathChecker::findPath(Hypergraph const& graph,
				unordered_set<IDType>& visited,
				IDType currentVertex,
				unsigned int currentPath,
				unsigned int maxPath) {

			// terminate immediately if maximal length is reached
			if(currentPath >= maxPath) {
				return true;
			} else {

				// try to use all edges attached to current node, to extend path
				Hypergraph::ESet const* posEdges = graph.getConnectedEdges(currentVertex);
				for(Hypergraph::ESet::const_iterator posEdgesIt = posEdges->begin(); posEdgesIt != posEdges->end(); ++posEdgesIt) {
					// only check edges if they are not already used in the path
					if(visited.count(*posEdgesIt) == 0) {
						visited.insert(*posEdgesIt); // mark edge
						unordered_set<IDType> multiNodes;

						// try to use all nodes to extend the path
						vector<IDType> const* nodes = graph.getVerticesOfEdge(*posEdgesIt);
						for(vector<IDType>::const_iterator nodesIt = nodes->begin(); nodesIt != nodes->end(); ++nodesIt) {
							// check nodes only if they are not already used in the path or were already checked
							if((visited.count(*nodesIt) == 0) && (multiNodes.count(*nodesIt) == 0)) {
								multiNodes.insert(*nodesIt);
								visited.insert(*nodesIt); // mark node
								// terminate immediately if too long path was found
								if(findPath(graph,visited,*nodesIt,currentPath+1,maxPath)) {
									return true;
								}
								visited.erase(*nodesIt); // unmark node
							} // else ignore node
						}

						visited.erase(*posEdgesIt); // unmark edges
					} // else ignore edge
				}

			}
			return false;
		}

	} /* namespace graphs */
} /* namespace uncover */
