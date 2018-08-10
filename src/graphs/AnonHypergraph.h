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

#ifndef ANONHYPERGRAPH_H_
#define ANONHYPERGRAPH_H_

#include "../basic_types/globals.h"
#include "Vertex.h"
#include "Edge.h"
#include "../basic_types/InitialisationException.h"
#include "../basic_types/InvalidInputException.h"
#include "../basic_types/Streamable.h"

namespace uncover {
	namespace graphs {

		/**
		 * An AnonHypergraph is a data structure storing a hypergraph. It stores a name, a collection of vertices
		 * and a collection of edges connected to these vertices. Furthermore, it provides a large collection of
		 * functions to manipulate the graph structure, ranging from add- and delete-function to iterators. The
		 * Hypergraph class extends AnonHypergraph by additionally providing an unique identifier for the graph.
		 * Note that this class can be big; the use of pointers is recommended.
		 * @see Hypergraph
		 * @author Jan Stückrath
		 */
		class AnonHypergraph : public basic_types::Streamable {

			public:

				/**
				 * Alias for a vertex map returned by this class. The keys are IDs and the values are vertices.
				 */
				// Tests have shown better performance when using map instead of unordered_map.
				typedef map<IDType, Vertex> VMap;

				/**
				 * Alias for an edge map returned by this class. The keys are IDs and the values are edges.
				 */
				// Tests have shown better performance when using map instead of unordered_map.
				typedef map<IDType, Edge> EMap;

				/**
				 * Alias for a set of edge IDs connected to a vertex. An ID may occur more than once if an edge
				 * is connected to the same vertex more than once.
				 */
				typedef unordered_multiset<IDType> ESet;

				/**
				 * Generates a new graph with the given name and an empty collection of vertices and edges.
				 * @param name a name for the graph, not necessarily unique
				 */
				AnonHypergraph(std::string name = "");

				/**
				 * Generates a new graph with data copied from the given graph. The new graph is an independent
				 * copy of the given graph.
				 * @param graph the graph to be copied
				 */
				AnonHypergraph(const AnonHypergraph& graph);

				/**
				 * Deletes this graph, freeing all its memory.
				 */
				virtual ~AnonHypergraph();

				/**
				 * Returns the name of this graph.
				 * @return the name of this graph
				 */
				string getName() const;

				/**
				 * Sets the name of this graph to the given string.
				 * @param s the new name of this graph
				 */
				void setName(string s);

				/**
				 * Prints a string representation of this graph to the given stream.
				 * @param ost the stream to which the graph will be printed
				 */
				virtual void streamTo(std::ostream& ost) const override;

				/////////////////////////////////////////////////////////////////////////////////////////
				// Vertex manipulation
				/////////////////////////////////////////////////////////////////////////////////////////

				/**
				 * Adds a new vertex to this graph and returns the ID of the added vertex.
				 * @return the ID of the added vertex
				 */
				IDType addVertex();

				/**
				 * Returns true if and only if this graph has a vertex with the given ID.
				 * @param id the ID to be checked
				 * @return true, iff this graph has a vertex with the given ID
				 */
				bool hasVertex(IDType id) const;

				/**
				 * Deletes the vertex with the given ID if it exists in this graph. All edges incident to the deleted
				 * vertex will be deleted as well. Optionally a set can be given, in which the IDs of all deleted
				 * edges will be stored.
				 * @param id the ID of the vertex to be deleted
				 * @param delEdges a set in which all IDs of deleted edges will be stored; use null if the IDs should
				 * 				not be stored
				 * @return true, iff a vertex was deleted
				 */
				bool deleteVertex(IDType id, unordered_set<IDType>* delEdges = nullptr);

