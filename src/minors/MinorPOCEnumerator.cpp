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

#include "MinorPOCEnumerator.h"
#include "../logging/message_logging.h"

using namespace uncover::analysis;
using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::rule_engine;
using namespace uncover::logging;

namespace uncover {
	namespace minors {

		MinorPOCEnumerator::MinorPOCEnumerator()
			: partialEnumerator(nullptr),
			  ruleID(0),
			  graphID(0) {}

		MinorPOCEnumerator::MinorPOCEnumerator(MinorPOCEnumerator&& oldEnum)
			:	partialEnumerator(oldEnum.partialEnumerator),
			 	ruleID(oldEnum.ruleID),
			 	graphID(oldEnum.graphID) {
			oldEnum.partialEnumerator = nullptr;
		}

		MinorPOCEnumerator::~MinorPOCEnumerator() {}

		bool MinorPOCEnumerator::ended() const {
			checkAndThrow();
			return partialEnumerator->ended();
		}

		MinorPOCEnumerator& MinorPOCEnumerator::operator ++() {

			checkAndThrow();
			++(*partialEnumerator);

			if(resultHandler && !partialEnumerator->ended()) {
				resultHandler->wasGeneratedByFrom(partialEnumerator->getGraph(),ruleID,graphID);
			}

			return *this;
		}

		graphs::Hypergraph_sp MinorPOCEnumerator::operator *() const {
			checkAndThrow();
			return partialEnumerator->getGraph();
		}

		bool MinorPOCEnumerator::resetWith(AnonRule_csp rule, IDType ruleID, Hypergraph_csp graph) {

			if(rule == nullptr || graph == nullptr) {
				return false;
			}

			this->ruleID = ruleID;
			graphID = graph->getID();

			shared_ptr<vector<AnonRule_csp>> originalRules = nullptr;
			if(prepGTS) {
				originalRules = prepGTS->getOriginalRules(ruleID);
			} else {
				logger.newError() << "Conflict-free matches need original rules to be checked against, ";
				logger.contMsg() << "but no PreparedGTS was given! I assume that all matches are fine, but it may ";
				logger.contMsg() << "cause results to be incorrect." << endLogMessage;
			}

			if(partialEnumerator) delete partialEnumerator;
			partialEnumerator = new PartialPOCEnumerator(rule,graph,false,originalRules);

			if(resultHandler && !partialEnumerator->ended()) {
				resultHandler->wasGeneratedByFrom(partialEnumerator->getGraph(),ruleID,graphID);
			}

			return true;

		}

		void MinorPOCEnumerator::checkAndThrow() const {
			if(!partialEnumerator) {
				throw InvalidStateException("Cannot perform actions with this MinorPOCEnumerator, "
						"since it was not initialized.");
			}
		}

	} /* namespace minors */
} /* namespac uncover */
