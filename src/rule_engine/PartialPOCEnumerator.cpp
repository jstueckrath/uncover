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

#include "PartialPOCEnumerator.h"
#include "../logging/message_logging.h"

using namespace uncover::rule_engine;
using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::logging;

/*
 * Here I give an overview of how the partial pushout complements are enumerated. In
 * general the computation works as follows:
 * 1. split the rule in a partial and a total part
 * 2. enumerate all pushout complement for the total part (using TotalPOCEnumerator)
 * 3. modify each such pushout complement by adding elements deleted by the partial
 *    part of the rule, also merging them in all possible ways, if matching need not
 *    be injective
 *
 * A. Initialization
 * -- is performed when calling reset() or generating a new object of this enumerator
 * 1. split rule into a partial part p and total part t such that the concatenation
 *    --p--> --t--> results in the original rule
 * 2. initialize TotalPOCEnumerator with the given graph and total rule part (and
 *    the boolean specifying injective or conflict-free matching)
 * 3. initialize template variables (B)
 * 4. compute current enumeration based on the current template values (C)
 *
 * Steps 3 and 4 are only performed if the TotalPOCEnumerator can return at least one
 * pushout complement!
 *
 * B. Initializing template variables
 * -- is performed while initialization or whenever all possible modifications have
 *    been applied to the pushout complement returned by the TotalPOCEnumerator and
 *    the next total pushout complement is used as basis
 * 1. initialize the template graph with the total poc and the template mapping with the
 * 		concatenation of the partial rule part and the total poc match
 * 2. for every node and edge in the left side for which the partial rule is undefined,
 *    add a copy to the template graph, adjusting the template mapping accordingly
 * 3. if matches are set to be injective stop now
 * 4. initialize the IDPartitionEnumerator vertexMergings to enumerate all partitions on
 *    vertices added in step 2 and store all edges added in step 2 in the mergableEdges
 *    vector
 * 5. perform a vertex merging according to vertexMergings (initializing the current graph
 * 		mapping with the template graph and mapping) and initialize edgeMergings, a vector
 * 		of IDPartitionEnumerators, to enumerate all mergings of parallel edges
 *
 * If matches are injective, nothing need to be merged, making step 4 and 5 unnecessary.
 *
 * C. Computing the current graph and mapping
 * -- called by initialization or by the increment operator
 * (for injective matches)
 * 1. just use template graph and mapping (need not even copy)
 * (for conflict-free matches)
 * 1.  generate a copy of the template graph
 * 2. merge nodes according to vertexMergings; skip this step if the current graph was
 *    already computed to calculate possible edge mergings
 * 3. merge edges according to edgeMergings
 *
 * D. Increment operator
 * 1. if enumerator ended stop immediately
 * (for conflict-free matches only)
 * 2. if not all edge mergings where enumerated, get next edge merging and goto 5
 * 3. if not all vertex mergings where enumerated, get next vertex merging, initiate
 *    enumerator vector for edge mergings and goto 5
 * (for both matching types)
 * 4. let TotalPOCEnumerator compute next poc and terminate immediately if it ended, else
 * 5. generate templates (B) and compute current (C)
 *
 */

namespace uncover {
	namespace rule_engine {

		PartialPOCEnumerator::PartialPOCEnumerator(
				graphs::AnonRule_csp rule,
				graphs::Hypergraph_csp graph,
				bool inj,
				shared_ptr<vector<AnonRule_csp>> orRules)
			: partialRule(nullptr),
			  originalRules(nullptr),
			  injective(inj),
			  totalPOCs(nullptr),
			  currentGraph(nullptr),
			  currentMapp(nullptr),
			  templateGraph(nullptr),
			  templateMapp(nullptr),
			  vertexMergings(nullptr),
			  mergableEdges(nullptr),
			  edgeMergings(nullptr),
			  nodesAlreadyMerged(false) {

			this->reset(rule,graph,inj,orRules);

		}

