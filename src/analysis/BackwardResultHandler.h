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

#ifndef BACKWARDRESULTHANDLER_H_
#define BACKWARDRESULTHANDLER_H_

#include "PreparedGTS.h"

namespace uncover {
	namespace analysis {

		/**
		 * The BackwardResultHandler is an interface to handle intermediate results produced by the backward
		 * search. Which and how intermediate results are stored depends on the implementation.
		 * @author Jan Stückrath
		 */
		class BackwardResultHandler {

			public:

				/**
				 * Destroys the current BackwardResultHandler.
				 */
				virtual ~BackwardResultHandler() {}

				/**
				 * Sets the old unprepared GTS.
				 * @param gts the old unprepared GTS
				 */
				virtual void setOldGTS(graphs::GTS_csp gts) = 0;

				/**
				 * Sets the new prepared GTS.
				 * @param gts the new prepared GTS
				 */
				virtual void setPreparedGTS(PreparedGTS_csp gts) = 0;

				/**
				 * Sets the initial set of graphs from which the backward search is started.
				 * @param graphs the initial set of graphs for the backward search
				 */
				virtual void setFirstGraphs(shared_ptr<const vector<graphs::Hypergraph_sp>> graphs) = 0;

				/**
				 * Stores that the graph with the first ID was deleted because it was subsumed by the graph
				 * with the second ID.
				 * @param deletedGraph the subsumed graph
				 * @param reasonGraph the graph subsuming the first graph
				 */
				virtual void wasDeletedBecauseOf(IDType deletedGraph, IDType reasonGraph) = 0;

				/**
				 * Stores that the graph with the given ID did not belong to the restricted set of graphs and
				 * was therefore dropped.
				 * @param deletedGraph the graph violating the restriction proposed by the backward search
				 */
				virtual void wasNoValidGraph(IDType deletedGraph) = 0;

				/**
				 * Stores that the given graph was generated from the second graph by the given rule.
				 * @param genGraph the graph generated
				 * @param rule the ID of the rule used to generate the first graph
				 * @param srcGraph the ID of the graph used to generate the first graph
				 */
				virtual void wasGeneratedByFrom(graphs::Hypergraph_sp genGraph, IDType rule, IDType srcGraph) = 0;

				/**
				 * Informs that the next backward step has begun.
				 */
				virtual void startingNewBackwardStep() = 0;

				/**
				 * Informs that the current backward step has ended.
				 */
				virtual void finishedBackwardStep() = 0;

			protected:

				/**
				 * Creates a new BackwardResultHandler. Can only be called from a subclass.
				 */
				BackwardResultHandler() {}

		};

		/**
		 * Alias for a shared pointer to a BackwardResultHandler.
		 */
		typedef shared_ptr<BackwardResultHandler> BackwardResultHandler_sp;

	}
}



#endif /* BACKWARDRESULTHANDLER_H_ */
