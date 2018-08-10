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

#ifndef OldMinorOrder_H_
#define OldMinorOrder_H_

#include <queue>
#include <list>
#include <set>
#include "../graphs/Hypergraph.h"
#include "../basic_types/Order.h"

namespace uncover {
	namespace minors {

		/**
		 * This class is an old implementation of the Hypergraph Minor Ordering based on searching for disjoint paths.
		 * It is only correct for directed graphs, i.e. where every edge is incident to at most two nodes.
		 * @deprecated use the MinorOrder instead
		 * @see MinorOrder
		 * @author Jan Stückrath
		 * @author Marvin Heumüller
		 */
		class OldMinorOrder : public basic_types::Order
		{

			class PathFinder;

			public:

				/**
				 * Creates a new OldMinorOrder object.
				 */
				OldMinorOrder();

				/**
				 * Destroys the current OldMinorOrder object.
				 */
				virtual ~OldMinorOrder();

				/**
				 * Checks whether the first given graph is a minor of the second.
				 * @param smaller the possible minor
				 * @param bigger the possibly larger graph
				 * @return true iff the first graph is a minor of the second
				 */
				virtual bool isLessOrEq(graphs::Hypergraph const& smaller, graphs::Hypergraph const& bigger) const override;

			private:

				typedef struct
				{
					vector<IDType> minor;
					vector<IDType> graph;
				} twoIDLists ;

				/**
				 * Finds a mapping of an edge in M in G. Function calls itself
		     * recursively. When all edges are mapped, the minor test process
		     * continues. If the result is bad, the function tries gradually any other
		     * mapping.
		     * Calls createEdgetupels.
				 */
				bool mapping(	graphs::Hypergraph const& minor,
						graphs::Hypergraph const& graph,
						std::map<string, vector<IDType> >& occurencesG,
						vector<pair<IDType, string> >& occurencesM,
						size_t currentI,
						std::map<IDType, IDType>& mappings) const;

				/**
		     * creates from the given edge-matches vertex-matches. This is done by
		     * checking every vertex m in M and find a set of vertices <g> in G which are adjacent (correct port number) to every mapping of the edges which
		     * are adjacent to m.
		     * Calls FindPaths.
				 */
				bool createEdgeTupels(graphs::Hypergraph const& minor,
						graphs::Hypergraph const& graph,
						std::map<IDType, IDType>& mappings) const;


				/**
		     * Based on the edgeTupels, sets of pathFinders are created for every
		     * edgeTupel. If edgeTupel t consists of vertices a, b, c, PathFinders p1 from a to b and p2 from b to c are created.
		     * Calls createCombinations in every step.
		     * Calls CombinePaths.
				 */
				bool findPaths(graphs::Hypergraph const& g, std::list<std::set<IDType> >& connV, std::set<IDType> mappedEdges) const;

				/**
		     * is called for each edgeTupel. Creates for each path combination a set
		     * of vertices and edges which lie on these paths. Each of those set will
		     * be added to the resulting list of possible combinations.
				 */
				void createCombinations(std::list<PathFinder>& p,
						std::list<PathFinder>::iterator it,
						std::list<std::set<IDType> >& result) const;

				/**
		     * Tries all possible combinations of all possible combinations of paths
		     * of edgeTupels. Long sentence, long act. Calls itself recursively until
		     * a combination of all necessary _disjoint_ paths is found.
		     * Returns true in case of success.
				 */
				bool combinePaths(std::list<std::list<std::set<IDType> >* >::iterator it0,
						std::list<std::list<std::set<IDType> >* >::iterator end,
						std::set<IDType>& nodesUsed) const;


				/**
		     * Searching for paths between two vertices. Each time getNextPath
		     * is executed, a new path will be searched. As result, you will get the
		     * ids of the vertices which lie on the path. If no (more) path is found,
		     * the function will return null. To iterate over paths which already
		     * have been found, call the reset method and then call getNextPath again.
		     * @author Marvin Heumüller
				 */
				class PathFinder
				{

					public:

						PathFinder(graphs::Hypergraph const& graph, IDType start, IDType end, std::set<IDType> forbidden);

						~PathFinder() {}

						const std::set<IDType>* getNextPath();

						const std::set<IDType>* getNextPath(std::set<IDType> tmpForbidden);

						const std::set<IDType>* getCurrentPath();

						void reset();

						int getPathCount() { return currentPath; }

					private:

						typedef struct
						{

							IDType position; //Position to search next

							bool isEdge; //Is element at position an edge?

							std::set<IDType> innerPath; //All vertices and edges on the path except for start and end nodes

						} QueueEl;

						graphs::Hypergraph const& graph; // The graph in which we search for a path

						IDType start; //Start vertex

						IDType end; //End vertex

						vector<std::set<IDType> > ids; //All Paths found so far

						std::queue<QueueEl> sq; //Current queue for breadth-first search

						std::set<IDType> forbidden; //forbidden element ids on path

						int currentPath; //Current Position in ids

				};

		};

	}
}

#endif