				/**
				 * All vertices with IDs in the given vector will be deleted, also deleting all edges incident to them.
				 * If there is an ID for which there is no vertex in the graph, the ID is simply ignored. Optionally a
				 * set can be given, in which the IDs of all deleted edges will be stored.
				 * @param ids a vector of all vertices which should be deleted
				 * @param delEdges a set in which all IDs of deleted edges will be stored; use null if the IDs should
				 * 				not be stored
				 * @return true, iff at least one vertex was deleted
				 */
				bool deleteVertices(vector<IDType> const& ids, unordered_set<IDType>* delEdges = nullptr);

				/**
				 * Returns the number of vertices of this graph.
				 * @return the number of vertices of this graph
				 */
				size_t getVertexCount() const;

				/**
				 * Returns an iterator to the first entry in the vertex map. No order on the entries is guaranteed.
				 * @return an iterator to the first entry in the vertex map
				 */
				AnonHypergraph::VMap::iterator beginVertices() noexcept;

				/**
				 * Returns a const_iterator to the first entry in the vertex map. No order on the entries is guaranteed.
				 * @return a const_iterator to the first entry in the vertex map
				 */
				AnonHypergraph::VMap::const_iterator beginVertices() const noexcept;

				/**
				 * Returns a const_iterator to the first entry in the vertex map. No order on the entries is guaranteed.
				 * @return a const_iterator to the first entry in the vertex map
				 */
				AnonHypergraph::VMap::const_iterator cbeginVertices() const noexcept;

				/**
				 * Returns an iterator pointing beyond the last entry in the vertex map. No order on the entries is
				 * guaranteed.
				 * @return an iterator pointing beyond the last entry in the vertex map
				 */
				AnonHypergraph::VMap::iterator endVertices() noexcept;

				/**
				 * Returns a const_iterator pointing beyond the last entry in the vertex map. No order on the entries
				 * is guaranteed.
				 * @return a const_iterator pointing beyond the last entry in the vertex map
				 */
				AnonHypergraph::VMap::const_iterator endVertices() const noexcept;

				/**
				 * Returns a const_iterator pointing beyond the last entry in the vertex map. No order on the entries
				 * is guaranteed.
				 * @return a const_iterator pointing beyond the last entry in the vertex map
				 */
				AnonHypergraph::VMap::const_iterator cendVertices() const noexcept;

				/**
				 * Returns the set of IDs of edges incident to the vertex with the given ID. If there is no such
				 * vertex, null is returned. The returned set contains every ID as often as it is connected to the
				 * vertex.
				 * @param vid the ID of the vertex of which the incident edges should be returned
				 * @return the set of edge IDs incident to the given vertex; null if there is no vertex with the given ID
				 */
				AnonHypergraph::ESet* getConnectedEdges(IDType vid);

				/**
				 * Returns the set of IDs of edges incident to the vertex with the given ID. If there is no such
				 * vertex, null is returned. The returned set contains every ID as often as it is connected to the
				 * vertex.
				 * @param vid the ID of the vertex of which the incident edges should be returned
				 * @return the set of edge IDs incident to the given vertex; null if there is no vertex with the given ID
				 */
				AnonHypergraph::ESet const* getConnectedEdges(IDType vid) const;

				/**
				 * Returns the number of edges connected to the vertex with the given ID. If there is no vertex with the
				 * given ID, then -1 is returned.
				 * @param vid the vertex ID of which the number of incident edges should be returned
				 * @return the number of incident edges; -1 if the is no vertex with the given ID
				 */
				int getConnEdgesCount(IDType vid) const;

				/**
				 * Merges the vertices with the given IDs, such that all edges incident to one of the vertices will be
				 * incident to the merging. If one of the given IDs is not a vertex, an InvalidInputException is thrown.
				 * The ID of the new (merged) vertex is returned.
				 * @param id1 the first vertex ID to be merged
				 * @param id2 the second vertex ID to be merged
				 * @return the ID of the new (merged) vertex
				 * @throws InvalidInputException if one of the given IDs is not a vertex
				 */
				IDType mergeVertices(IDType id1, IDType id2);

