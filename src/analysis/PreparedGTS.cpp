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

#include "PreparedGTS.h"
#include <stdexcept>
#include "../logging/message_logging.h"
#include "../basic_types/VectorCallback.h"

using namespace uncover::logging;
using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace std;

namespace uncover {
	namespace analysis {

		PreparedGTS::PreparedGTS(GTS const& gts, RulePreparer_sp rulePrep, bool opt)
			: GTS(gts.getName(),
					unordered_map<IDType, Rule_sp>(),
					gts.getUQRules(),
					gts.getInitialGraphs()),
				oldStdRules(gts.getStdRules()),
				prepFrom(),
				rulePreparer(rulePrep),
				uqRuleInstances(),
				uqRuleLengths() {

			if(!this->rulePreparer) {
				logger.newError() << "PreparedRules object cannot be instantiated without a "
						"RulePreparer. Using empty rule sets for analysis!!!" << endLogMessage;
				return;
			}

			this->rulePreparer->setOptimization(opt);
			StdRuleMapCB callbackStdRules(*this);

			// each standard rule is prepared independently, i.e. the result set can contain isomorphic rules
			// if there are two old rules with identical left sides
			for(auto it = oldStdRules.cbegin(); it != oldStdRules.cend(); ++it) {
				callbackStdRules.setCurrentRuleID(it->first);
				rulePreparer->prepareRule(*it->second, callbackStdRules);
			}

			// universally quantified rules have to be prepared at runtime

			if(opt) {

				// TODO possible optimization: delete isomorphic morphisms

				// TODO possible optimization: delete all UQRules if all their instantiations are order morphisms

			}

		}

		PreparedGTS::~PreparedGTS() {}


		shared_ptr<vector<AnonRule_csp>> PreparedGTS::getOriginalRules(IDType id) const {

			shared_ptr<vector<AnonRule_csp>> res = make_shared<vector<AnonRule_csp>>();

			// if the ID cannot be found, then an empty collection is returned
			if(prepFrom.count(id) != 0) {
				for(auto innerIt = prepFrom.at(id).cbegin(); innerIt != prepFrom.at(id).cend(); ++innerIt) {
					if(oldStdRules.count(*innerIt)) {
						res->push_back(oldStdRules.at(*innerIt));
					} else {
						logger.newError() << "An original rule from which some other rule was supposedly prepared ";
						logger.contMsg() << "from, could not be found. We ignore this, but end results could be erroneous." << endLogMessage;
					}
				}
			}

			return res;
		}


		PreparedGTS::uqinst_iterator PreparedGTS::beginUQInstances(IDType const& ruleID, Hypergraph const& graph) {

			if(uqRules.count(ruleID) == 0) {
				throw out_of_range ("PreparedGTS does not know a UQRule with the given rule ID.");
			} else {

				size_t bound = graph.getVertexCount() + graph.getEdgeCount();
				return PreparedGTS::uqinst_iterator(uqRules[ruleID], rulePreparer, bound);

			}

		}

		void PreparedGTS::createInstancesUpTo(IDType const& ruleID, size_t bound) {

			vector<UQRuleInstance_sp>& instances = uqRuleInstances[ruleID];
			vector<size_t>& lengths = uqRuleLengths[ruleID];
			UQRule_csp rule = uqRules[ruleID];

			if(lengths.size() == 0) {
				instances.push_back(rule->createZeroInstance());
				lengths.push_back(1);
			}

			VectorCallback<UQRuleInstance_sp> callback(instances);

			// possibly multiple instantiations (concatenated) are necessary
			while(lengths.size() <= bound) {

				size_t index = (lengths.size() == 1) ? 0 : lengths[lengths.size()-2];
				size_t oldEnd = lengths.back();

				// for each rule instance with a number of instantiations of one less than current
				for(; index < oldEnd; ++index) {
					// for each quantification of the rule
					for(size_t quantCount = 0; quantCount < rule->getQuantCount(); ++quantCount) {

						// prepare the rule and insert all resulting rules (possibly using optimization)
						UQRuleInstance_sp newRule = rule->extendInstance(instances[index], quantCount);
						rulePreparer->prepareRule(*newRule, callback);
					}
				}

				lengths.push_back(instances.size());
			}
		}

		// **************************************************************************************
		// uqinst_iterator implementation
		// **************************************************************************************

		PreparedGTS::uqinst_iterator::uqinst_iterator(PreparedGTS::uqinst_iterator&& old)
			: rule(old.rule),
			  rulePreparer(old.rulePreparer),
			  currentQuant(old.currentQuant),
			  currentInstances(old.currentInstances),
			  bound(old.bound) {

			old.rule = nullptr;
			old.rulePreparer = nullptr;
			currentQuant = nullptr;
			currentInstances = nullptr;

		}

		PreparedGTS::uqinst_iterator::uqinst_iterator(UQRule_sp rule, RulePreparer_sp rulepreparer, unsigned int bound)
			: rule(rule),
			  rulePreparer(rulepreparer),
			  currentQuant(new vector<unsigned int>()),
			  currentInstances(new vector<UQRuleInstance_sp>()),
			  bound(bound) {

			for(size_t i = 0; i < rule->getQuantCount(); ++i) {
				currentQuant->push_back(0);
			}

			loadNextRule(true);
		}

		PreparedGTS::uqinst_iterator::~uqinst_iterator() {
			if(currentQuant) delete currentQuant;
			if(currentInstances) delete currentInstances;
		}

		void PreparedGTS::uqinst_iterator::loadNextRule(bool firstTime) {

			// if rules are still stored, take them
			if(currentInstances->size() > 1) {
				currentInstances->pop_back();
				return;
			}

			// search and generate the next quantification (terminate if found)
			vector<unsigned int>::iterator curIt = currentQuant->begin();
			while(curIt != currentQuant->end()) {

				// this check should only block incrementation once (i.e. when initializing the iterator)
				if(firstTime) {
					firstTime = false;
				} else {
					++(*curIt);
				}

				// if currentQuant is valid, then:
				// 1) generate UQRuleInstance for currentQuant
				// 2) prepare just generated UQRuleInstance
				// 3) if prepared set is non-empty, terminate, otherwise try next currentQuant
				if(sumCurrentQuant() <= bound) {
					VectorCallback<UQRuleInstance_sp> callback(*currentInstances);
					UQRuleInstance_sp currentInstance = rule->createInstance(*currentQuant);
					rulePreparer->prepareRule(*currentInstance, callback);
					if(currentInstances->size()) {
						return;
					} // else try next quantification possibility
				} else {
					*curIt = 0;
					++curIt;
				}

			}

			// there is no next element
			delete currentQuant;
			currentQuant = nullptr;
			delete currentInstances;
			currentInstances = nullptr;
		}

		unsigned int PreparedGTS::uqinst_iterator::sumCurrentQuant() {
			unsigned int result(0);
			for(vector<unsigned int>::const_iterator it = currentQuant->begin();
					it != currentQuant->end(); ++it) {
				result += *it;
			}
			return result;
		}

		PreparedGTS::uqinst_iterator& PreparedGTS::uqinst_iterator::operator++ () {
			loadNextRule();
			return *this;
		}

		UQRuleInstance_sp PreparedGTS::uqinst_iterator::operator*() {
			if(currentInstances) {
				return currentInstances->back();
			} else {
				return nullptr;
			}
		}

		bool PreparedGTS::uqinst_iterator::hasEnded() {
			return currentInstances == nullptr;
		}

	} /* namespace analysis */
} /* namespace uncover */
