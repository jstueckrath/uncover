/***************************************************************************
 *   Copyright (C) 2005 by SZS                                             *
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

#include "AnonHypergraph.h"
#include <string.h>
#include <sstream>
#include "../basic_types/standard_operators.h"
#include "../logging/message_logging.h"

using namespace uncover::basic_types;
using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace graphs {

		AnonHypergraph::AnonHypergraph(string name) :
				lastUsedID(0),
				name(name),
				vertices(),
				edges(),
				connectedEdges() {}

		AnonHypergraph::AnonHypergraph(const AnonHypergraph& graph) :
				lastUsedID(graph.lastUsedID),
				name(graph.name),
				vertices(graph.vertices),
				edges(graph.edges),
				connectedEdges(graph.connectedEdges) {
		}

		AnonHypergraph::~AnonHypergraph()	{}

		void AnonHypergraph::mergeVertexIntoFirst(IDType id1, IDType id2) {
			mergeVertices(id1,id2);
		}

		IDType AnonHypergraph::mergeVertices(IDType id1, IDType id2) {

			// check if both IDs are vertices
			if(vertices.count(id1) == 0 || vertices.count(id2) == 0) {

				stringstream str;
				str << "Failed to merge vertices with IDs '" << id1 << "' and '" << id2 << "'. ";
				if(vertices.count(id2) != 0) {
					str << "There is no vertex with ID '" << id1 << "'.";
				} else if(vertices.count(id1) != 0) {
					str << "There is no vertex with ID '" << id2 << "'.";
				} else {
					str << "Both IDs are no valid vertices.";
				}
				str << " The problematic graph was: " << *this;

				throw InvalidInputException(str.str());

			} else if(id1 == id2) {
				return id1;
			} else {

				for(auto eit = connectedEdges[id2].cbegin(); eit != connectedEdges[id2].cend(); ++eit) {
					for(auto vit = edges.at(*eit).beginV(); vit != edges.at(*eit).endV(); ++vit) {
						if(*vit == id2) {
							*vit = id1;
							connectedEdges[id1].insert(*eit);
						}
					}
				}

				connectedEdges.erase(id2);
				vertices.erase(id2);

				return id1;
			}
		}

		IDType AnonHypergraph::mergeVertices(unordered_set<IDType> const& ids) {

			// only store IDs that actually exist
			vector<IDType> minIDs;
			for(unordered_set<IDType>::const_iterator it = ids.cbegin(); it != ids.cend(); ++it) {
				if(vertices.count(*it)) {
					minIDs.push_back(*it);
				}
			}

			if(ids.size() != minIDs.size()) {
				logger.newWarning() << "AnonHypergraph::mergeVertices(unordered_set<IDType>) was called with a set "
						"which contained invalid nodes." << endLogMessage;
			}

			switch(minIDs.size()) {
				case 0:
				{
					stringstream str;
					str << "Cannot merge the given collection of vertices into one vertex, since all IDs are non-valid vertices.";
					str << " The collection was: " << ids << "\nthe graph was: " << *this;
					throw InvalidInputException(str.str());
				}

				case 1:
					return minIDs.back();
				default:

					while(minIDs.size() >= 2) {
						IDType id1 = minIDs.back();
						minIDs.pop_back();
						IDType id2 = minIDs.back();
						minIDs.pop_back();
						minIDs.push_back(mergeVertices(id1,id2));
					}

					return minIDs.back();

			}
		}

		IDType AnonHypergraph::mergeEdges(IDType id1, IDType id2) {

			// check if both IDs are actual edges
			if(edges.count(id1) == 0 || edges.count(id2) == 0) {

				stringstream str;
				str << "Failed to merge edges with IDs '" << id1 << "' and '" << id2 << "'. ";
				if(vertices.count(id2) != 0) {
					str << "There is no edge with ID '" << id1 << "'.";
				} else if(vertices.count(id1) != 0) {
					str << "There is no edge with ID '" << id2 << "'.";
				} else {
					str << "Both IDs are no valid edges.";
				}
				str << " The problematic graph was: " << *this;

				throw InvalidInputException(str.str());

			} else if(id1 == id2) {
				return id1;
			} else if(edges.at(id1).getLabel() != edges.at(id2).getLabel() ||
					edges.at(id1).getArity() != edges.at(id2).getArity()) {

				stringstream str;
				str << "The edge with IDs '" << id1 << "' and  '" << id2 << "' cannot be merged. ";
				if(edges.at(id1).getLabel() != edges.at(id2).getLabel()) {
					str << "Their label differs. ";
				}
				if(edges.at(id1).getArity() != edges.at(id2).getArity()) {
					str << "Their arity differs. ";
				}
				str << " The problematic graph was: " << *this;

				throw InvalidInputException(str.str());

			} else {

				auto id1Nodes = edges.at(id1).getVertices();
				auto id2Nodes = edges.at(id2).getVertices();
				for(size_t i = 0; i < id1Nodes.size(); ++i) {
					mergeVertices(id1Nodes[i], id2Nodes[i]);
				}

				deleteEdge(id2);

				return id1;
			}
		}

		IDType AnonHypergraph::mergeEdges(unordered_set<IDType> const& ids) {

			// only store IDs that actually exist
			vector<IDType> minIDs;
			for(unordered_set<IDType>::const_iterator it = ids.cbegin(); it != ids.cend(); ++it) {
				if(edges.count(*it)) {
					minIDs.push_back(*it);
				}
			}

			if(ids.size() != minIDs.size()) {
				logger.newWarning() << "AnonHypergraph::mergeEdges(unordered_set<IDType>) was called with a set "
						"which contained invalid edges." << endLogMessage;
			}

			switch(minIDs.size()) {
				case 0:
				{
					stringstream str;
					str << "Cannot merge the given collection of edges into one edge, since all IDs are non-valid edges.";
					str << " The collection was: " << ids << "\nthe graph was: " << *this;
					throw InvalidInputException(str.str());
				}

				case 1:
					return minIDs.back();

				default:

					while(minIDs.size() >= 2) {
						IDType id1 = minIDs.back();
						minIDs.pop_back();
						IDType id2 = minIDs.back();
						minIDs.pop_back();
						minIDs.push_back(mergeEdges(id1,id2));
					}

					return minIDs.back();

			}

		}

		string AnonHypergraph::getName() const
		{
			return this->name;
		}

		void AnonHypergraph::setName(string s)
		{
			this->name = s;
		}

		AnonHypergraph::VMap::iterator AnonHypergraph::beginVertices() noexcept {
			return vertices.begin();
		}

		AnonHypergraph::VMap::const_iterator AnonHypergraph::beginVertices() const noexcept {
			return vertices.cbegin();
		}

		AnonHypergraph::VMap::const_iterator AnonHypergraph::cbeginVertices() const noexcept {
			return vertices.cbegin();
		}

		AnonHypergraph::VMap::iterator AnonHypergraph::endVertices() noexcept {
			return vertices.end();
		}

		AnonHypergraph::VMap::const_iterator AnonHypergraph::endVertices() const noexcept {
			return vertices.cend();
		}

		AnonHypergraph::VMap::const_iterator AnonHypergraph::cendVertices() const noexcept {
			return vertices.cend();
		}

		AnonHypergraph::EMap::iterator AnonHypergraph::beginEdges() noexcept {
			return edges.begin();
		}

		AnonHypergraph::EMap::const_iterator AnonHypergraph::beginEdges() const noexcept {
			return edges.cbegin();
		}

		AnonHypergraph::EMap::const_iterator AnonHypergraph::cbeginEdges() const noexcept {
			return edges.cbegin();
		}

		AnonHypergraph::EMap::iterator AnonHypergraph::endEdges() noexcept {
			return edges.end();
		}

		AnonHypergraph::EMap::const_iterator AnonHypergraph::endEdges() const noexcept {
			return edges.cend();
		}

		AnonHypergraph::EMap::const_iterator AnonHypergraph::cendEdges() const noexcept {
			return edges.cend();
		}

		int AnonHypergraph::getConnEdgesCount(IDType vid) const {
			if(connectedEdges.count(vid) != 0) {
				unordered_set<IDType> uniqueIDs;
				for(auto iter = connectedEdges.at(vid).cbegin(); iter != connectedEdges.at(vid).cend(); ++iter) {
					uniqueIDs.insert(*iter);
				}
				return (int)uniqueIDs.size();
			} else {
				return -1;
			}
		}

		IDType AnonHypergraph::addEdge (string label, vector<IDType> const& attVertices) {

			// check if all vertices of the edge exist
			for(vector<IDType>::const_iterator iter = attVertices.begin(); iter != attVertices.end(); iter++) {
				if(vertices.count(*iter) == 0) {
					stringstream str;
					str << "Failed to add edge to graph, since incident ID '" << *iter << "' is no vertex ID.";
					str << " Graphs was: " << *this;
					throw new InvalidInputException(str.str());
				}
			}

			++lastUsedID;
			edges.insert(std::pair<IDType,Edge>(lastUsedID,Edge(lastUsedID, attVertices, label)));

			// store that the new edge is connected to its nodes (from the node perspective)
			for(vector<IDType>::const_iterator iter = attVertices.begin(); iter != attVertices.end(); iter++) {
				if(connectedEdges.count(*iter) == 0) {
					connectedEdges[*iter] = unordered_multiset<IDType>();
				}
				connectedEdges[*iter].insert(lastUsedID);
			}

			return lastUsedID;
		}

		IDType AnonHypergraph::addVertex() {
			++lastUsedID;
			vertices.insert(pair<IDType,Vertex>(lastUsedID,Vertex(lastUsedID)));
			connectedEdges[lastUsedID] = unordered_multiset<IDType>();
			return lastUsedID;
		}

		bool AnonHypergraph::hasEdge(IDType id) const
		{
			return (this->edges.count(id) > 0);
		}

		bool AnonHypergraph::hasVertex(IDType id) const
		{
			return (this->vertices.count(id) > 0);
		}

		vector<IDType>* AnonHypergraph::getVerticesOfEdge(IDType edgeID) {
			if(edges.count(edgeID) != 0) {
				return &(edges.at(edgeID).getVertices());
			}
			return nullptr;
		}

		vector<IDType> const* AnonHypergraph::getVerticesOfEdge(IDType edgeID) const {
			if(edges.count(edgeID) != 0) {
				return &(edges.at(edgeID).getVertices());
			}
			return nullptr;
		}

		Edge* AnonHypergraph::getEdge(IDType id) {
			if(edges.count(id) == 0) {
				return nullptr;
			} else return &(edges.at(id));
		}

		Edge const* AnonHypergraph::getEdge(IDType id) const {
			if(edges.count(id) == 0) {
				return nullptr;
			} else return &(edges.at(id));
		}

		int AnonHypergraph::getEdgeArity(IDType id) const {
			if(edges.count(id) > 0) {
				return ((int)edges.at(id).getArity());
			} else {
				return -1;
			}
		}

		size_t AnonHypergraph::getEdgeCount() const {
			return edges.size();
		}

		size_t AnonHypergraph::getVertexCount() const {
			return vertices.size();
		}

		AnonHypergraph::ESet* AnonHypergraph::getConnectedEdges(IDType vid)
		{
			if(connectedEdges.count(vid) == 0) {
				return nullptr;
			}
			return &(this->connectedEdges[vid]);
		}

		AnonHypergraph::ESet const* AnonHypergraph::getConnectedEdges(IDType vid) const
		{
			if(connectedEdges.count(vid) == 0) {
				return nullptr;
			}
			return &(this->connectedEdges.at(vid));
		}

		bool AnonHypergraph::deleteEdge(IDType id)
		{
			if(this->edges.count(id) > 0)
			{
				// remove this edge from all vertex connection lists where it occurs
				for(auto it = edges.at(id).cbeginV(); it != edges.at(id).cendV();	++it )
				{
					this->connectedEdges[*it].erase ( id );
				}

				// delete edge
				this->edges.erase ( id );
				return true;
			}
			return false;
		}

		void AnonHypergraph::streamTo(std::ostream& ost) const {
			ost << "AnonHypergraph[name=" << name << ", vertices=[";

			// stream out the nodes
			auto vit = vertices.cbegin();
			if(vit != vertices.cend()) {
				ost << vit->first;
				vit++;
			}
			for (; vit != vertices.cend(); vit++)
			{
				ost << "," << vit->first;
			}
			ost << "], edges=[";

			// stream out the edges
			auto eit = edges.cbegin();
			if(eit != edges.cend()) {
				ost << (eit->second);
				++eit;
			}
			for(;eit != edges.cend(); ++eit) {
				ost << ", " << (eit->second);
			}
			ost << "]]";
		}

		bool AnonHypergraph::deleteVertex(IDType id, unordered_set<IDType>* delEdges)
		{
			if(this->vertices.count(id) > 0)
			{
				// delete all connected edges and return their IDs if desired
				auto cE = this->connectedEdges[id];
				ESet::const_iterator connectit;
				for(connectit = cE.cbegin(); connectit != cE.cend(); connectit++)
				{
					this->deleteEdge(*connectit);
					if(delEdges) {
						delEdges->insert(*connectit);
					}
				}

				// delete the vertex
				this->vertices.erase(id);
				this->connectedEdges.erase(id);
				return true;
			}
			return false;
		}

		bool AnonHypergraph::deleteVertices(vector<IDType> const& ids, unordered_set<IDType>* delEdges) {
			bool result = false;
			for(vector<IDType>::const_iterator iter = ids.begin(); iter != ids.end(); ++iter) {
				result |= deleteVertex(*iter, delEdges);
			}
			return result;
		}

		bool AnonHypergraph::areParallelEdges(IDType id1, IDType id2) const {

			if(edges.count(id1) == 0 || edges.count(id2) == 0) {

				stringstream str;
				str << "Cannot check if edges with IDs '" << id1 << "' and '" << id2 << "' are parallel, ";
				str << "(at least) one of them is not an edge. ";
				str << "The problematic graph was: " << *this;

				throw InvalidInputException(str.str());
			}

			if(edges.at(id1).getLabel().compare(edges.at(id2).getLabel())) {
				return false; // different labels
			}

			Edge::VList const& id1nodes = edges.at(id1).getVertices();
			Edge::VList const& id2nodes = edges.at(id2).getVertices();

			if(id1nodes.size() != id2nodes.size()) {
				logger.newWarning() << "AnonHypergraph::areParallelEdges found edges with the same "
						"label, but a different number of nodes." << endLogMessage;
				return false;
			}

			for(size_t i = 0; i < id1nodes.size(); ++i) {
				if(id1nodes.at(i) != id2nodes.at(i)) {
					return false; // node sequence different
				}
			}

			return true;
		}

	}
}
