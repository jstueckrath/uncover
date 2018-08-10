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

#include "TotalPOCEnumerator.h"
#include "../basic_types/InvalidStateException.h"
#include "../logging/message_logging.h"

using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::logging;

/*
 * In the following I give an overview on how this enumerator computes pushout complements. The necessary
 * theory is described in detail in:
 * Marvin Heumüller, Salil Joshi, Barbara König, and Jan Stückrath. Construction of pushout complements in
 * the category of hypergraphs. In Proc. of GCM '10
 *
 * A. Initialization
 * -- is performed by the constructor and the reset method
 * 1. compute all co-matches of the right hand-side into the given graph.
 * 2. discard all co-matches until one is found for which isBackApplicable() states true
 * 3. if there is none, this enumerator ends immediately
 * 4. if there is one, compute the first pushout complement (B)
 *
 * B1. Pushout complement computation if the rule is injective
 * 1. copy the given graph and delete all elements of which the preimage in the right hand-side has no
 *    preimage in the left hand-side
 *
 * There is only one pushout complement in this case.
 *
 * B2. Pushout complement computation if rule in non-injective and co-matches are injective
 * 1. copy the given graph deleting element of which the preimage in the right hand-side has no preimage
 *    in the left hand-side, but do not copy edges without preimage under the current co-match
 * 2. add the edges left out in step 1, but attach them to new, fresh nodes
 * 3. merge these nodes according to the current content of vertexEnumsInj
 *
 * B3. Pushout complement computation if rule in non-injective and co-matches are conflict-free
 * 1. copy the given graph deleting element of which the preimage in the right hand-side has no preimage
 *    in the left hand-side,
 * 2. merge edges according to edgeEnums
 * 3. merge vertices according to vertexEnums
 *
 * C1. Increment operator if the rule is injective
 * 1. delete co-matches until one is found for which the rule is applicable
 * 2. use new co-match to compute pushout complement or end if all co-matches where used
 *
 * C2. Increment operator if rule is non-injective and co-matches are injective
 * 1. increment vertexEnumsInj to obtain next vertex merging and go to step 3 if successful
 * 2. if vertexEnumsInj was fully enumerated, delete co-matches until one is found for which the rule
 *    is applicable
 * 3. compute pushout complement or end if the last remaining co-match was deleted in step 2
 *
 * C3. Increment operator if rule is non-injective and co-matches are conflict-free
 * 1. increment vertexEnums to obtain next node merging and goto 4 if successful
 * 2. if all node mergings where enumerated, increment edgeEnums and if successful
 *  2a. initialize vertexEnums resulting from current edgeEnum
 *  2b. get first vertexEnum and goto 4
 * 3. if all edge mergings where enumerated, delete co-matches until one is found for which the rule
 *    is applicable
 * 4. compute pushout complement or end if the last remaining co-match was deleted in step 3
 *
 */

namespace uncover {
	namespace rule_engine {

		TotalPOCEnumerator::TotalPOCEnumerator(
				AnonRule_csp rule,
				Hypergraph_csp graph,
				bool injective)
		: rule(rule),
		  graph(graph),
		  injective(injective),
		  injectiveRule(rule->isInjective()),
		  matcher(new StdMatchFinder()),
		  matchList(nullptr),
		  currGraph(nullptr),
		  currMapp(nullptr),
		  edgeEnums(nullptr),
		  vertexEnums(nullptr),
		  templateGraphOr(nullptr),
		  templateMappOr(nullptr),
		  templateGraphME(nullptr),
		  templateMappME(nullptr),
		  mappPOCtoGraph(nullptr),
		  vertexEnumsInj(nullptr) {

			this->reset();

		}

