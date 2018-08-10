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

#include "SubgraphPOCEnumerator.h"
#include "../logging/message_logging.h"
#include "../graphs/PathChecker.h"

using namespace uncover::rule_engine;
using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::logging;

namespace uncover {
	namespace subgraphs {

		SubgraphPOCEnumerator::SubgraphPOCEnumerator(bool inj, int bound)
			:	pathbound(bound),
			 	injective(inj),
				partialEnumerator(nullptr),
				ruleID(0),
				graphID(0) {}

		SubgraphPOCEnumerator::SubgraphPOCEnumerator(SubgraphPOCEnumerator&& oldEnum)
			: pathbound(oldEnum.pathbound),
			  injective(oldEnum.injective),
			  partialEnumerator(oldEnum.partialEnumerator),
			  ruleID(oldEnum.ruleID),
			  graphID(oldEnum.graphID) {

			oldEnum.partialEnumerator = nullptr;

		}

		SubgraphPOCEnumerator::~SubgraphPOCEnumerator() {
			if(partialEnumerator) delete partialEnumerator;
		}

		bool SubgraphPOCEnumerator::ended() const {
			checkAndThrow();
			return partialEnumerator->ended();
		}

		bool SubgraphPOCEnumerator::resetWith(AnonRule_csp rule, IDType ruleID, Hypergraph_csp graph) {

			if(rule == nullptr || graph == nullptr) {
				return false;
			}

			this->ruleID = ruleID;
			graphID = graph->getID();

			// ignore original rules in the injective case
			shared_ptr<vector<AnonRule_csp>> originalRules = nullptr;
			if(!injective) {
				if(prepGTS) {
					originalRules = prepGTS->getOriginalRules(ruleID);
				} else {
					logger.newError() << "Conflict-free matches need original rules to be checked against, ";
					logger.contMsg() << "but no PreparedGTS was given! I assume that all matches are fine, but it may ";
					logger.contMsg() << "cause results to be incorrect." << endLogMessage;
				}
			}

			if(partialEnumerator) delete partialEnumerator;
			partialEnumerator = new PartialPOCEnumerator(rule,graph,injective,originalRules);

			if(!partialEnumerator->ended()) {

				if(resultHandler) {
					resultHandler->wasGeneratedByFrom(partialEnumerator->getGraph(),ruleID,graphID);
				}

				// this will find the first graph not exceeding the pathbound
				if(exceedsBound()) ++(*this);
			}

			return true;
		}

		SubgraphPOCEnumerator& SubgraphPOCEnumerator::operator++() {

			checkAndThrow();
			++(*partialEnumerator);
			if(resultHandler && !partialEnumerator->ended()) {
				resultHandler->wasGeneratedByFrom(partialEnumerator->getGraph(),ruleID,graphID);
			}

			while(!partialEnumerator->ended() && exceedsBound()) {

				++(*partialEnumerator);
				if(resultHandler && !partialEnumerator->ended()) {
					resultHandler->wasGeneratedByFrom(partialEnumerator->getGraph(),ruleID,graphID);
				}

			}

			return *this;

		}

		Hypergraph_sp SubgraphPOCEnumerator::operator*() const {
			checkAndThrow();
			return partialEnumerator->getGraph();
		}

		void SubgraphPOCEnumerator::checkAndThrow() const {
			if(!partialEnumerator) {
				throw InvalidStateException("Cannot perform actions with this SubgraphPOCEnumerator, "
						"since it was not initialized.");
			}
		}

		bool SubgraphPOCEnumerator::exceedsBound() const {

			if(pathbound >= 0) {
				if(PathChecker::hasPath(*partialEnumerator->getGraph(),pathbound+1)) {

					if(resultHandler) {
						resultHandler->wasNoValidGraph(partialEnumerator->getGraph()->getID());
					}

					logger.newUserInfo() << "A graph (with id=" << partialEnumerator->getGraph()->getID();
					logger.contMsg() << ") has exceeded the pathbound and was dropped." << endLogMessage;

					return true;

				}
			}

			return false;
		}


	} /* namespace subgraphs */
} /* namespace uncover */
