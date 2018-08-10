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

#include "StdMatchFinder.h"
#include "../basic_types/RunException.h"
#include "../logging/message_logging.h"
#include "ExpEnumerator.h"

using namespace uncover::graphs;
using namespace uncover::basic_types;
using namespace uncover::logging;

namespace uncover {
	namespace rule_engine {

		StdMatchFinder::StdMatchFinder(bool injective, bool stopFast) : matchInjective(injective), stopOnFirstFind(stopFast) {}

		StdMatchFinder::~StdMatchFinder() {}

		void StdMatchFinder::setInjectivity(bool injective) {
			matchInjective = injective;
		}

		bool StdMatchFinder::matchesInjectively() const {
			return matchInjective;
		}

		void StdMatchFinder::setStopOnFirstFind(bool stopFast) {
			stopOnFirstFind = stopFast;
		}

		bool StdMatchFinder::stopsOnFirstFind() const {
			return stopOnFirstFind;
		}

		shared_ptr<vector<Mapping>> StdMatchFinder::findMatches(
				AnonHypergraph const& source,
				AnonHypergraph const& target) const {

			shared_ptr<vector<Mapping>> resultMappings = make_shared<vector<Mapping>>();
			// source graph is bigger than target graph => no injective matching possible
			if(matchInjective) {
				if(source.getVertexCount() > target.getVertexCount() || source.getEdgeCount() > target.getEdgeCount()) {
					return resultMappings;
				}
			}

			size_t edgesToGo = source.getEdgeCount();
			// catch the case when there are no edges (discrete graph)
			if(edgesToGo == 0) {
				Mapping tempMap;
				resultMappings->push_back(tempMap);
			} else {
				// if there is at least one edge, mapEdge will recursively map all edges and their attached nodes, in all possible ways
				unordered_set<IDType> mappableEdges;
				Mapping currentMapping;
				Mapping revCurrentMapping;
				mapEdges(source, target, edgesToGo, mappableEdges, currentMapping, revCurrentMapping, resultMappings);
			}

			// just map the remaining isolated nodes and return the result
			return mapIsolatedNodes(source, target, resultMappings);
		}

		void StdMatchFinder::mapEdges(
				AnonHypergraph const& source,
				AnonHypergraph const& target,
				size_t edgesToGo,
				unordered_set<IDType>& mappableEdges,
				Mapping& currentMapping,
				Mapping& revCurrentMapping,
				shared_ptr<vector<Mapping>> foundMappings) const {

			// all edges were mapped, match found
			if(edgesToGo == 0) {
				foundMappings->push_back(currentMapping);
			} else {

				IDType currentEdge;
				vector<IDType> possibleTargets;

				// no mappable edge found, so find a new entry point
				if(mappableEdges.size() == 0) {

					currentEdge = getNewEntryPoint(source, currentMapping);
					// since the edge is completely new, all edges of the target graph are potential targets
					for(auto iter = target.beginEdges(); iter != target.endEdges(); ++iter) {
						if(isMapPossible(source, currentEdge, target, iter->first, currentMapping, revCurrentMapping)) {
							possibleTargets.push_back(iter->first);
						}
					}

				} else {

					currentEdge = *(mappableEdges.begin());
					vector<IDType> const* srcNodes = source.getVerticesOfEdge(currentEdge);
					// at least one of the vertices of the current edge was already mapped; find it and use it to find edge candidates
					AnonHypergraph::ESet const* edgeCandidates = nullptr;
					for(vector<IDType>::const_iterator iter = srcNodes->begin(); iter != srcNodes->end(); ++iter) {
						if(currentMapping.hasSrc(*iter)) {
							edgeCandidates = target.getConnectedEdges(currentMapping[*iter]);
							break;
						}
					}

					// this error cannot occur
					if(edgeCandidates == nullptr) {
						throw RunException("Matcher failed to use a common node to find new edge candidates."
								" Flood search error.");
					}

					// dublicationCheck is used to prevent duplications of possible targets, since edges can be attached to a node more than once
					unordered_set<IDType> dublicationCheck;

					// since the edge is completely new, all edges of the target graph are potential targets
					for(auto iter = edgeCandidates->cbegin(); iter != edgeCandidates->cend(); ++iter) {
						if(dublicationCheck.count(*iter) == 0 &&
								isMapPossible(source, currentEdge, target, *iter, currentMapping, revCurrentMapping)) {
							possibleTargets.push_back(*iter);
							dublicationCheck.insert(*iter);
						}
					}
				}

				bool mappableEdgeDeleted = (mappableEdges.erase(currentEdge) > 0);
				// recursively choose a possibleTarget and check if further matching is possible
				for(vector<IDType>::iterator iter = possibleTargets.begin(); iter != possibleTargets.end(); ++iter) {
					// map the edge and calculate differences
					vector<IDType> currentMappingDiff;
					vector<IDType> revCurrentMappingDiff;
					vector<IDType> mappableEdgesDiff;
					{
						currentMapping[currentEdge] = *iter;
						currentMappingDiff.push_back(currentEdge);
						if(matchInjective) {
							revCurrentMapping[*iter] = currentEdge;
							revCurrentMappingDiff.push_back(*iter);
						}

						vector<IDType> const* srcNodes = source.getVerticesOfEdge(currentEdge);
						for(vector<IDType>::const_iterator srcIt = srcNodes->begin(), tarIt = target.getVerticesOfEdge(*iter)->begin();
								srcIt != srcNodes->end(); ++srcIt, ++tarIt) {
							if(!currentMapping.hasSrc(*srcIt)) {
								currentMapping[*srcIt] = *tarIt;
								currentMappingDiff.push_back(*srcIt);
								if(matchInjective) {
									revCurrentMapping[*tarIt] = *srcIt;
									revCurrentMappingDiff.push_back(*tarIt);
								}

								auto conEdges = source.getConnectedEdges(*srcIt);
								for(auto conEdgesIt = conEdges->cbegin(); conEdgesIt != conEdges->cend(); ++conEdgesIt) {
									if(!currentMapping.hasSrc(*conEdgesIt) && (mappableEdges.count(*conEdgesIt) == 0)) {
										mappableEdges.insert(*conEdgesIt);
										mappableEdgesDiff.push_back(*conEdgesIt);
									}
								}
							}
						}
					}

					// recursion checks the other edges of the left rule graph for possible matches
					mapEdges(source, target, edgesToGo-1, mappableEdges, currentMapping, revCurrentMapping, foundMappings);

					// stop if just one mapping is searched for
					if(stopOnFirstFind && (foundMappings->size() != 0)) {
						return;
					}

					// undo all changes to currentMapping, foundTargets and mappableEdges
					for(vector<IDType>::iterator iter2 = currentMappingDiff.begin(); iter2 != currentMappingDiff.end(); ++iter2) {
						currentMapping.delSrc(*iter2);
					}
					for(vector<IDType>::iterator iter2 = revCurrentMappingDiff.begin(); iter2 != revCurrentMappingDiff.end(); ++iter2) {
						revCurrentMapping.delSrc(*iter2);
					}
					for(vector<IDType>::iterator iter2 = mappableEdgesDiff.begin(); iter2 != mappableEdgesDiff.end(); ++iter2) {
						mappableEdges.erase(*iter2);
					}
				}

				if(mappableEdgeDeleted) {
					mappableEdges.insert(currentEdge);
				}
			}
		}