		TotalPOCEnumerator::TotalPOCEnumerator(TotalPOCEnumerator&& oldEnum)
			: rule(oldEnum.rule),
			  graph(oldEnum.graph),
			  injective(oldEnum.injective),
			  injectiveRule(oldEnum.injectiveRule),
			  matcher(oldEnum.matcher),
			  matchList(oldEnum.matchList),
			  currGraph(oldEnum.currGraph),
			  currMapp(oldEnum.currMapp),
			  edgeEnums(oldEnum.edgeEnums),
			  vertexEnums(oldEnum.vertexEnums),
			  templateGraphOr(oldEnum.templateGraphOr),
			  templateMappOr(oldEnum.templateMappOr),
			  templateGraphME(oldEnum.templateGraphME),
			  templateMappME(oldEnum.templateMappME),
			  mappPOCtoGraph(oldEnum.mappPOCtoGraph),
			  vertexEnumsInj(oldEnum.vertexEnumsInj) {

			oldEnum.rule = nullptr;
			oldEnum.graph = nullptr;
			oldEnum.matcher = nullptr;
			oldEnum.matchList = nullptr;
			oldEnum.currGraph = nullptr;
			oldEnum.currMapp = nullptr;
			oldEnum.edgeEnums = nullptr;
			oldEnum.vertexEnums = nullptr;
			oldEnum.templateGraphOr = nullptr;
			oldEnum.templateMappOr = nullptr;
			oldEnum.templateGraphME = nullptr;
			oldEnum.templateMappME = nullptr;
			oldEnum.mappPOCtoGraph = nullptr;
			oldEnum.vertexEnumsInj = nullptr;

		}

		TotalPOCEnumerator::~TotalPOCEnumerator() {

			if(matcher) delete matcher;
			if(edgeEnums) delete edgeEnums;
			if(vertexEnums) delete vertexEnums;
			if(templateGraphOr) delete templateGraphOr;
			if(templateMappOr) delete templateMappOr;
			if(templateGraphME) delete templateGraphME;
			if(templateMappME) delete templateMappME;
			if(mappPOCtoGraph) delete mappPOCtoGraph;
			if(vertexEnumsInj) delete vertexEnumsInj;

		}

		Mapping const& TotalPOCEnumerator::getCurrentCoMatch() const {
			if(this->ended()) {
				throw InvalidStateException("TotalPOCEnumerator cannot return the currently used match, "
						"since it has ended.");
			} else {
				return matchList->back();
			}
		}

		void TotalPOCEnumerator::reset() {

			// delete old computations
			deleteTemporaryFields();

			// compute matchings
			matcher->setInjectivity(injective);
			matchList = matcher->findMatches(*rule->getRight(), *graph);

			// if at least one applicable match exists, compute the first POC
			while((matchList->size() > 0) && (!rule->isBackApplicable(*graph, matchList->back()))) {
				matchList->pop_back();
			}

			if(matchList->size() > 0) {
				if(injectiveRule) {
					computePOCInjRule();
				} else {
					if(injective) {
						initTemplatesInj();
						computePOCInj();
					} else {
						initEdgeTemplates();
						initVertexTemplates();
						computePOC();
					}
				}
			}

		}

		bool TotalPOCEnumerator::ended() const {
			return matchList->size() == 0;
		}

		bool TotalPOCEnumerator::chooseNextMatch() {
			if(matchList->size() == 0) {
				return false;
			}
			matchList->pop_back();
			while((matchList->size() > 0) && (!rule->isBackApplicable(*graph, matchList->back()))) {
				matchList->pop_back();
			}
			return matchList->size() > 0;
		}

		TotalPOCEnumerator& TotalPOCEnumerator::operator++() {

			if(matchList->size() == 0) {

				// ignore operator call if enumerator ended

			} else if(injectiveRule) {

				if(chooseNextMatch()) {
					computePOCInjRule();
				}

			} else {

				if(injective) {

					// use next node merging if possible, otherwise use next match
					if(incEnums(*vertexEnumsInj)) {
						// do nothing
					} else {

						deleteTemporaryFields();

						if(chooseNextMatch()) {
							initTemplatesInj();
						} else {
							return *this;
						}

					}

					computePOCInj();

				} else {

					// compute next vertex merging if possible, then next edge merging if possible, then next match
					if(incEnums(*vertexEnums)) {
						// do nothing
					} else if (incEnums(*edgeEnums)) {
						// note: at least one vertex merging exists for each edge merging
						initVertexTemplates();
					} else {

						// delete all fields which are used for POC enumeration for a single match
						deleteTemporaryFields();

						// initialize fields for new match if one was found
						if(chooseNextMatch()) {
							initEdgeTemplates();
							initVertexTemplates();
						} else {
							return *this;
						}

					}

					// delete old current and generate new one
					computePOC();

				}

			}

			return *this;
		}