		PartialPOCEnumerator::PartialPOCEnumerator(PartialPOCEnumerator&& oldEnum)
			: partialRule(oldEnum.partialRule),
			  injective(oldEnum.injective),
			  totalPOCs(oldEnum.totalPOCs),
			  currentGraph(oldEnum.currentGraph),
			  currentMapp(oldEnum.currentMapp),
			  templateGraph(oldEnum.templateGraph),
			  templateMapp(oldEnum.templateMapp),
			  vertexMergings(oldEnum.vertexMergings),
			  mergableEdges(oldEnum.mergableEdges),
			  edgeMergings(oldEnum.edgeMergings),
			  nodesAlreadyMerged(oldEnum.nodesAlreadyMerged) {

			oldEnum.partialRule = nullptr;
			oldEnum.totalPOCs = nullptr;
			oldEnum.currentGraph = nullptr;
			oldEnum.currentMapp = nullptr;
			oldEnum.templateGraph = nullptr;
			oldEnum.templateMapp = nullptr;
			oldEnum.vertexMergings = nullptr;
			oldEnum.mergableEdges = nullptr;
			oldEnum.edgeMergings = nullptr;

		}

		PartialPOCEnumerator::~PartialPOCEnumerator() {
			if(totalPOCs) delete totalPOCs;
			if(vertexMergings) delete vertexMergings;
			if(mergableEdges) delete mergableEdges;
			if(edgeMergings) delete edgeMergings;
		}

		bool PartialPOCEnumerator::ended() const {
			return currentGraph == nullptr;
		}

		void PartialPOCEnumerator::genTemplates() {

			// delete results of previous mergings if existent
			// other previous computations are deleted implicitly
			if(vertexMergings) delete vertexMergings;
			vertexMergings = nullptr;
			if(mergableEdges) delete mergableEdges;
			mergableEdges = nullptr;
			if(edgeMergings) delete edgeMergings;
			edgeMergings = nullptr;

			// we reuse the total POC as initial partial POC
			templateGraph = totalPOCs->getGraph();
			templateMapp = make_shared<Mapping>(partialRule->getMapping());
			templateMapp->concat(*totalPOCs->getMapping());

			auto leftGraph = partialRule->getLeft();
			Mapping const& partMapp = partialRule->getMapping();

			// add all vertices without image via the partial rule
			vector<IDType> delVertices;
			for(auto iter = leftGraph->cbeginVertices(); iter != leftGraph->cendVertices(); ++iter) {
				if(!partMapp.hasSrc(iter->first)) {
					delVertices.push_back(templateGraph->addVertex());
					(*templateMapp)[iter->first] = delVertices.back();
				} //else ignore
			}

			// add all edges without preimage via the partial rule
			mergableEdges = new vector<IDType>();
			for(auto iter = leftGraph->cbeginEdges(); iter != leftGraph->cendEdges(); ++iter) {
				if(!partMapp.hasSrc(iter->first)) {

					vector<IDType> newVertices;
					auto oldVertices = leftGraph->getVerticesOfEdge(iter->first);
					for(size_t i = 0; i < oldVertices->size(); ++i) {
						newVertices.push_back(templateMapp->getTar(oldVertices->at(i)));
					}
					mergableEdges->push_back(templateGraph->addEdge(iter->second.getLabel(),newVertices));
					(*templateMapp)[iter->first] = mergableEdges->back();

				}
			}

			// initialize vertex and edge mergings if applicable
			if(injective) {
				delete mergableEdges;
				mergableEdges = nullptr;
			} else {
				vertexMergings = new IDPartitionEnumerator(delVertices);
				prepEdgeMergings();
			}

		}

		void PartialPOCEnumerator::computeCurrent() {

			if(injective) {

				// just use template graph and mapping (there is only one POC)
				currentGraph = templateGraph;
				currentMapp = templateMapp;

			} else {

				// mergeNodes generates a currentGraph and currentMapp, then it merges vertices
				mergeNodes();
				mergeEdges();

			}

		}

