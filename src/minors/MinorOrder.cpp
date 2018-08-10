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

#include "MinorOrder.h"

using namespace std;
using namespace uncover::graphs;
using namespace uncover::basic_types;

namespace uncover {
	namespace minors {

		MinorOrder::MinorOrder() {}

		MinorOrder::~MinorOrder() {}

		bool MinorOrder::isLessOrEq(Hypergraph const& smaller, Hypergraph const& bigger) const {

			// abort if 'smaller' can obviously not be smaller
			if(smaller.getEdgeCount() > bigger.getEdgeCount() || smaller.getVertexCount() > bigger.getVertexCount()) {
				return false;
			}

			// index edges in the smaller graph by their labels
			map<string, vector<IDType>> edgesInS;
			for(auto it = smaller.cbeginEdges(); it != smaller.cendEdges(); ++it)
			{
				edgesInS[it->second.getLabel()].push_back(it->first);
			}

			// index edges in the larger graph by their labels, but only if their label is also used in the smaller graph
			map<string, vector<IDType>> edgesInB;
			for(auto it = bigger.cbeginEdges(); it != bigger.cendEdges(); ++it)
			{
				if(edgesInS.count(it->second.getLabel())) {
					edgesInB[it->second.getLabel()].push_back(it->first);
				}
			}
			
			// for each label the larger graph must have at least as much edges as the smaller graph
			for(auto it = edgesInS.cbegin(); it != edgesInS.cend(); ++it)
			{
				if(it->second.size() > edgesInB[it->first].size()) {
					return false;
				}
			}
			
			// initialize the mapping from edges of the smaller graph to edges of the larger graph
			vector<pair<string,vector<size_t>>> currentEdgeMap;
			for(auto& p : edgesInS) {
				currentEdgeMap.push_back(make_pair(p.first,vector<size_t>()));
				reset(currentEdgeMap.back().second,p.second.size());
			}

			// compute how much isolated vertices the smaller graph has more than the bigger one
			int isoNodes = 0;
			for(auto it = smaller.cbeginVertices(); it != smaller.cendVertices(); ++it) {
				if(smaller.getConnEdgesCount(it->first) == 0) {
					++isoNodes;
				}
			}
			for(auto it = bigger.cbeginVertices(); it != bigger.cendVertices(); ++it) {
				if(bigger.getConnEdgesCount(it->first) == 0) {
					--isoNodes;
				}
			}

			// try every possible mappings for the edges
			do {

				// first generate the corresponding Mapping (of node IDs only)
				Mapping mapp;
				unordered_set<IDType> preservedEdges;
				if(!generateMapp(smaller,bigger,edgesInS,edgesInB,currentEdgeMap,mapp,preservedEdges)) {
					continue;
				}
				Mapping::Inversion invMapp;
				mapp.getInverseMatch(invMapp);

				// try all possible contractions of other edges
				if(checkContractions(bigger,mapp,invMapp,preservedEdges,isoNodes)) {
					return true;
				}

			} while (inc(currentEdgeMap,edgesInB));

			return false;

		}

		bool MinorOrder::checkContractions(
				Hypergraph const& graphB,
				Mapping const& nodeMap,
				Mapping::Inversion const& invNodeMap,
				unordered_set<IDType> const& preservedEdges,
				int isoNodes) const {

			// initialize all IDPartitionEnumerators
			vector<IDPartitionEnumerator> enumeratorList;
			for(auto it = graphB.cbeginEdges(); it != graphB.cendEdges(); ++it) {
				// ignore all edges which have an image in the smaller graph
				if(preservedEdges.count(it->first) == 0) {
					IDPartitionEnumerator enumer(it->second.getVertices());
					if(enumer.baseSetSize() > 1) {
						enumeratorList.push_back(move(enumer));
					}
				}
			}

			// try all combinations of enumerator results
			size_t skip = 0;
			do {

				// generate initial partition with all non-isolated nodes in an own subpartition
				IDPartition partition;
				for(auto it = graphB.cbeginVertices(); it != graphB.cendVertices(); ++it) {
					if(graphB.getConnEdgesCount(it->first) > 0) {
						partition.insert(SubIDPartition({it->first}));
					}
				}

				// small trick: we increment from 0 to 'size' but merge from 'size' to 0
				// if we can find an invalid merging before reaching index 0, we can skip a lot of increments
				skip = 0;
				bool aborted = false;
				for(size_t i = enumeratorList.size()-1; i < enumeratorList.size(); --i) {
					idpart_transitive_closure_into1(partition,*enumeratorList.at(i));
					if(!isFiner(partition,nodeMap)) {
						aborted = true;
						skip = i;
						break;
					}
				}

				// if we did not abort prematurely, we check if the desired merging was found
				if(!aborted && isIncludedIn(partition,invNodeMap) && enoughIsolated(partition,nodeMap,isoNodes)) {
					return true;
				}

			} while (inc(enumeratorList,skip));

			return false;
		}

		inline bool MinorOrder::enoughIsolated(IDPartition const& part, Mapping const& mapp, int num) const {

			// there are at least as much isolated nodes in the larger graph as there are in the smaller graph
			if(num <= 0) {
				return true;
			}

			// for each isolated node which the smaller graph has more than the larger graph, there must be a
			// set of merged IDs which is not already matched, i.e. the images of all elements are undefined
			auto iter = part.cbegin();
			do {

				if(iter == part.cend()) {
					return false;
				}

				// if an image is defined, ignore this set of IDs
				for(auto& id : *iter) {
					if(mapp.hasSrc(id)) {
						++num;
						break;
					}
				}

				--num;
				++iter;

			} while(num > 0);

			return true;
		}