		void TotalPOCEnumerator::initEdgeTemplates() {

			// generate templateGraphOr as L + D~ (optimized) and the mapping mappPOCtoGraph
			generateNonMergedPOC();

			// will contain one empty partition for each equivalence class (of edges) of \equiv_{g'}
			vector<IDPartition> partList;
			// stores the set of IDs used for each enumerator
			vector<unordered_set<IDType>> idsOfPartList;

			Mapping::Inversion invTmpMapp;
			mappPOCtoGraph->getInverseMatch(invTmpMapp);
			for(auto it = invTmpMapp.cbegin(); it != invTmpMapp.cend(); ++it) {
				if(graph->hasEdge(it->first) && it->second.size() > 1) {
					idsOfPartList.push_back(it->second);
					partList.push_back(IDPartition());
				} // else the equivalence class can be ignored, since there will be always exactly one possible merging
			}

			// fill partition list with sets of equivalence classes of \equiv_f;
			// each set contains all equivalence classes which are subsets of the same class of \equiv_{g'}
			Mapping::Inversion invRule;
			rule->getMapping().getInverseMatch(invRule);
			for(auto it = invRule.cbegin(); it != invRule.cend(); ++it) {
				if(rule->getRight()->hasEdge(it->first) && it->second.size() > 1) {

					IDType representant = *it->second.begin();
					for(size_t i = 0; i < idsOfPartList.size(); ++i) {
						if(idsOfPartList.at(i).count(representant)) {
							partList.at(i).insert(it->second);
							break; // the representant cannot be in any other partition
						}
					}

				} // classes with only one element have no effect, thus can be dropped
			}

			// fill enumerator list
			if(edgeEnums) delete edgeEnums;
			edgeEnums = new vector<IDClosureEnumerator>();
			for(size_t i = 0; i < idsOfPartList.size(); ++i) {
				edgeEnums->push_back(IDClosureEnumerator(idsOfPartList.at(i),partList.at(i)));
			}

		}

