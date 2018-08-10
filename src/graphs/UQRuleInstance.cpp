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

#include "UQRuleInstance.h"
#include "../logging/message_logging.h"

using namespace std;
using namespace uncover::logging;

namespace uncover {
	namespace graphs {

		UQRuleInstance::UQRuleInstance(AnonRule const& rule,
				unordered_set<IDType> const& leftUQVertices,
				unordered_set<IDType> const& rightUQVertices) :
			AnonRule(rule), leftUQVertices(leftUQVertices), rightUQVertices(rightUQVertices) {}

		UQRuleInstance::UQRuleInstance(string name, AnonHypergraph_sp leftGraph, AnonHypergraph_sp rightGraph, Mapping const& mapping,
								unordered_set<IDType> const& leftUQVertices,
								unordered_set<IDType> const& rightUQVertices) :
			AnonRule(name, leftGraph, rightGraph, mapping), leftUQVertices(leftUQVertices), rightUQVertices(rightUQVertices) {}

		UQRuleInstance::UQRuleInstance(AnonHypergraph const& graph, unordered_set<IDType> const& uqVertices) :
				AnonRule(graph), leftUQVertices(uqVertices), rightUQVertices(uqVertices) {}

		UQRuleInstance::~UQRuleInstance() {}

		unordered_set<IDType>& UQRuleInstance::getLeftUQVertices() {
			return leftUQVertices;
		}

		unordered_set<IDType> const& UQRuleInstance::getLeftUQVertices() const {
			return leftUQVertices;
		}

		unordered_set<IDType>& UQRuleInstance::getRightUQVertices() {
			return rightUQVertices;
		}

		unordered_set<IDType> const& UQRuleInstance::getRightUQVertices() const {
			return rightUQVertices;
		}

		bool UQRuleInstance::isBackApplicable(AnonHypergraph const& graph, Mapping const& match) const {

			if(!AnonRule::isBackApplicable(graph,match)) {
				return false;
			}

			for(auto vIt = rightUQVertices.cbegin(); vIt != rightUQVertices.cend(); ++vIt) {

				if(!match.hasSrc(*vIt)) {
					logger.newError() << "A UQRuleInstance is naturally not backward applicable, because a given match was partial!" << endLogMessage;
					return false;
				}

				if(right->getConnEdgesCount(*vIt) != graph.getConnEdgesCount(match.getTar(*vIt))) {
					return false;
				}

			}

			return true;
		}

		void UQRuleInstance::fillSplitRule(pair<AnonRule_sp, AnonRule_sp>& rulePair) const {

			UQRuleInstance_sp total = make_shared<UQRuleInstance>(*this);
			UQRuleInstance_sp partial = make_shared<UQRuleInstance>(*this->getLeft(), this->getLeftUQVertices());
			adjustSplitRule(*partial, *total);

			// remove left UQ vertices of total part if necessary
			vector<IDType> deadIDs;
			for(unordered_set<IDType>::const_iterator totIt = total->getLeftUQVertices().cbegin();
					totIt != total->getLeftUQVertices().cend(); ++totIt) {
				if(!total->getLeft()->hasVertex(*totIt)) {
					deadIDs.push_back(*totIt);
				}
			}

			for(vector<IDType>::const_iterator deadIt = deadIDs.cbegin(); deadIt != deadIDs.cend(); ++deadIt) {
				total->getLeftUQVertices().erase(*deadIt);
			}

			// remove right UQ vertices of partial part if necessary
			deadIDs.clear();
			for(unordered_set<IDType>::const_iterator partIt = partial->getRightUQVertices().cbegin();
					partIt != partial->getRightUQVertices().cend(); ++partIt) {
				if(!partial->getRight()->hasVertex(*partIt)) {
					deadIDs.push_back(*partIt);
				}
			}

			for(vector<IDType>::const_iterator deadIt = deadIDs.cbegin(); deadIt != deadIDs.cend(); ++deadIt) {
				partial->getRightUQVertices().erase(*deadIt);
			}

			rulePair.first = partial;
			rulePair.second = total;
		}

	}
}
