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

#ifndef DATA_STRUCTURESHYPERGRAPH_H
#define DATA_STRUCTURESHYPERGRAPH_H

#include "AnonHypergraph.h"
#include "../basic_types/Streamable.h"

namespace uncover {
	namespace graphs {

		/**
		 * A Hypergraph represents a graph with a unique identifier. It extends the AnonHypergraph implementation.
		 * Note that -- just like AnonHypergraph -- this class can be big; the use of pointers is recommended.
		 * @see AnonHypergraph
		 * @author Jan Stückrath
		 */
		class Hypergraph : public AnonHypergraph
		{

			public:

				/**
				 * Generates a new Hypergraph with the given name and without vertices or edges.
				 * @param name the name of the new graph
				 */
				Hypergraph(std::string name = "");

				/**
				 * Generates a new Hypergraph copying the data of the given AnonHypergraph. The new Hypergraph is an
				 * independent copy of the old graph.
				 * @param graph the AnonHypergraph from which the data will be copied
				 */
				Hypergraph(const AnonHypergraph& graph);

				/**
				 * Generates a new Hypergraph copying the data of the given Hypergraph. The new Hypergraph is an
				 * independent copy of the old graph.
				 * @param graph the Hypergraph from which the data will be copied
				 */
				Hypergraph(const Hypergraph& graph);

				/**
				 * Deletes this Hypergraph, freeing all memory.
				 */
				virtual ~Hypergraph();

				/**
				 * Returns the (unique) ID of this graph.
				 * @return the (unique) ID of this graph
				 */
				IDType getID() const;

				virtual void streamTo(std::ostream& ost) const override;

			protected:

				/**
				 * Stores the (unique) iD of this graph.
				 */
				IDType const id;

		};

		/**
		 * Alias for a shared pointer to a Hypergraph.
		 */
		typedef shared_ptr<Hypergraph> Hypergraph_sp;

		/**
		 * Alias for a shared pointer to a constant Hypergraph.
		 */
		typedef shared_ptr<const Hypergraph> Hypergraph_csp;

	}
}

#endif