		inline bool MinorOrder::isIncludedIn(IDPartition const& part, Mapping::Inversion const& mapp) const {

			// for each set of IDs with the same images under mapp
			for(auto it = mapp.cbegin(); it != mapp.cend(); ++it) {

				// take one ID and search for the SubIDPartition in which it is contained
				auto inIt = it->second.cbegin();
				SubIDPartition const* subPart = nullptr;
				for(auto& el : part) {
					if(el.count(*inIt) > 0) {
						subPart = &el;
						break;
					}
				}

				// check that:
				// (1) the chosen ID is in at least one subpartition (it is guaranteed to be in no more than that)
				// (2) every ID with the same image (under mapp) as the chosen ID is also in this subpartition
				if(subPart == nullptr) {
					return false;
				} else {
					++inIt;
					for(;inIt != it->second.cend(); ++inIt) {
						if(subPart->count(*inIt) == 0) {
							return false;
						}
					}
				}
			}

			return true;
		}

		bool MinorOrder::generateMapp(
				graphs::Hypergraph const& graphS,
				graphs::Hypergraph const& graphB,
				map<string, vector<IDType>> const& edgeMapS,
				map<string, vector<IDType>> const& edgeMapB,
				vector<pair<string,vector<size_t>>> const& edgesToEdges,
				graphs::Mapping& nodeMap,
				unordered_set<IDType>& preservedEdges) const {

			nodeMap.clear();
			preservedEdges.clear();

			for(auto& p : edgesToEdges) {
				for(size_t index = 0; index < p.second.size(); ++index) {
					Edge const* edgeS = graphS.getEdge(edgeMapS.at(p.first).at(index));
					Edge const* edgeB = graphB.getEdge(edgeMapB.at(p.first).at(p.second.at(index)));
					preservedEdges.insert(edgeB->getID());

					auto edgeSit = edgeS->cbeginV();
					auto edgeBit = edgeB->cbeginV();
					for(;edgeBit != edgeB->cendV(); ++edgeBit, ++edgeSit) {

						// add a map entry if none is defined
						if(!nodeMap.hasSrc(*edgeBit)) {
							nodeMap[*edgeBit] = *edgeSit;

						// abort if an entry exists and is inconsistent with this mapping
						} else if (nodeMap[*edgeBit] != *edgeSit) {
							return false;
						} // else do nothing

					}

				}
			}

			return true;

		}

		inline void MinorOrder::reset(vector<size_t>& vec, size_t numCount) const {
			vec.clear();
			for(size_t i = 0; i < numCount; ++i) {
				vec.push_back(i);
			}
		}

		inline bool MinorOrder::inc(vector<IDPartitionEnumerator>& enums, size_t pos) const {

			for(size_t i = 0; i < pos; ++i) {
				enums[i].reset();
			}

			for(size_t i = pos; i < enums.size(); ++i) {
				++enums[i];
				if(enums[i].ended()) {
					enums[i].reset();
				} else {
					return true;
				}
			}

			return false;

		}

		bool MinorOrder::inc(vector<pair<string,vector<size_t>>>& vec, map<string, vector<IDType>>& posTargets) const {

			for(size_t index = 0; index < vec.size(); ++index) {
				if(inc(vec[index].second,posTargets[vec[index].first].size())) {
					return true;
				} else {
					reset(vec[index].second,vec[index].second.size());
				}
			}

			return false;
		}

		bool MinorOrder::inc(vector<size_t>& vec, size_t tarCount) const {

			size_t index = vec.size() - 1;
			for(;; --index) {

				// the last element was enumerated if this holds:
				if(index == 0 && vec[index] == tarCount - 1) {
					return false;
				}

				// increment the value at the current index (mod tarCount)
				size_t curValue = (vec[index] + 1) % tarCount;
				vec[index] = tarCount;
				bool finished = false;
				do {

					size_t pos = findFirst(vec,curValue);
					// if the new value is not already used, use it for this index
					if(pos >= vec.size()) {
						vec[index] = curValue;
						finished = true;
						break;
					// if the new value is found at the preceding index, the vector is incremented maximally
					// up to this index, and the next index must be checked
					} else if(pos + 1 == index) {
						break;
					// if the new value is already used, ignore it
					} else {
						curValue = (curValue + 1) % tarCount;
					}

				} while (true);

				if(finished) break;
			}

			// if vec was incremented at some index in the previous loop, all larger values of index
			// must be initialized again (with the smallest unused values larger than vec[index])
			size_t curValue = (vec[index] + 1) % tarCount;
			++index;
			while(index < vec.size()) {
				if(findFirst(vec,curValue) >= vec.size()) {
					vec[index] = curValue;
					++index;
				}
				curValue = (curValue + 1) % tarCount;
			}

			return true;

		}

		inline size_t MinorOrder::findFirst(vector<size_t> const& vec, size_t tofind) const {
			for(size_t i = 0; i < vec.size(); ++i) {
				if(vec.at(i) == tofind) {
					return i;
				}
			}
			return vec.size();
		}

		inline bool MinorOrder::isFiner(IDPartition const& part, Mapping const& mapp) const {

			for(auto& sp : part) {
				IDType id = 0;
				for(auto& spel : sp) {
					// ignore IDs without image under mapp
					if(mapp.hasSrc(spel)) {
						if(id == 0) {
							id = mapp.getTar(spel);
						} else if (id != mapp.getTar(spel)) {
							return false;
						}
					}
				}
			}

			return true;
		}

	}
}