		IDType StdMatchFinder::getNewEntryPoint(AnonHypergraph const& source, const Mapping& currentMapping) const {
			IDType startEdge;
			int lastArity = -1;
			for(auto iter = source.beginEdges(); iter != source.endEdges(); ++iter) {
				if(!currentMapping.hasSrc(iter->first) && (source.getEdgeArity(iter->first) > lastArity)) {
					lastArity = source.getEdgeArity(iter->first);
					startEdge = iter->first;
				}
			}

			if(lastArity == -1) {
				throw RunException("Matcher failed to match all edges of the left rule side. New entry point failure.");
			}
			return startEdge;
		}

		bool StdMatchFinder::isMapPossible(
				AnonHypergraph const& source,
				IDType srcEdge,
				AnonHypergraph const& target,
				IDType tarEdge,
				Mapping& currentMapping,
				Mapping& revCurrentMapping) const {

			// check if labels are equal, additionally check if target is already mapped to in injective case
			if(!(matchInjective && revCurrentMapping.hasSrc(tarEdge)) &&
					(source.getEdge(srcEdge)->getLabel().compare(target.getEdge(tarEdge)->getLabel()) == 0)) {
				vector<IDType> const* srcNodes = source.getVerticesOfEdge(srcEdge);
				vector<IDType> const* tarNodes = target.getVerticesOfEdge(tarEdge);
				if(srcNodes->size() != tarNodes->size()) {
					logger.newWarning() << "An edge label is used for two different arities! No Matching possible." << endLogMessage;
				} else {

					unordered_map<IDType,IDType> tempMapping;
					for(vector<IDType>::const_iterator srcIt = srcNodes->begin(), tarIt = tarNodes->begin(); srcIt != srcNodes->end(); ++srcIt, ++tarIt) {

						// if a node is already mapped, abort, if the mapping is not compatible
						if(currentMapping.hasSrc(*srcIt) && (currentMapping[*srcIt] != *tarIt)) {
							return false;
						}

						// in the injective case a node may not be mapped to, if it is already mapped to from a different node
						if(matchInjective && revCurrentMapping.hasSrc(*tarIt) && (revCurrentMapping[*tarIt] != *srcIt)) {
							return false;
						}

						// if the source edge has an id multiple times, it also has to be the same in the target edge
						if((tempMapping.count(*srcIt) != 0) && (tempMapping[*srcIt] != *tarIt)) {
							return false;
						} else {
							// (compatible) mappings already in currentMapping are also added to tempMapping in this way
							tempMapping[*srcIt] = *tarIt;
						}
					}
				}
				// nothing speaks against adding the match
				return true;
			}
			return false;
		}

