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

#include "SubgraphRulePreparer.h"
#include "../basic_types/RunException.h"
#include "../analysis/rule_preparation_helper.h"
#include <math.h>
#include <sstream>

using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::analysis;
using namespace std;

#include "../logging/message_logging.h"
using namespace uncover::logging;

namespace uncover {
	namespace subgraphs {

		/**
		 * Serves as an implementation of the SubgraphRulePreparer::prepareRule(...) variants for normal and UQRules.
		 * @param rule a reference to the rule to be prepared
		 * @param callback a callback for storing newly generated rules
		 * @param rulePreparer the RulePreparer used for preparation
		 */
		template<class T>
		void prepareRule_Impl(T const& rule,
				CollectionCallback<shared_ptr<T>>& callback,
				RulePreparer const& rulePreparer) {

			auto rightSide = rule.getRight();

			size_t nodeCount = rightSide->getVertexCount();
			size_t edgeCount = rightSide->getEdgeCount();
			if(nodeCount > 31 || edgeCount > 31) {
				throw RunException("Cannot prepare rules with more than 31 nodes or edges on the right side. Any way this would generate at least 2^32 new rules (before optimization)!");
			}
			// each digit of this integer specifies if some node deleted (1) or not (0)
			// all mappings are enumerated by increasing this variable
			unsigned int nodeDefMap;
			unsigned int nodeDefMapMax = (unsigned int)pow(2,nodeCount);
			// maps digits of nodeDefMap to node IDs
			IDType nodeID[nodeCount];
			// defines an order on edges, for easier enumeration on edges
			IDType edgeID[edgeCount];
			// stores an int which represents the set (not multiset) of attached nodes
			unsigned int edgeAttachMap[edgeCount];

			{ // initializes the variables above
				unsigned int counter = 0;
				unordered_map<IDType, size_t> reverseNodeID;
				for(auto it = rightSide->cbeginVertices(); it != rightSide->cendVertices(); ++it, ++counter) {
					nodeID[counter] = it->first;
					reverseNodeID[it->first] = counter;
				}
				counter = 0;
				for(auto it = rightSide->cbeginEdges(); it != rightSide->cendEdges(); ++it, ++counter) {
					edgeID[counter] = it->first;
					vector<IDType> const* conNodes = rightSide->getVerticesOfEdge(it->first);
					edgeAttachMap[counter] = 0;
					for(vector<IDType>::const_iterator nIt = conNodes->cbegin(); nIt != conNodes->cend(); ++nIt) {
						edgeAttachMap[counter] = edgeAttachMap[counter] | (1 << reverseNodeID[(*nIt)]);
					}
				}
			}

			// actual enumeration of mappings
			unsigned int counter = 0;
			for(nodeDefMap = 0; nodeDefMap < nodeDefMapMax; ++nodeDefMap) {
				// generate a copy of the right graph
				auto newRight = make_shared<AnonHypergraph>(*rightSide);
				unordered_set<IDType> deletedElem;

				// delete all nodes of this iteration including their attached edges
				for(unsigned int i = 0; i < nodeCount; ++i) {
					if(nodeDefMap & (1 << i)) {
						deletedElem.insert(nodeID[i]);
						newRight->deleteVertex(nodeID[i],&deletedElem);
					}
				}

				// also delete mappings of deleted elements
				Mapping newMapping(rule.getMapping());
				newMapping.delTar(deletedElem);

				// find all non-deleted edges
				vector<IDType> remainingEdges;
				for(unsigned int i = 0; i < edgeCount; ++i) {
					if(deletedElem.count(edgeID[i]) == 0) {
						remainingEdges.push_back(edgeID[i]);
					}
				}

				// now enumerate all edge deletions of undeleted edges
				unsigned int edgeDefMap;
				unsigned int edgeDefMapMax = (unsigned int)pow(2,remainingEdges.size());
				stringstream converter;
				for(edgeDefMap = 0; edgeDefMap < edgeDefMapMax; ++edgeDefMap, ++counter) {

					// generate a full copy of the rule, including all changes to this point
					auto finalNewRight = make_shared<AnonHypergraph>(*newRight);
					converter << (rule.getName()) << "-v" << counter;
					shared_ptr<T> newRule = createRuleFrom(rule,
																								 converter.str(),
																								 make_shared<AnonHypergraph>(*rule.getLeft()),
																								 finalNewRight,
																								 newMapping);
					converter.str(string());
					converter.clear();
					Mapping& finalNewMapping = newRule->getMapping();

					// then apply further (individual) changes
					for(unsigned int i = 0; i < remainingEdges.size(); ++i) {
						if(edgeDefMap & (1 << i)) {
							finalNewRight->deleteEdge(remainingEdges.at(i));
							finalNewMapping.delTar(remainingEdges.at(i));
						}
					}

					// store resulting rule
					if(!rulePreparer.willOptimize() || !rulePreparer.isNoOpRule(*newRule)) {
						callback.insert(newRule);
					}
				}
			}
		}

		SubgraphRulePreparer::SubgraphRulePreparer() : RulePreparer() {}

		SubgraphRulePreparer::~SubgraphRulePreparer() {}

		void SubgraphRulePreparer::prepareRule(Rule const& rule, CollectionCallback<Rule_sp>& callback) const {
			prepareRule_Impl<Rule>(rule, callback, *this);
		}

		void SubgraphRulePreparer::prepareRule(UQRuleInstance const& rule, CollectionCallback<UQRuleInstance_sp>& callback) const {
			prepareRule_Impl<UQRuleInstance>(rule, callback, *this);
		}

		bool SubgraphRulePreparer::isNoOpRule(AnonRule const& rule) const {
			unordered_set<IDType> mappedTo;

			// check if mapping is injective
			Mapping const& mapping = rule.getMapping();
			for(Mapping::const_iterator mapIt = mapping.cbegin(); mapIt != mapping.cend(); ++mapIt) {
				if(mappedTo.count(mapIt->second) != 0) {
					return false;
				} else {
					mappedTo.insert(mapIt->second);
				}
			}

			// check if mapping is surjective
			auto graph = rule.getRight();
			for(auto vertexIt = graph->cbeginVertices(); vertexIt != graph->cendVertices(); ++vertexIt) {
				if(mappedTo.count(vertexIt->first) == 0) {
					return false;
				}
			}
			for(auto edgeIt = graph->cbeginEdges(); edgeIt != graph->cendEdges(); ++edgeIt) {
				if(mappedTo.count(edgeIt->first) == 0) {
					return false;
				}
			}

			// rule is a injective and surjective partial morphism
			return true;
		}

	} /* namespace subgraphs */
} /* namespace uncover */