				/**
				 * Same as mergeVertices(IDType,IDType), but it is guaranteed that the ID of the merged vertex will be
				 * the ID of the first vertex, i.e. the second vertex will be merged into the first one.
				 * @param id1 the ID of the vertex to which the other vertex is merged
				 * @param id2 the ID of the vertex which is merged into the other vertex
				 */
				void mergeVertexIntoFirst(IDType id1, IDType id2);

				/**
				 * Merges all vertices in the given vector into one vertex. If an ID in the vector is not a valid
				 * vertex ID, it is ignored, but if there are only invalid IDs, an exception is thrown.
				 * @param ids the vector of IDs of vertices which will be merged
				 * @return the new ID of the merged vertex
				 * @throws InvalidInputException if all IDs in the given vector are non-valid vertex IDs
				 */
				IDType mergeVertices(unordered_set<IDType> const& ids);

				/////////////////////////////////////////////////////////////////////////////////////////
				// Edge manipulation
				/////////////////////////////////////////////////////////////////////////////////////////

				/**
				 * Returns true, iff this graph has an edge with the given ID.
				 * @param id the ID to be checked
				 * @return true, iff this graph has an edge with the given ID
				 */
				bool hasEdge(IDType id) const;

				/**
				 * Adds an edge with the given label and incident vertices to this graph. If an ID in the vector
				 * of incident vertices is not a vertex in this graph, an exception is thrown.
				 * @param label the label of the new edge
				 * @param attVertices the sequence of incident vertices; order is important; if not given, an edge
				 * 				with no incident vertices is added
				 * @return the ID of the new edge
				 * @throws InvalidInputException if an incident ID is no vertex in this graph
				 */
				IDType addEdge(string label, vector<IDType> const& attVertices = vector<IDType>());

				/**
				 * Deleted the edge with the given ID, if it exists (ignored if it does not exist).
				 * @param id the ID of the edge to be deleted
				 * @return true, iff an edge was deleted
				 */
				bool deleteEdge(IDType id);

				/**
				 * Returns the sequence of vertices incident to the edge with the given ID. If there is no edge
				 * with the given ID, null is returned.
				 * @param edgeID the edge for which the sequence of incident vertices should be returned
				 * @return the sequence of vertices incident to the edge with the given ID; null if no such edge esists
				 */
				vector<IDType>* getVerticesOfEdge(IDType edgeID);

				/**
				 * Returns the sequence of vertices incident to the edge with the given ID. If there is no edge
				 * with the given ID, null is returned.
				 * @param edgeID the edge for which the sequence of incident vertices should be returned
				 * @return the sequence of vertices incident to the edge with the given ID; null if no such edge esists
				 */
				vector<IDType> const* getVerticesOfEdge(IDType edgeID) const;

				/**
				 * Returns the edge with the given ID if it exists. Returns null, if it does not exist.
				 * @param id the ID of the edge
				 * @return the edge with the given ID or null if no such edge exists
				 */
				Edge* getEdge(IDType id);

				/**
				 * Returns the edge with the given ID if it exists. Returns null, if it does not exist.
				 * @param id the ID of the edge
				 * @return the edge with the given ID or null if no such edge exists
				 */
				Edge const* getEdge(IDType id) const;

				/**
				 * Returns the arity of the edge with the given ID, i.e. the number of vertices to which it is
				 * incident. A vertex to which the edge is incident multiple times, is counted multiple times.
				 * If no edge with the given ID exists, -1 is returned.
				 * @param id the ID of the edge to be checked
				 * @return the arity of the edge or -1 if no edge with the given ID exists
				 */
				int getEdgeArity(IDType id) const;

				/**
				 * Returns the number of edges of this graph.
				 * @return the number of edges of this graph
				 */
				size_t getEdgeCount() const;

				/**
				 * Returns an iterator to the first entry in the edge map. No order on the entries is guaranteed.
				 * @return an iterator to the first entry in the edge map
				 */
				AnonHypergraph::EMap::iterator beginEdges() noexcept;