		shared_ptr<vector<Mapping>> StdMatchFinder::mapIsolatedNodes(
				AnonHypergraph const& source,
				AnonHypergraph const& target,
				shared_ptr<vector<Mapping>> foundMappings) const {

			// no mappings = nothing to do
			if(foundMappings->size() == 0) {
				return foundMappings;
			}

			// one mapping can be used as representative to search for isolated nodes
			vector<Mapping>::iterator mappingIt = foundMappings->begin();
			vector<IDType> isolatedNodes;
			for(auto iter = source.beginVertices(); iter != source.endVertices(); ++iter) {
				if(!mappingIt->hasSrc(iter->first)) {
					isolatedNodes.push_back(iter->first);
				}
			}

			// no isolated nodes = nothing to do
			if(isolatedNodes.size() == 0) {
				return foundMappings;
			}

			// at this point it is clear that isolated nodes have to be mapped
			shared_ptr<vector<Mapping>> result = make_shared<vector<Mapping>>();

			// just map the nodes in some way
			if(stopOnFirstFind) {
				Mapping& mapp = *(foundMappings->begin());
				if(!matchInjective) {
					IDType targetID = target.beginVertices()->first;
					for(vector<IDType>::iterator iter = isolatedNodes.begin(); iter != isolatedNodes.end(); ++iter) {
						mapp[*iter] = targetID;
					}
				} else {
					auto tarIt = target.beginVertices();
					for(vector<IDType>::iterator iter = isolatedNodes.begin(); iter != isolatedNodes.end(); ++iter) {
						while(mapp.hasTar(tarIt->first) && (tarIt != target.endVertices())) {
							++tarIt;
						}
						// return empty list if not enough nodes to map all injectively
						if(tarIt == target.endVertices()) {
							return result;
						}
						mapp[*iter] = tarIt->first;
					}
				}
				result->push_back(mapp);
				return result;
			}

			// mapping isolated nodes in all possible ways
			vector<IDType> possibleTargets;
			// in the non-injective case the possible targets do not depend on the already found mapping
			if(!matchInjective) {
				for(auto iter = target.beginVertices(); iter != target.endVertices(); ++iter) {
					possibleTargets.push_back(iter->first);
				}
			}
			// every mapping has to be processed individually
			for(vector<Mapping>::iterator mapIt = foundMappings->begin(); mapIt != foundMappings->end(); ++mapIt) {
				if(matchInjective) {
					// only nodes not already mapped to are valid targets
					possibleTargets.clear();
					for(auto iter = target.beginVertices(); iter != target.endVertices(); ++iter) {
						if(!mapIt->hasTar(iter->first)) {
							possibleTargets.push_back(iter->first);
						}
					}
					unordered_set<IDType> noTargets;
					enumAllIsoNodesInj(isolatedNodes, 0, possibleTargets, noTargets, *mapIt, result);
				} else { // non-injective case
					enumAllIsoNodesNonInj(isolatedNodes, possibleTargets, *mapIt, result);
				}
			}

			return result;
		}

		void StdMatchFinder::enumAllIsoNodesInj(
				vector<IDType>& isolatedNodes,
				size_t isolatedNodesPos,
				vector<IDType>& targets,
				unordered_set<IDType>& noTargets,
				Mapping& currentMapping,
				shared_ptr<vector<Mapping>> allMappings) const {

			if(isolatedNodesPos >= isolatedNodes.size()) {
				allMappings->push_back(currentMapping);
			} else {
				for(vector<IDType>::iterator iter = targets.begin(); iter != targets.end(); ++iter) {
					if(noTargets.count(*iter) == 0) {
						noTargets.insert(*iter);
						currentMapping[isolatedNodes.at(isolatedNodesPos)] = *iter;
						enumAllIsoNodesInj(isolatedNodes, isolatedNodesPos + 1, targets, noTargets, currentMapping, allMappings);
						noTargets.erase(*iter);
					}
				}
			}
		}

		void StdMatchFinder::enumAllIsoNodesNonInj(
				vector<IDType>& isolatedNodes,
				vector<IDType>& targets,
				Mapping& currentMapping,
				shared_ptr<vector<Mapping>> allMappings) const {

			if(targets.size() == 0) {
				return;
			}
			ExpEnumerator enumerator(targets.size(), isolatedNodes.size());
			while(true) {
				for(size_t i = 0; i < isolatedNodes.size(); ++i) {
					currentMapping[isolatedNodes.at(i)] = targets.at(enumerator[i]);
				}
				allMappings->push_back(currentMapping);
				if(enumerator.canCountUp()) {
					++enumerator;
				} else {
					return;
				}
			}
		}

	} /* namespace rule_engine */
} /* namespace uncover */
