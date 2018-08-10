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

#ifndef MINIMALGRAPHLIST_H_
#define MINIMALGRAPHLIST_H_

#include "../basic_types/Order.h"
#include "../analysis/BackwardResultHandler.h"

namespace uncover {
	namespace graphs {

		/**
		 * This data structure stores a collection of graphs minimal to some given order. A graph is only added to this
		 * list if there is no graph in the list which is less or equal to the graph. Additionally, after adding a graph
		 * all graphs which are now non-minimal are deleted.
		 * @see basic_typed::Order
		 * @author Jan Stückrath
		 */
		class MinGraphList {

			public:

				/**
				 * Generates a new MinGraphList using the given Order for minimization and the given BackwardResultHandler
				 * (if given) for storing intermediate results.
				 * @param order the Order to be used for minimization
				 * @param resHandler the BackwardResultHandler for storing intermediate results (use null if no storing is
				 * 				desired)
				 */
				MinGraphList(basic_types::Order_sp order, analysis::BackwardResultHandler_sp resHandler = nullptr);

				/**
				 * Destroys this MinGraphList.
				 */
				virtual ~MinGraphList();

				/**
				 * Returns the minimal graphs stores in this MinGraphList as a vector of Hypergraphs. Modifying the
				 * returned vector will invalidate this MinGraphList.
				 * @return the minimal graphs as a vector
				 */
				shared_ptr<vector<Hypergraph_sp>> getGraphs();

				/**
				 * Adds the given graph to this MinGraphList, if there is no graph already in the list which is smaller
				 * of equal to the given graph. After that, the list is minimized.
				 * @param graph the graph to be added
				 * @param deletedGraphs if not null, all deleted graphs will be added to this vector
				 * @return true, iff the graph was added
				 */
				bool addGraph(Hypergraph_sp graph, shared_ptr<vector<Hypergraph_sp>> deletedGraphs = nullptr);

				/**
				 * Adds the given collection of graphs to this MinGraphList. Every non-minimal graph in the collection
				 * is immediately dropped.
				 * @param graphs the collection of graphs to be added
				 * @return the number of graphs effectively added to this MinGraphList
				 */
				size_t addGraphs(vector<Hypergraph_sp> const& graphs);

				/**
				 * Returns the number of minimal graphs stored in this list.
				 * @return the number of minimal grapgs stored in this list
				 */
				size_t size() const;

				/**
				 * Checks whether a graph with the given ID is stored in this list.
				 * @param graphID the ID to be checked
				 * @return true, iff a graph with the given ID is stored in this list
				 */
				bool contains(IDType graphID) const;

			private:

				/**
				 * Stores the Order used for minimization.
				 */
				basic_types::Order_sp order;

				/**
				 * Stores the minimal graphs.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> graphs;

				/**
				 * Stores the BackwardResultHandler for handling intermediate results.
				 */
				analysis::BackwardResultHandler_sp resHandler;

				/**
				 * Caches all IDs of graphs currently stored in this list.
				 */
				unordered_set<IDType> idsInList;

		};

	} /* namespace graphs */
} /* namespace uncover */

#endif /* MINIMALGRAPHLIST_H_ */