				/**
				 * Returns a const_iterator to the first entry in the edge map. No order on the entries is guaranteed.
				 * @return a const_iterator to the first entry in the edge map
				 */
				AnonHypergraph::EMap::const_iterator beginEdges() const noexcept;

				/**
				 * Returns a const_iterator to the first entry in the edge map. No order on the entries is guaranteed.
				 * @return a const_iterator to the first entry in the edge map
				 */
				AnonHypergraph::EMap::const_iterator cbeginEdges() const noexcept;

				/**
				 * Returns an iterator pointing beyond the last entry in the edge map. No order on the entries is
				 * guaranteed.
				 * @return an iterator pointing beyond the last entry in the edge map
				 */
				AnonHypergraph::EMap::iterator endEdges() noexcept;

				/**
				 * Returns a const_iterator pointing beyond the last entry in the edge map. No order on the entries is
				 * guaranteed.
				 * @return a const_iterator pointing beyond the last entry in the edge map
				 */
				AnonHypergraph::EMap::const_iterator endEdges() const noexcept;

				/**
				 * Returns a const_iterator pointing beyond the last entry in the edge map. No order on the entries is
				 * guaranteed.
				 * @return a const_iterator pointing beyond the last entry in the edge map
				 */
				AnonHypergraph::EMap::const_iterator cendEdges() const noexcept;

				/**
				 * Merges the edges with the given IDs. This automatically merges the i-th vertex incident to both edges,
				 * for every i up to the arity. An exception is thrown if the IDs do not represent two edges, the label differ
				 * or the arity differs.
				 * @param id1 ID of an edge to be merged
				 * @param id2 ID of an edge to be merged
				 * @return the ID of the new (merged) edge
				 * @throws InvalidInputException if one of the IDs does not represent an edge or the edges differ in label or arity
				 */
				IDType mergeEdges(IDType id1, IDType id2);

				/**
				 * Merges all edges in the given vector into one edge. If an ID in the vector is not a valid
				 * edge ID, it is ignored, but if there are only invalid IDs, an exception is thrown.
				 * @param ids the vector of IDs of edges which will be merged
				 * @return the new ID of the merged edge
				 * @throws InvalidInputException if all IDs in the given vector are non-valid edge IDs
				 */
				IDType mergeEdges(unordered_set<IDType> const& ids);

				/**
				 * Returns true if the given IDs are parallel edges, i.e. they have the same label, the same arity and the
				 * same sequence of vertices incident to them. Throws an exception if one of the IDs is not an edge.
				 * @param id1 ID of an edge
				 * @param id2 ID of an edge
				 * @return true, iff the edges with the given IDs are parallel
				 * @throws InvalidInputException if one of the IDs is not an edge
				 */
				bool areParallelEdges(IDType id1, IDType id2) const;

			protected:

				/**
				 * Stores the last ID assigned to a vertex or edge.
				 */
				IDType lastUsedID;

				/**
				 * Stores the name of this graph
				 */
				string name;

				/**
				 * Stores a map of all vertices of this graph, where the key is their ID.
				 */
				AnonHypergraph::VMap vertices;

				/**
				 * Stores a map of all edges of this graph, where the key is their ID.
				 */
				AnonHypergraph::EMap edges;

				/**
				 * Maps every vertex ID to a set containing all IDs of edges incident to them. An edge is in the set
				 * as often as it is incident to the vertex.
				 */
				unordered_map<IDType, AnonHypergraph::ESet> connectedEdges;

		};

		/**
		 * Alias of a shared pointer to an AnonHypergraph.
		 */
		typedef shared_ptr<AnonHypergraph> AnonHypergraph_sp;

		/**
		 * Alias of a shared pointer to a constant AnonHypergraph.
		 */
		typedef shared_ptr<const AnonHypergraph> AnonHypergraph_csp;

	} /* namespace graphs */
} /* namespace uncover */

#endif /* ANONHYPERGRAPH_H_ */
