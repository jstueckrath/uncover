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

#ifndef BACKWARDANALYSIS_H_
#define BACKWARDANALYSIS_H_

#include "../basic_types/Order.h"
#include "../basic_types/Timer.h"
#include "../graphs/MinGraphList.h"
#include "../graphs/GTS.h"
#include "RulePreparer.h"
#include "MinPOCEnumerator.h"
#include "BackwardResultHandler.h"

namespace uncover {
	namespace analysis {

		/**
		 * This class performs a backward search to solve the coverability problem with respect to a given Order.
		 * The GTS and Order have to form a Q-restricted well-structured transition system for this algorithm to
		 * be sound and complete. See the following publication for more details about this algorithm:
		 * Barbara König and Jan Stückrath. A general framework for well-structured graph transformation systems.
		 * In P. Baldan and D. Gorla, editors, Proc. of CONCUR 2014, volume 8704 of LNCS, pages 467––481. Springer, 2014.
		 * @author Jan Stückrath
		 * @see analysis::BackwardStep
		 */
		class BackwardAnalysis {

			public:

				/**
				 * Generates and initializes a new BackwardAnalysis object with the given parameters. The GTS has to be
				 * monotone with respect to the Order. The Order has to be a wqo (possibly on a restricted set of
				 * graphs) and the Order, RulePreparer and MinPOCEnumerator have to be compatible. There are three
				 * optional parameters. A boolean can be used to activate a check if a graph smaller or equal to an
				 * initial graph was reached (default = false). A BackwardResultHandler can be given to store or display
				 * intermediate results (default = deactivated). A timeout can be given, to stop (softly) too long
				 * computations (default = deactivated).
				 * @param gts the GTS to be used, has to be well-structured
				 * @param errorGraphs the initial set of undesired graphs (representing all error configurations)
				 * @param order the order used
				 * @param rulePreparer a RulePreparer compatible to the order
				 * @param pocEnumerator a MinPOCEnumerator compatible to the order
				 * @param checkInitial if true, the analysis will stop as soon as an initial graph or a smaller graph was
				 * 				reached; false will deactivate this check
				 * @param resultHandler a BackwardResultHandler for handling intermediate results
				 * @param timeout a threshold after which the computation will be stopped (soft stop); a timeout of 0
				 * 				suppresses this check
				 */
				BackwardAnalysis(graphs::GTS_sp gts,
						shared_ptr<vector<graphs::Hypergraph_sp>> errorGraphs,
						basic_types::Order_sp order,
						RulePreparer_sp rulePreparer,
						MinPOCEnumerator_sp pocEnumerator,
						bool checkInitial = false,
						BackwardResultHandler_sp resultHandler = nullptr,
						unsigned int timeout = 0);

				/**
				 * Destroys the current instance of this class freeing all raw pointed data!
				 */
				virtual ~BackwardAnalysis();

				/**
				 * This function performs the backwards analysis with the parameters given during initialization. The
				 * search terminates when a backward step leads to no new graphs, a graph smaller or equal to an initial
				 * graph is reached (if set) or the timer threshold was exceeded (if set). The resulting set of minimal
				 * graphs is returned. Note that the set of minimal graphs may be incomplete if one of the latter two
				 * conditions caused termination.
				 * @return a vector containing all minimal graphs resulting from this backward search; may be incomplete
				 * 				 if the search terminated due to a timeout or a found initial graph
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> performAnalysis();

			private:

				/**
				 * Takes the given rule and graph and computes all minimal representants of predecessor of the
				 * upward-closure of the graph. If a computed graph was not immediately subsumed by a graph in the
				 * given MinGraphList, it is added to the MinGraphList and the given vector. If these checks are
				 * activated, after every computed predecessor it is checked if an initial graph (or anything smaller)
				 * was found or a timeout was reached, which will cause the function to terminate early. The
				 * MinPOCEnumerator used must be reset immediately before calling this function.
				 * @param minList all computed graphs will be added to this list (and may be immediately deleted by it)
				 * @param newGraphs all graphs not dropped my the MinGraphList will be added to this vector
				 * @return true, iff the function terminated because of a timeout or found initial graph
				 */
				bool applyOne(graphs::MinGraphList& minList, vector<graphs::Hypergraph_sp>& newGraphs);

				/**
				 * Stores the GTS used for the backwards search (before rule preparation).
				 */
				graphs::GTS_sp gts;

				/**
				 * Stores the error graphs used as initial graphs for the backward search.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> errorGraphs;

				/**
				 * Stores the Order used for minimization. Must be compatible with the RulePreparer and the
				 * MinPOCEnumerator.
				 */
				basic_types::Order_sp order;

				/**
				 * Stores the RulePreparer used for preprocessing the rule set. Must be compatible with the Order and
				 * the MinPOCEnumerator.
				 */
				RulePreparer_sp rulePreparer;

				/**
				 * Stores the MinPOCEnumerator used to compute all minimal pushout complements for a pair of rule and
				 * graph. Must be compatible with the Order and RulePreparer.
				 */
				MinPOCEnumerator_sp pocEnumerator;

				/**
				 * If set to true, the analysis will check if a graph smaller or equal to an initial graph was found,
				 * and stop early if this is the case.
				 */
				bool checkInitial;

				/**
				 * Stores the BackwardResultHandler used to process intermediate results of the backward analysis.
				 */
				BackwardResultHandler_sp resultHandler;

				/**
				 * Stores a Timer to handle premature timeout termination.
				 */
				basic_types::Timer* timer;

		};

	} /* namespace analysis */
} /* namespace uncover */

#endif /* BACKWARDANALYSIS_H_ */