		void TotalPOCEnumerator::initVertexTemplates() {

			// generate copies of templateOr fields
			if(templateGraphME) delete templateGraphME;
			templateGraphME = new Hypergraph(*templateGraphOr);

			if(templateMappME) delete templateMappME;
			templateMappME = new Mapping(*templateMappOr);

			// perform edge merging and implied node mergings
			Mapping tmpMapp(*mappPOCtoGraph);

			for(vector<IDClosureEnumerator>::const_iterator it = edgeEnums->cbegin(); it != edgeEnums->cend(); ++it) {

				IDPartition const& tmpPart = **it;
				for(IDPartition::const_iterator partIt = tmpPart.cbegin(); partIt != tmpPart.cend(); ++partIt) {

					// no need to merge sets smaller than 2
					if(partIt->size() < 2) continue;

					// merge edges
					IDType newID = templateGraphME->mergeEdges(*partIt);

					// adjust mappings (from POC to the matched graph) of edges
					IDType newIDMap = mappPOCtoGraph->getTar(*partIt->begin());
					tmpMapp.delSrc(*partIt);
					tmpMapp[newID] = newIDMap;

					vector<IDType> const* newNodeIDs = templateGraphME->getVerticesOfEdge(newID);

					// adjust other mappings
					for(unordered_set<IDType>::const_iterator innerIt = partIt->cbegin(); innerIt != partIt->cend(); ++innerIt) {

						// adjust mappings (from L to POC) for edges
						// only if the element has a preimage in the left side, we have to adjust the mapping
						// note: templateMappOr is the identity
						if(templateMappOr->hasSrc(*innerIt)) {
							(*templateMappME)[*innerIt] = newID;
						}

						// do the same for attached nodes
						vector<IDType> const* innerItNodes = templateGraphOr->getVerticesOfEdge(*innerIt);
						for(size_t i = 0; i < innerItNodes->size(); ++i) {

							// adjust mappings (from L to POC) for nodes attached to merges edges
							if(templateMappOr->hasSrc(innerItNodes->at(i))) {
								(*templateMappME)[innerItNodes->at(i)] = newNodeIDs->at(i);
							}

							// adjust mappings (from POC to the matched graph) for nodes attached to the merges edges
							IDType newNodeIDMap = mappPOCtoGraph->getTar(innerItNodes->at(i));
							tmpMapp.delSrc(innerItNodes->at(i));
							tmpMapp[newNodeIDs->at(i)] = newNodeIDMap;

						}

					}

				}

			}

			// will contain one empty partition for each equivalence class (of nodes) of \equiv_{g'}
			vector<IDPartition> partList;
			// stores the set of IDs used for each enumerator
			vector<unordered_set<IDType>> idsOfPartList;

			Mapping::Inversion invTmpMapp;
			tmpMapp.getInverseMatch(invTmpMapp);
			for(auto it = invTmpMapp.cbegin(); it != invTmpMapp.cend(); ++it) {
				if(graph->hasVertex(it->first) && it->second.size() > 1) {
					idsOfPartList.push_back(it->second);
					partList.push_back(IDPartition());
				} // else the equivalence class can be ignored, since there will be always exactly one possible merging
			}

			// fill partition list with sets of equivalence classes of \equiv_f;
			// each set contains all equivalence classes which are subsets of the same class of \equiv_{g'}
			Mapping::Inversion invRule;
			rule->getMapping().getInverseMatch(invRule);
			for(auto it = invRule.cbegin(); it != invRule.cend(); ++it) {
				if(rule->getRight()->hasVertex(it->first)) {

					// nodes have to be mapped to POC first, since templateMappME may be non-injective
					unordered_set<IDType> newEquivClass;
					for(unordered_set<IDType>::const_iterator innerIt = it->second.cbegin(); innerIt != it->second.cend(); ++innerIt) {
						newEquivClass.insert(templateMappME->getTar(*innerIt));
					}

					// no entries for equivalence classes with only one element
					if(newEquivClass.size() <= 1) continue;

					IDType representant = *newEquivClass.begin();
					for(size_t i = 0; i < idsOfPartList.size(); ++i) {
						if(idsOfPartList.at(i).count(representant)) {
							partList.at(i).insert(newEquivClass);
							break; // the representant cannot be in any other partition
						}
					}

				} // classes with only one element have no effect, thus can be dropped
			}

			// fill enumerator list
			if(vertexEnums) delete vertexEnums;
			vertexEnums = new vector<IDClosureEnumerator>();
			for(size_t i = 0; i < idsOfPartList.size(); ++i) {
				vertexEnums->push_back(IDClosureEnumerator(idsOfPartList.at(i),partList.at(i)));
			}

		}

		void TotalPOCEnumerator::initTemplatesInj() {

			// generate templateGraphOr as L + D~ (optimized) and the mappings mappPOCtoGraph, templateMappOr
			generateNonMergedPOC();

			if(vertexEnumsInj) delete vertexEnumsInj;
			vertexEnumsInj = new vector<IDtoIDEnumerator>();

			Mapping::Inversion invPOC2Graph;
			mappPOCtoGraph->getInverseMatch(invPOC2Graph);
			for(auto it = invPOC2Graph.cbegin(); it != invPOC2Graph.cend(); ++it) {

				// ignore edges and injective parts of matchings
				if(graph->hasVertex(it->first) && it->second.size() > 1) {

					// split the found set into:
					unordered_set<IDType> withPre;
					unordered_set<IDType> withoutPre;
					for(auto innerIt = it->second.cbegin(); innerIt != it->second.cend(); ++innerIt) {
						// note: templateMappOr is currently the identity
						if(templateMappOr->hasSrc(*innerIt)) {
							withPre.insert(*innerIt);
						} else {
							withoutPre.insert(*innerIt);
						}
					}

					if(withPre.size() == 1) {
						// merge everything immediately, there is only one possibility
						IDType mergeIntoID = *withPre.cbegin();
						for(auto innerIt = withoutPre.cbegin(); innerIt != withoutPre.cend(); ++innerIt) {
							templateGraphOr->mergeVertexIntoFirst(mergeIntoID, *innerIt);
						}
					} else if(withoutPre.size() > 0) {

						// we know: there are more than one elements in withPre and withoutPre
						for(auto innerIt = withoutPre.cbegin(); innerIt != withoutPre.cend(); ++innerIt) {
							vertexEnumsInj->push_back(IDtoIDEnumerator(*innerIt,withPre));
						}

					}

				}

			}

		}

