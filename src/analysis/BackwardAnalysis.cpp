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

#include "BackwardAnalysis.h"
#include "../basic_types/RunException.h"
#include "../logging/message_logging.h"
#include <time.h>
#include <sstream>

using namespace std;
using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::logging;

namespace uncover {
	namespace analysis {

		BackwardAnalysis::BackwardAnalysis(GTS_sp gts,
				shared_ptr<vector<Hypergraph_sp>> errorGraphs,
				Order_sp order,
				RulePreparer_sp rulePreparer,
				MinPOCEnumerator_sp pocEnumerator,
				bool checkInitial,
				BackwardResultHandler_sp resultHandler,
				unsigned int timeout)
		: gts(gts),
		  errorGraphs(errorGraphs),
		  order(order),
		  rulePreparer(rulePreparer),
		  pocEnumerator(pocEnumerator),
		  checkInitial(checkInitial),
		  resultHandler(resultHandler),
		  timer((timeout==0) ? nullptr : new Timer(timeout)) {}

		BackwardAnalysis::~BackwardAnalysis() {
			if(timer) delete timer;
		}

		shared_ptr<vector<Hypergraph_sp>> BackwardAnalysis::performAnalysis() {

			// check if all needed parameters are defined
			if(gts == nullptr) {
				throw RunException("No GTS was given for the backward search.");
			}

			if(errorGraphs == nullptr) {
				throw RunException("No error graphs were given for the backward search.");
			}

			if(order == nullptr) {
				throw RunException("No order was given for the backward search.");
			}

			if(rulePreparer == nullptr) {
				throw RunException("No rule preparer was given for the backward search.");
			}

			if(pocEnumerator == nullptr) {
				throw RunException("No enumerator for minimal pushout complements was given for the backward search.");
			}

			// start timer if existent
			if(timer) {
				timer->start();
			}

			// prepare rule set and generate new gts
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(*gts,rulePreparer);
			logger.newUserInfo() << "The rule preparer generated a new set of (standard) rules now containing ";
			logger.contMsg() << prepGTS->getStdRuleCount() << " rules (old set contained " << gts->getStdRules().size() << " rules). ";
			logger.contMsg() << "The " << prepGTS->getUQRuleCount() << " universally quantified rules where not prepared." << endLogMessage;
			pocEnumerator->setResultHandler(resultHandler);
			pocEnumerator->setPreparedGTS(prepGTS);

			// Process all preparation results, if ResultHandler is defined
			if(resultHandler) {
				resultHandler->setOldGTS(gts);
				resultHandler->setPreparedGTS(prepGTS);
				resultHandler->setFirstGraphs(errorGraphs);
			}

			// generate all needed graph lists
			MinGraphList minimalGraphs(order, resultHandler);
			minimalGraphs.addGraphs(*errorGraphs);
			vector<Hypergraph_sp>* todoGraphs = new vector<Hypergraph_sp>(*errorGraphs);
			vector<Hypergraph_sp>* newGraphs = new vector<Hypergraph_sp>();

			// check timeout before starting backward step computation
			if(timer && timer->outOfTime()) {
				logger.newWarning() << "Timeout reached before first backward step! Stopping." << endLogMessage;
				return minimalGraphs.getGraphs();
			}

			size_t searchDepth = 1;
			while (todoGraphs->size() > 0) {

				logger.newUserInfo() << "Starting with search depth " << searchDepth << ", ";
				logger.contMsg() << todoGraphs->size() << " new graphs to process." << endLogMessage;

				if(resultHandler) {
					resultHandler->startingNewBackwardStep();
				}

				size_t todoCounter = 0;
				size_t todoSize = todoGraphs->size();
				while(todoGraphs->size() > 0) {

					Hypergraph_sp currentGraph = todoGraphs->back();
					todoGraphs->pop_back();

					// check if a graph smaller than the current graph was found in the meantime
					if(!minimalGraphs.contains(currentGraph->getID())) {
						logger.newUserInfo() << "Skipping graph " << ++todoCounter << " of " << todoSize;
						logger.contMsg() << " (id=" << currentGraph->getID() << "), it was already subsumed by another graph." << endLogMessage;
						continue;
					}

					logger.newUserInfo() << "Processing graph " << ++todoCounter << " of " << todoSize << " (size: ";
					logger.contMsg() << currentGraph->getVertexCount() << " vertices, " << currentGraph->getEdgeCount() << " edges)." << endLogMessage;

					// apply all rules backward one at a time
					auto ruleMap = prepGTS->getStdRules();
					for(auto ruleIt = ruleMap.cbegin(); ruleIt != ruleMap.cend(); ++ruleIt) {
						pocEnumerator->resetWith(ruleIt->second, currentGraph);
						if(applyOne(minimalGraphs, *newGraphs)) {
							return minimalGraphs.getGraphs();
						}
					}

					// do the same for all universally quantified rules
					auto& ruleMapUQ = prepGTS->getUQRules();
					for(auto ruleIt = ruleMapUQ.cbegin(); ruleIt != ruleMapUQ.cend(); ++ruleIt) {

						// generate all instances up to the necessary bound
						for(auto instIt = prepGTS->beginUQInstances(ruleIt->first, *currentGraph);	!instIt.hasEnded(); ++instIt) {
							pocEnumerator->resetWith(*instIt, ruleIt->first, currentGraph);
							if(applyOne(minimalGraphs, *newGraphs)) {
								return minimalGraphs.getGraphs();
							}
						}

					}

				}

				// initialize lists for next round
				delete todoGraphs;
				todoGraphs = newGraphs;
				newGraphs = new vector<Hypergraph_sp>();

				if(resultHandler) {
					resultHandler->finishedBackwardStep();
				}

				++searchDepth;

			}

			logger.newUserInfo() << "Backward search terminated finding " << minimalGraphs.size() << " error graphs." << endLogMessage;

			// set names of all error graphs if they don't already have a name
			auto result = minimalGraphs.getGraphs();
			for(auto graph : *result) {
				if(graph->getName().empty()) {
					stringstream s;
					s << "EG" << graph->getID();
					graph->setName(s.str());
				}
			}

			return result;
		}

		bool BackwardAnalysis::applyOne(
				MinGraphList& minList,
				vector<Hypergraph_sp>& newGraphs) {

			while(!pocEnumerator->ended()) {

				Hypergraph_sp newGraph = **pocEnumerator;
				if(minList.addGraph(newGraph)) {

					newGraphs.push_back(newGraph);

					if(checkInitial) {

						// note: the preparation does not change the initial graphs
						auto initialGraphs = gts->getInitialGraphs();
						for(auto initIt = initialGraphs.cbegin(); initIt != initialGraphs.cend(); ++initIt) {

							if(order->isLessOrEq(*newGraph, **initIt)) {
								logger.newUserInfo() << "One of the initial graphs was covered. ";
								logger.contMsg() << "Aborting backward step and terminating after minimization." << endLogMessage;
								return true;
							}

						}

					}

				} // else the new graph was subsumed already

				// check timeout
				if(timer && timer->outOfTime()) {
					logger.newWarning() << "Stopping because of timeout reached. Discarding unfinished backward step results." << endLogMessage;
					return true;
				}

				++(*pocEnumerator);
			}

			return false;
		}

	} /* namespace analysis */
} /* namespace uncover */