		void PartialPOCEnumerator::mergeNodes() {

			// ignore the call of this method, since it was already called by prepEdgeMergings()
			if(nodesAlreadyMerged) {
				nodesAlreadyMerged = false;
				return;
			}

			currentGraph = make_shared<Hypergraph>(*templateGraph);
			currentMapp = make_shared<Mapping>(*templateMapp);

			IDPartition const& vMergings = **vertexMergings;
			for(auto iter = vMergings.cbegin(); iter != vMergings.cend(); ++iter) {
				if(iter->size() >= 2) {

					// merge the nodes
					IDType newID = currentGraph->mergeVertices(*iter);

					// update also the mapping
					for(auto mapIt = currentMapp->begin(); mapIt != currentMapp->end(); ++mapIt) {
						if(iter->count(mapIt->second)) {
							mapIt->second = newID;
						}
					}

				}
			}

		}

		void PartialPOCEnumerator::prepEdgeMergings() {

			// merge nodes to compute viable edge mergings
			mergeNodes();
			// next time computeCurrent() will not merge nodes, since they already are
			nodesAlreadyMerged = true;

			vector<vector<IDType>> partOnEdges;
			for(auto iter = mergableEdges->cbegin(); iter != mergableEdges->cend(); ++iter) {

				bool wasAdded = false;

				// search for edges with which the current edge can be merged
				for(auto partIt = partOnEdges.begin(); partIt != partOnEdges.end(); partIt++) {

					// check only with one representant
					if(currentGraph->areParallelEdges(*iter,partIt->back())) {
						partIt->push_back(*iter);
						wasAdded = true;
						break;
					}

				}

				// add ID in its own partition if no parallel edges where found
				if(!wasAdded) {
					partOnEdges.push_back(vector<IDType>({*iter}));
				}

			}

			// partOnEdges is now a partition on the deleted edges, where two edges are in the same set iff they can be merged

			// generate enumerators for edge merging
			if(edgeMergings) delete edgeMergings;
			edgeMergings = new vector<IDPartitionEnumerator>();
			for(auto iter = partOnEdges.cbegin(); iter != partOnEdges.cend(); ++iter) {
				if(iter->size() > 1) {
					edgeMergings->push_back(IDPartitionEnumerator(*iter));
				}
			}

		}

		void PartialPOCEnumerator::mergeEdges() {

			for(auto iter = edgeMergings->cbegin(); iter != edgeMergings->cend(); ++iter) {

				IDPartition const& idPart = **iter;
				for(auto partIt = idPart.cbegin(); partIt != idPart.cend(); ++partIt) {
					if(partIt->size() > 1) {

						// since all edges are parallel, merging is equal to deleting all but one edge
						auto innerIt = partIt->cbegin();
						IDType newID = *innerIt;
						++innerIt;

						// delete edges
						while(innerIt != partIt->cend()) {
							currentGraph->deleteEdge(*innerIt);
							++innerIt;
						}

						// update mapping
						for(auto mapIt = currentMapp->begin(); mapIt != currentMapp->end(); ++mapIt) {
							if(partIt->count(mapIt->second)) {
								mapIt->second = newID;
							}
						}

					}
				}

			}

		}