		void TotalPOCEnumerator::computePOCInj() {

			currGraph = make_shared<Hypergraph>(*templateGraphOr);
			currMapp = make_shared<Mapping>(*templateMappOr);

			// for each tentacle of each edge without preimage in the right side:
			// attach the tentacle to a node in the POC which will be mapped to the right node in the mapped graph
			for(auto it = vertexEnumsInj->cbegin(); it != vertexEnumsInj->cend(); ++it) {
				currGraph->mergeVertexIntoFirst(it->getTarget(),it->getSource());
			}

		}

		void TotalPOCEnumerator::computePOC() {

			currGraph = make_shared<Hypergraph>(*templateGraphME);
			currMapp = make_shared<Mapping>(*templateMappME);

			// for each partition in each equivalence
			for(auto it = vertexEnums->cbegin(); it != vertexEnums->cend(); ++it) {

				IDPartition const& tmpPart = **it;
				for(auto partIt = tmpPart.cbegin(); partIt != tmpPart.cend(); ++partIt) {

					// no need to merge sets smaller than 2
					if(partIt->size() < 2) continue;

					// merge vertices and adjust mapping
					IDType newID = currGraph->mergeVertices(*partIt);
					for(auto innerIt = partIt->cbegin(); innerIt != partIt->cend(); ++innerIt) {
						// only if the element has a preimage in the left side, we have to adjust the mapping
						if(templateMappME->hasTar(*innerIt)) {
							(*currMapp)[*innerIt] = newID;
						}
					}

				}

			}
		}

		bool TotalPOCEnumerator::incEnums(vector<IDClosureEnumerator>& enums) {

			// then there was only one possible enumeration (no enumerators are generated for singletons)
			if(enums.size() == 0) {
				return false;
			}

			size_t index = 0;
			while(index < enums.size()) {
				IDClosureEnumerator& enumRef = enums.at(index);
				++enumRef;
				if(enumRef.ended()) {
					enumRef.reset();
					++index;
				} else {
					return true;
				}
			}

			return false;
		}

		bool TotalPOCEnumerator::incEnums(vector<IDtoIDEnumerator>& enums) {

			// then there was only one possible enumeration (no enumerators are generated for singletons)
			if(enums.size() == 0) {
				return false;
			}

			size_t index = 0;
			while(index < enums.size()) {
				IDtoIDEnumerator& enumRef = enums.at(index);
				++enumRef;
				if(enumRef.ended()) {
					enumRef.reset();
					++index;
				} else {
					return true;
				}
			}

			return false;
		}

		Mapping_sp TotalPOCEnumerator::getMapping() const {
			return currMapp;
		}

		Hypergraph_sp TotalPOCEnumerator::getGraph() const {
			return currGraph;
		}

