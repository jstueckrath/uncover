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

#include "UQRule.h"
#include <stdexcept>
#include "../logging/message_logging.h"
#include "../basic_types/standard_operators.h"

using namespace std;
using namespace uncover::logging;

namespace uncover {
	namespace graphs {

		UQRule::UQRule(Rule const& mainRule)
		: mainRule(mainRule),
		  quantifications(),
			leftUQVertices(),
			rightUQVertices() {}

		UQRule::UQRule(string name,
				AnonHypergraph_sp left,
				AnonHypergraph_sp right,
				Mapping const& match)
		:	mainRule(name, left, right, match),
		 	quantifications(),
		 	leftUQVertices(),
		 	rightUQVertices() {}

		UQRule::~UQRule() {}

		IDType UQRule::getID() const {
			return mainRule.getID();
		}

		string UQRule::getName() const {
			return mainRule.getName();
		}

		Rule& UQRule::getMainRule() {
			return mainRule;
		}

		Rule const& UQRule::getMainRule() const {
			return mainRule;
		}

		void UQRule::addQuantification(AnonRule const& rule, Mapping const& match) {
			addQuantification(pair<AnonRule,Mapping>(rule,match));
		}

		void UQRule::addQuantification(Quantification const& quantification) {

			// the validity of the given quantification is assumed here
			quantifications.push_back(quantification);

			auto lqGraph = quantification.first.getLeft();
			Mapping const & qMapping = quantification.second;

			for(auto iter = lqGraph->beginVertices(); iter != lqGraph->endVertices(); ++iter) {
				if(qMapping.hasSrc(iter->first)) {
					IDType target = qMapping.getTar(iter->first);
					leftUQVertices.insert(target);
					if(mainRule.getMapping().hasSrc(target)) {
						rightUQVertices.insert(mainRule.getMapping()[target]);
					}
				}
			}

		}

		size_t UQRule::getQuantCount() const {
			return quantifications.size();
		}

		vector<UQRule::Quantification>& UQRule::getQuantifications() {
			return quantifications;
		}

		vector<UQRule::Quantification> const& UQRule::getQuantifications() const {
			return quantifications;
		}

		UQRuleInstance_sp UQRule::createInstance(
				vector<unsigned int> const& quantities,
				vector<unordered_set<IDType>>* genLeft,
				vector<unordered_set<IDType>>* genRight) const {

			if(quantities.size() != quantifications.size()) {
				logger.newError() << "Failed to create UQRuleInstance, number of "
						"quantities does not match quantification count. Will return null." << endLogMessage;
				return nullptr;
			}

			UQRuleInstance_sp result = createZeroInstance();

			// clear and initialize storages for generated IDs, if given
			if(genLeft) {
				genLeft->clear();
				for(size_t i = 0; i < quantifications.size(); ++i) {
					genLeft->push_back(unordered_set<IDType>());
				}
			}
			if(genRight) {
				genRight->clear();
				for(size_t i = 0; i < quantifications.size(); ++i) {
					genRight->push_back(unordered_set<IDType>());
				}
			}

			// for every universal quantification
			for(size_t i = 0; i < quantities.size(); ++i) {
				for(unsigned int j = 0; j < quantities.at(i); ++j) {
					extendRuleByUQ(*result, i, genLeft, genRight);
				}
			}

			return result;
		}

		UQRuleInstance_sp UQRule::createZeroInstance() const {
			return make_shared<UQRuleInstance>(mainRule, leftUQVertices, rightUQVertices);
		}

		UQRuleInstance_sp UQRule::createOneInstance(
				vector<unordered_set<IDType>>* genLeft,
				vector<unordered_set<IDType>>* genRight) const {

			vector<unsigned int> listOfOnes;
			for(size_t i = 0; i < quantifications.size(); ++i) {
				listOfOnes.push_back(1);
			}

			return createInstance(listOfOnes, genLeft, genRight);

		}

		UQRuleInstance_sp UQRule::extendInstance(
				UQRuleInstance_csp rule,
				size_t quantID,
				vector<unordered_set<IDType>>* genLeft,
				vector<unordered_set<IDType>>* genRight) const {

			if(quantID >= quantifications.size()) {
				throw out_of_range("The given index is larger (or as large as) the number of quantifications of this UQRule.");
			} else {

				// clear and initialize storages for generated IDs, if given
				if(genLeft) {
					genLeft->clear();
					for(size_t i = 0; i < quantifications.size(); ++i) {
						genLeft->push_back(unordered_set<IDType>());
					}
				}
				if(genRight) {
					genRight->clear();
					for(size_t i = 0; i < quantifications.size(); ++i) {
						genRight->push_back(unordered_set<IDType>());
					}
				}

				UQRuleInstance_sp newRule = make_shared<UQRuleInstance>(*rule);
				extendRuleByUQ(*newRule, quantID, genLeft, genRight);
				return newRule;

			}

		}