		void PartialPOCEnumerator::reset(AnonRule_csp rule,
				Hypergraph_csp graph,
				bool inj,
				shared_ptr<vector<AnonRule_csp>> orRules) {

			// delete all old content
			if(totalPOCs) delete totalPOCs;
			totalPOCs = nullptr;
			currentGraph = nullptr;
			currentMapp = nullptr;
			templateGraph = nullptr;
			templateMapp = nullptr;
			if(vertexMergings) delete vertexMergings;
			vertexMergings = nullptr;
			if(mergableEdges) delete mergableEdges;
			mergableEdges = nullptr;
			if(edgeMergings) delete edgeMergings;
			edgeMergings = nullptr;
			nodesAlreadyMerged = false;

			// set data unchanged
			injective = inj;
			originalRules = orRules;

			// split the rule in partial and total part and initialize TotalPOCEnumerator
			pair<AnonRule_sp, AnonRule_sp> splitRule(nullptr,nullptr);
			rule->fillSplitRule(splitRule);
			partialRule = splitRule.first;

			totalPOCs = new TotalPOCEnumerator(splitRule.second, graph, this->injective);

			// if there are no total POCs, this enumerator immediately ends
			if(!totalPOCs->ended()) {
				genTemplates();
				computeCurrent();

				// if the first match is not conflict-free, find another one
				if(!injective && !cfWrtOriginalRule()) {
					++(*this);
				}
			}

		}

		bool PartialPOCEnumerator::incEdgeMergings() {

			// then there was only one possible enumeration (no enumerators are generated for singletons)
			if(edgeMergings->size() == 0) {
				return false;
			}

			size_t index = 0;
			while(index < edgeMergings->size()) {
				IDPartitionEnumerator& enumRef = edgeMergings->at(index);
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

		PartialPOCEnumerator& PartialPOCEnumerator::operator++() {

			// if enumerator has ended, do nothing
			if(!this->ended()) {

				// compute next POC
				bool stillWork = getNext();
				// skip the POC if conflict-free rules are used and all original rules are not
				// conflict-free wrt. the current match
				while(!injective && stillWork && !cfWrtOriginalRule()) {
					stillWork = getNext();
				}

			}

			return *this;
		}

		bool PartialPOCEnumerator::getNext() {

			// if the POC in non-injective all deleted elements must be merged in all possible ways
			if(!injective) {

				// get next edge merging if possible, then get next node merging if possible
				if(incEdgeMergings()) {
					computeCurrent();
					return true;
				} else {
					++(*vertexMergings);
					if(!vertexMergings->ended()) {
						prepEdgeMergings();
						computeCurrent();
						return true;
					}
				}

			}

			// at this point the POCs are injective or no more mergings are possible

			++(*totalPOCs);
			if(totalPOCs->ended()) {

				// clear all data
				currentGraph = nullptr;
				currentMapp = nullptr;
				templateGraph = nullptr;
				templateMapp = nullptr;
				if(vertexMergings) delete vertexMergings;
				vertexMergings = nullptr;
				if(mergableEdges) delete mergableEdges;
				mergableEdges = nullptr;
				if(edgeMergings) delete edgeMergings;
				edgeMergings = nullptr;
				nodesAlreadyMerged = false;

				return false;

			} else {

				// compute new partial POC from new total POC (at least one exists)
				genTemplates();
				computeCurrent();
				return true;

			}

		}

		graphs::Mapping_sp PartialPOCEnumerator::getMapping() const {
			return currentMapp;
		}

		graphs::Hypergraph_sp PartialPOCEnumerator::getGraph() const {
			return currentGraph;
		}

		Mapping const& PartialPOCEnumerator::getCurrentCoMatch() const {
			if(this->ended()) {
				throw InvalidStateException("PartialPOCEnumerator cannot return the currently used match, "
						"since it has ended.");
			} else {
				return totalPOCs->getCurrentCoMatch();
			}
		}

		inline bool PartialPOCEnumerator::cfWrtOriginalRule() const {

			if(originalRules && originalRules->size() > 0) {

				// if the match is conflict-free wrt. at least one original rule, we are fine
				for(auto it = originalRules->cbegin(); it != originalRules->cend(); ++it) {
					if((*it)->isConflictFree(*currentMapp)) {
						return true;
					}
				}

				return false;
			}

			return true;
		}

	} /* namespace rule_engine */
} /* namespace uncover */