		void TotalPOCEnumerator::generateNonMergedPOC() {

			// generate a copy of the left side L and the identity as mapping
			if(templateGraphOr) delete templateGraphOr;
			templateGraphOr = new Hypergraph(*rule->getLeft());

			if(templateMappOr) delete templateMappOr;
			templateMappOr = new Mapping();

			for(auto it = templateGraphOr->cbeginVertices(); it != templateGraphOr->cendVertices(); ++it) {
				(*templateMappOr)[it->first] = it->first;
			}

			for(auto it = templateGraphOr->cbeginEdges(); it != templateGraphOr->cendEdges(); ++it) {
				(*templateMappOr)[it->first] = it->first;
			}

			// will later store a full mapping from templateGraphOr to the matched graph
			if(mappPOCtoGraph) delete mappPOCtoGraph;
			mappPOCtoGraph = new Mapping(rule->getMapping());
			// is guaranteed to be total, since inputs are total
			mappPOCtoGraph->concat(matchList->back());

			Mapping::Inversion invMatch;
			matchList->back().getInverseMatch(invMatch);

			// add a node to the POC for each node of the matched graph without preimage
			Mapping nodesNoPre;
			for(auto it = graph->cbeginVertices(); it != graph->cendVertices(); ++it) {
				if(!invMatch.count(it->first)) {
					IDType newID = templateGraphOr->addVertex();
					nodesNoPre[it->first] = newID;
					(*mappPOCtoGraph)[newID] = it->first;
				}
			}

			// add an edge to the POC for each edge of the matched graph without preimage
			// also add new nodes for each attached node, except they were already added above (optimization)
			for(auto it = graph->cbeginEdges(); it != graph->cendEdges(); ++it) {
				if(!invMatch.count(it->first)) {
					vector<IDType> const* oldVertices = graph->getVerticesOfEdge(it->first);

					vector<IDType> newVertices;
					for(auto vIt = oldVertices->cbegin(); vIt != oldVertices->cend(); ++vIt) {
						if(nodesNoPre.hasSrc(*vIt)) {
							newVertices.push_back(nodesNoPre[*vIt]);
						} else {
							IDType newID = templateGraphOr->addVertex();
							(*mappPOCtoGraph)[newID] = *vIt;
							newVertices.push_back(newID);
						}
					}

					templateGraphOr->addEdge(it->second.getLabel(),newVertices);
				}
			}

		}

		void TotalPOCEnumerator::deleteTemporaryFields() {

			currGraph = nullptr;
			currMapp = nullptr;

			if(edgeEnums) {
				delete edgeEnums;
				edgeEnums = nullptr;
			}

			if(vertexEnums) {
				delete vertexEnums;
				vertexEnums = nullptr;
			}

			if(templateGraphOr == templateGraphME) {
				templateGraphME = nullptr;
				if(templateGraphOr) delete templateGraphOr;
				templateGraphOr = nullptr;
			} else {
				if(templateGraphOr) {
					delete templateGraphOr;
					templateGraphOr = nullptr;
				}
				if(templateGraphME) {
					delete templateGraphME;
					templateGraphME = nullptr;
				}
			}

			if(templateMappOr == templateMappME) {
				templateMappME = nullptr;
				if(templateMappOr) delete templateMappOr;
				templateMappOr = nullptr;
			} else {
				if(templateMappOr) {
					delete templateMappOr;
					templateMappOr = nullptr;
				}
				if(templateMappME) {
					delete templateMappME;
					templateMappME = nullptr;
				}
			}

			if(mappPOCtoGraph) {
				delete mappPOCtoGraph;
				mappPOCtoGraph = nullptr;
			}

			if(vertexEnumsInj) {
				delete vertexEnumsInj;
				vertexEnumsInj = nullptr;
			}

		}

		void TotalPOCEnumerator::computePOCInjRule() {

			currGraph = make_shared<Hypergraph>(*graph);
			currMapp = make_shared<Mapping>(rule->getMapping());
			currMapp->concat(matchList->back());

			Mapping::Inversion invRule;
			rule->getMapping().getInverseMatch(invRule);
			Mapping::Inversion invMatch;
			matchList->back().getInverseMatch(invMatch);

			// delete every element in the copied graph which as a preimage in the right side but none in the left side
			// note: it was already checked that the pushout complement exists (which is used here)
			for(auto it = invMatch.cbegin(); it != invMatch.cend(); ++it) {

				// according to the gluing condition all preimages in R have preimages in L iff one has
				IDType preimage = *it->second.begin();
				if(!invRule.count(preimage)) {
					if(graph->hasVertex(it->first)) {
						currGraph->deleteVertex(it->first);
					} else {
						currGraph->deleteEdge(it->first);
					}
				}
			}

		}

	} /* namespace rule_engine */
} /* namespace uncover */