		void UQRule::extendRuleByUQ(
				UQRuleInstance& rule,
				size_t quantID,
				vector<unordered_set<IDType>>* genLeft,
				vector<unordered_set<IDType>>* genRight) const {

			Quantification const& quantification = quantifications.at(quantID);

			// short cut variables
			auto qLeft = quantification.first.getLeft();
			auto qRight = quantification.first.getRight();
			Mapping const& qMap = quantification.first.getMapping();
			Mapping const& interMap = quantification.second;
			auto rLeft = rule.getLeft();
			auto rRight = rule.getRight();
			Mapping& rMap = rule.getMapping();

			// maps right side of quantification to right side of rule
			Mapping r2r;
			Mapping::Inversion invmap;
			quantification.first.getMapping().getInverseMatch(invmap);

			// step 1: extend right side of rule by vertices
			for(auto iter = qRight->cbeginVertices(); iter != qRight->cendVertices(); ++iter) {

				if(invmap.count(iter->first) == 0 || invmap[iter->first].size() != 1) {
					r2r[iter->first] = rRight->addVertex();
					if(genRight) genRight->at(quantID).insert(r2r[iter->first]);
				} else { // iter->first has exactly one preimage

					IDType preimage = *(invmap[iter->first].begin());
					if(!interMap.hasSrc(preimage)) {
						r2r[iter->first] = rRight->addVertex();
						if(genRight) genRight->at(quantID).insert(r2r[iter->first]);
					} else if(rMap.hasSrc(interMap.getTar(preimage))) {
						r2r[iter->first] = rMap.getTar(interMap.getTar(preimage));
					} // else iter->first has no image
				}
			}

			// step 2: extend right side of rule by edges, but only if all attached vertices are defined
			for(auto iter = qRight->cbeginEdges(); iter != qRight->cendEdges(); ++iter) {

				if(invmap.count(iter->first) == 0 || invmap[iter->first].size() != 1) {
					vector<IDType> vertices;
					if(checkVertices(qRight, iter->first, r2r, vertices)) {
						r2r[iter->first] = rRight->addEdge(iter->second.getLabel(),vertices);
						if(genRight) genRight->at(quantID).insert(r2r[iter->first]);
					}
				} else { // iter->first has exactly one preimage

					IDType preimage = *invmap[iter->first].begin();
					if(!interMap.hasSrc(preimage)) {
						vector<IDType> vertices;
						if(checkVertices(qRight, iter->first, r2r, vertices)) {
							r2r[iter->first] = rRight->addEdge(iter->second.getLabel(),vertices);
							if(genRight) genRight->at(quantID).insert(r2r[iter->first]);
						}
					} else if(rMap.hasSrc(interMap.getTar(preimage))) {
						r2r[iter->first] = rMap.getTar(interMap.getTar(preimage));
					} // else iter->first has no image
				}
			}

			// maps left side of quantification to left side of rule
			Mapping l2l(interMap);

			// step 3: extend left side and mapping of rule by vertices
			for(auto iter = qLeft->cbeginVertices(); iter != qLeft->cendVertices(); ++iter) {

				if(!interMap.hasSrc(iter->first)) {
					l2l[iter->first] = rLeft->addVertex();
					if(genLeft) genLeft->at(quantID).insert(l2l[iter->first]);
					if(qMap.hasSrc(iter->first) && r2r.hasSrc(qMap.getTar(iter->first))) {
						rMap[l2l[iter->first]] = r2r.getTar(qMap.getTar(iter->first));
					}
				} // else nothing has to be done

			}

			// step 4: extend left side and mapping of rule by edges, but only if all attached vertices are defined
			for(auto iter = qLeft->cbeginEdges(); iter != qLeft->cendEdges(); ++iter) {

				if(!interMap.hasSrc(iter->first)) {

					// the following check will always be true
					vector<IDType> vertices;
					if(checkVertices(qLeft, iter->first, l2l, vertices)) {
						l2l[iter->first] = rLeft->addEdge(iter->second.getLabel(),vertices);
						if(genLeft) genLeft->at(quantID).insert(l2l[iter->first]);
					}

					if(qMap.hasSrc(iter->first) && r2r.hasSrc(qMap.getTar(iter->first))) {
						rMap[l2l[iter->first]] = r2r.getTar(qMap.getTar(iter->first));
					}
				} // else nothing has to be done

			}

		}

		bool UQRule::checkVertices(
				AnonHypergraph_csp graph,
				IDType const& edgeID,
				Mapping const& vmap,
				vector<IDType>& vertices) const {
			vector<IDType> const* attachedV = graph->getVerticesOfEdge(edgeID);
			vertices.clear();
			for(auto it = attachedV->cbegin(); it != attachedV->cend(); ++it) {
				if(vmap.hasSrc(*it)) {
					vertices.push_back(vmap.getTar(*it));
				} else {
					vertices.clear();
					return false;
				}
			}
			return true;
		}

		void UQRule::streamTo(std::ostream& ost) const {
			ost << "UQRule(mainRule=" << mainRule << ",\n";
			ost << "left quantified vertices=" << leftUQVertices << ",\n";
			ost << "right quantified vertices=" << rightUQVertices << ",\n";
			ost << "number of quantifications=" << quantifications.size();
			if(quantifications.size() == 0) {
				ost << ")";
				return;
			}
			ost << ",\n";
			for(size_t i = 0; i < quantifications.size(); ++i) {
				ost << "quantification " << i << "=" << quantifications.at(i);
				if(i != quantifications.size() - 1) {
					ost << ",\n";
				}
			}
			ost << ")";
		}

	} /* namespace graphs */
} /* namespace uncover */
