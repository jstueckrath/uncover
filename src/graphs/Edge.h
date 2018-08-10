/***************************************************************************
 *   Copyright (C) 2005 SZS                                                *
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

#ifndef DATA_STRUCTURESEDGE_H
#define DATA_STRUCTURESEDGE_H

#include "Vertex.h"

namespace uncover {
	namespace graphs {

		/**
		 * This class represents an edge in a Hypergraph. It can be addressed by an ID, but must be stored
		 * within a Hypergraph, i.e. it cannot be a stand-alone object.
		 * @author Jan Stückrath
		 */
		class Edge
		{

			public:

				/**
				 * Alias for the sequence of vertices to which this edge is attached.
				 */
				typedef vector<IDType> VList;

				/**
				 * Creates a new Edge with the given ID, vertex sequence and label.
				 * @param id the ID of the Edge
				 * @param verts the sequence of vertices to which this edge is attached.
				 * @param label the label of the Edge
				 */
				Edge(IDType id, vector<IDType> const& verts = vector<IDType>(), string label = "");

				/**
				 * Destroys this instance of an Edge.
				 */
				~Edge();

				/**
				 * Returns the ID of this Edge.
				 * @return the ID of this Edge
				 */
				IDType getID() const;

				/**
				 * Returns the label of this Edge.
				 * @return the label of this Edge
				 */
				string getLabel() const;

				/**
				 * Sets the label of this Edge.
				 * @param s the new label
				 */
				void setLabel(string s);

				/**
				 * Returns the number of vertices attached to this Edge. A vertex is counted as often as it is
				 * attached to the edge.
				 * @return the number of vertices attached
				 */
				size_t getArity() const;

				/**
				 * Returns a reference to the sequence of vertices of this Edge.
				 * @return a reference to the sequence of vertices of this Edge.
				 */
				VList& getVertices();

				/**
				 * Returns a reference to the sequence of vertices of this Edge.
				 * @return a reference to the sequence of vertices of this Edge.
				 */
				VList const& getVertices() const;

				/**
				 * Returns an iterator pointing to the first element of the sequence of attached vertices.
				 * @return an iterator pointing to the first element of the sequence of attached vertices
				 */
				VList::iterator beginV();

				/**
				 * Returns an const_iterator pointing to the first element of the sequence of attached vertices.
				 * @return an const_iterator pointing to the first element of the sequence of attached vertices
				 */
				VList::const_iterator beginV() const;

				/**
				 * Returns an const_iterator pointing to the first element of the sequence of attached vertices.
				 * @return an const_iterator pointing to the first element of the sequence of attached vertices
				 */
				VList::const_iterator cbeginV() const;

				/**
				 * Returns an iterator pointing beyond the last element of the sequence of attached vertices.
				 * @return an iterator pointing beyond the last element of the sequence of attached vertices
				 */
				VList::iterator endV();

				/**
				 * Returns an const_iterator pointing beyond the last element of the sequence of attached vertices.
				 * @return an const_iterator pointing beyond the last element of the sequence of attached vertices
				 */
				VList::const_iterator endV() const;

				/**
				 * Returns an const_iterator pointing beyond the last element of the sequence of attached vertices.
				 * @return an const_iterator pointing beyond the last element of the sequence of attached vertices
				 */
				VList::const_iterator cendV() const;

				/**
				 * Streams all data stores in the given Edge to the given stream.
				 * @param ost the ostream to be used
				 * @param data the Edge to be streamed
				 * @return a reference to the given stream
				 */
				friend std::ostream& operator<< (std::ostream& ost, Edge const& data);

			private:

				/**
				 * Stores the ID of this Edge.
				 */
				IDType ID;

				/**
				 * Stores the label of this Edge.
				 */
				string label;

				/**
				 * Stores the sequence of vertices to which this edge is attached.
				 */
				VList vertices;

		};

	}
}

#endif
