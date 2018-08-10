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

#ifndef DATA_STRUCTURESVERTEX_H
#define DATA_STRUCTURESVERTEX_H

#include "../basic_types/globals.h"
#include <ostream>

namespace uncover {
	namespace graphs {

		/**
		 * Vertex stores all data of a vertex in a Hypergraph.
		 * @see Hypergraph
		 * @author Jan Stückrath
		 */
		class Vertex
		{

			public:

				/**
				 * Creates a new Vertex with the given ID.
				 */
				Vertex(IDType id);

				/**
				 * Destroys this Vertex.
				 */
				~Vertex();

				/**
				 * Returns the ID of this Vertex.
				 * @return the ID of this Vertex
				 */
				IDType getID() const;

				/**
				 * Streams a short string representation of the given Vertex.
				 * @param ost the stream to which the Vertex ist streamed
				 * @param data the Vertex to be streamed
				 */
				friend std::ostream& operator<< (std::ostream& ost, Vertex const& data);

			protected:

				/**
				 * Stores the ID of this Vertex.
				 */
				IDType id;

		};

	}
}

#endif
