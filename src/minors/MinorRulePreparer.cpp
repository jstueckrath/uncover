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

#include "MinorRulePreparer.h"
#include <sstream>
#include <math.h>
#include "../logging/message_logging.h"
#include "../basic_types/RunException.h"
#include "../basic_types/standard_operators.h"
#include "../basic_types/IDPartitionEnumerator.h"
#include "../analysis/rule_preparation_helper.h"

using namespace std;
using namespace uncover::logging;
using namespace uncover::graphs;
using namespace uncover::analysis;
using namespace uncover::basic_types;

namespace uncover {
	namespace minors {

		MinorRulePreparer::MinorRulePreparer() {}

		MinorRulePreparer::~MinorRulePreparer() {}

		/**
		 * Adds one IDPartitionEnumerator to the given vector for each edge in genFrom, but only if the edge can be
		 * contracted in a way which is not also a simple deletion. A real contraction is possible if the edge is
		 * incident to at least two different vertices which are not already marked to be deleted.
		 * @param enumList the vector to which generated IDPartitionEnumerators will be added
		 * @param genFrom the set of edges for which IDPartitionEnumerators should be generated
		 * @param delVertices the set of vertices which are marked to be deleted
		 * @param graph the graph which contains all given edge and vertex IDs
		 */
		void generateIDEnumerator(vector<IDPartitionEnumerator>& enumList,
							unordered_set<IDType> const& genFrom,
							unordered_set<IDType> const& delVertices,
							AnonHypergraph const& graph) {

			for(auto eit = genFrom.cbegin(); eit != genFrom.cend(); ++eit) {

				// search edges for all nodes which are not deleted (and hence mergeable)
				vector<IDType> mergeableVertices = *graph.getVerticesOfEdge(*eit);
				vector<IDType>::iterator vit = mergeableVertices.begin();
				while(vit != mergeableVertices.end()) {
					if(delVertices.count(*vit)) {
						vit = mergeableVertices.erase(vit);
					} else {
						++vit;
					}
				}

				// ignore edges with at most one non-deleted node
				if(mergeableVertices.size() > 1) {
					// check if the current edge is incident to at least two different vertices
					// ignore any edge for which this is not the case
					vit = mergeableVertices.begin();
					IDType oneID(*vit);
					++vit;
					for(; vit != mergeableVertices.end(); ++vit) {
						if(oneID != *vit) {
							enumList.push_back(IDPartitionEnumerator(mergeableVertices));
							break;
						}
					}
				} // else do nothing

			}
		}

		/**
		 * Serves as an implementation of the MinorRulePreparer::prepareRule(...) variants for normal and UQRules.
		 * @param rule a reference to the rule to be prepared
		 * @param callback a callback for storing newly generated rules
		 * @param rulePreparer the RulePreparer used for preparation
		 */
		template<typename T>
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

				unordered_set<IDType> deletedVertices;
				unordered_set<IDType> deletedEdges;

				// calculate all nodes which will be deleted in this iteration and their attached edges
				for(unsigned int i = 0; i < nodeCount; ++i) {
					if(nodeDefMap & (1 << i)) {
						deletedVertices.insert(nodeID[i]);
						newRight->deleteVertex(nodeID[i], &deletedEdges);
					}
				}

				// also delete mappings of deleted nodes
				Mapping newMapping(rule.getMapping());
				newMapping.delTar(deletedVertices);
				newMapping.delTar(deletedEdges);

				// find all non-deleted edges
				vector<IDType> remainingEdges;
				for(unsigned int i = 0; i < edgeCount; ++i) {
					if(deletedEdges.count(edgeID[i]) == 0) {
						remainingEdges.push_back(edgeID[i]);
					}
				}

				// now enumerate all undeleted edges and contract them using all possible partitions
				unsigned int edgeDefMap;
				unsigned int edgeDefMapMax = (unsigned int)pow(2,remainingEdges.size());
				for(edgeDefMap = 0; edgeDefMap < edgeDefMapMax; ++edgeDefMap) {

					// calculate additional edges, where no attached nodes where deleted
					unordered_set<IDType> additionalDeletedEdges = deletedEdges;
					for(unsigned int i = 0; i < remainingEdges.size(); ++i) {
						if(edgeDefMap & (1 << i)) {
							additionalDeletedEdges.insert(remainingEdges.at(i));
						}
					}

					// enumerate over all possible contractions of deleted edges
					vector<IDPartitionEnumerator> partitionEnums;
					generateIDEnumerator(partitionEnums, deletedEdges, deletedVertices, *rightSide);
					generateIDEnumerator(partitionEnums, additionalDeletedEdges, deletedVertices, *rightSide);

					if(partitionEnums.size() == 0) {

						// generate a full copy of the rule, including all changes to this point
						auto finalNewRight = make_shared<AnonHypergraph>(*newRight);
						stringstream converter;
						converter << (rule.getName()) << "-v" << counter;
						++counter;
						shared_ptr<T> newRule = createRuleFrom(rule,
																									 converter.str(),
																									 make_shared<AnonHypergraph>(*rule.getLeft()),
																									 finalNewRight,
																									 newMapping);
						Mapping& finalNewMapping = newRule->getMapping();

						// also delete additional edges
						for(unordered_set<IDType>::iterator eit = additionalDeletedEdges.begin(); eit != additionalDeletedEdges.end(); ++eit) {
							finalNewRight->deleteEdge(*eit);
						}
						finalNewMapping.delTar(additionalDeletedEdges);

						// no contractions performed
						if(!rulePreparer.willOptimize() || !rulePreparer.isNoOpRule(*newRule)) {
							callback.insert(newRule);
						}

					} else {

						size_t index = 0; // index of last element of paritionsEnums which has not ended
						unordered_set<IDPartition> enumedPartitions;
						while(index < partitionEnums.size()) {

							// for transitive closure of all current elements of partitionEnums
							vector<IDPartitionEnumerator>::iterator enumIt = partitionEnums.begin();
							IDPartition finalPartition = **enumIt;
							++enumIt;
							for(; enumIt != partitionEnums.end(); ++enumIt) {
								idpart_transitive_closure_into1(finalPartition,**enumIt);
							}

							// add a new rule if new partition was generated
							if(enumedPartitions.insert(finalPartition).second) {

								// generate a full copy of the rule, including all changes to this point
								auto finalNewRight = make_shared<AnonHypergraph>(*newRight);
								stringstream converter;
								converter << (rule.getName()) << "-v" << counter;
								++counter;
								shared_ptr<T> newRule = createRuleFrom(rule,
																											 converter.str(),
																											 make_shared<AnonHypergraph>(*rule.getLeft()),
																											 finalNewRight,
																											 newMapping);
								Mapping& finalNewMapping = newRule->getMapping();

								// also delete additional edges
								for(auto eit = additionalDeletedEdges.begin(); eit != additionalDeletedEdges.end(); ++eit) {
									finalNewRight->deleteEdge(*eit);
								}
								finalNewMapping.delTar(additionalDeletedEdges);

								// perform contractions
								for(auto partIt = finalPartition.cbegin(); partIt != finalPartition.cend(); ++partIt) {

									if(partIt->size() <= 1) {
										continue;
									}

									unordered_set<IDType>::const_iterator innerPartIt = partIt->cbegin();
									IDType currentID = *innerPartIt;
									++innerPartIt;

									for(; innerPartIt != partIt->cend(); ++innerPartIt) {

										// merge vertices in graph
										IDType oldID = currentID;
										currentID = finalNewRight->mergeVertices(currentID, *innerPartIt);

										// correct mappings to old currentID
										if(currentID != oldID) {
											unordered_set<IDType> oldMaps = finalNewMapping.getInverseMatch(oldID);
											for(auto oldMapsIt = oldMaps.cbegin(); oldMapsIt != oldMaps.cend(); ++oldMapsIt) {
												finalNewMapping[*oldMapsIt] = currentID;
											}
										}

										// correct mappings to *innerPartIt
										if(currentID != *innerPartIt) {
											unordered_set<IDType> oldMaps = finalNewMapping.getInverseMatch(*innerPartIt);
											for(auto oldMapsIt = oldMaps.cbegin(); oldMapsIt != oldMaps.cend(); ++oldMapsIt) {
												finalNewMapping[*oldMapsIt] = currentID;
											}
										}
									}
								}

								// add rule
								if(!rulePreparer.willOptimize() || !rulePreparer.isNoOpRule(*newRule)) {
									callback.insert(newRule);
								}
							}

							// increase partitionEnums
							index = 0;
							while(index < partitionEnums.size()) {
								++partitionEnums.at(index);
								if(partitionEnums.at(index).ended()) {
									partitionEnums.at(index).reset();
									++index;
								} else {
									break;
								}
							}

						}
					}

				}
			}

		}

		void MinorRulePreparer::prepareRule(Rule const& rule, CollectionCallback<Rule_sp>& callback) const {
			prepareRule_Impl<Rule>(rule, callback, *this);
		}

		void MinorRulePreparer::prepareRule(UQRuleInstance const& rule, CollectionCallback<UQRuleInstance_sp>& callback) const {
			prepareRule_Impl<UQRuleInstance>(rule, callback, *this);
		}

		bool MinorRulePreparer::isNoOpRule(AnonRule const& rule) const {

			Mapping const& oldMapping = rule.getMapping();
			auto rgraph = rule.getRight();
			auto lgraph = rule.getLeft();

			Mapping::Inversion inverseMapping;
			oldMapping.getInverseMatch(inverseMapping);

			// check surjectivity on vertices
			for(auto it = rgraph->cbeginVertices(); it != rgraph->cendVertices(); ++it) {
				if(!inverseMapping.count(it->first)) {
					return false;
				}
			}

			// check injectivity and surjectivity on edges
			for(auto it = rgraph->cbeginEdges(); it != rgraph->cendEdges(); ++it) {
				if(!inverseMapping.count(it->first)) {
					return false; // not surjective
				} else if(inverseMapping[it->first].size() != 1) {
					return false; // not injective
				}
			}

			// check if there is a contracted path between vertices with identical mappings
			for(auto it = inverseMapping.begin(); it != inverseMapping.end(); ++it) {

				// ignore elements with only one preimage; this also includes all edges!
				if(it->second.size() < 2) {
					continue;
				}

				unordered_set<IDType> foundIDs;
				vector<IDType> todoIDs;
				todoIDs.push_back(*it->second.cbegin());

				while(todoIDs.size() > 0) {
					IDType currentID  = todoIDs.back();
					todoIDs.pop_back();
					foundIDs.insert(currentID);

					auto conEdges = lgraph->getConnectedEdges(currentID);
					for(auto eit = conEdges->cbegin(); eit != conEdges->cend(); ++eit) {

						// ignore non-contracted edges
						if(oldMapping.hasSrc(*eit)) {
							continue;
						}

						vector<IDType> const* attachedV = lgraph->getVerticesOfEdge(*eit);
						for(vector<IDType>::const_iterator vit = attachedV->cbegin(); vit != attachedV->cend(); ++vit) {
							if(it->second.count(*vit) && !foundIDs.count(*vit)) {
								foundIDs.insert(*vit);
								todoIDs.push_back(*vit);
							}
						}

					}
				}

				if(foundIDs.size() != it->second.size()) {
					return false;
				} // else foundIDs == it->second
			}

			return true;
		}

	}
}
