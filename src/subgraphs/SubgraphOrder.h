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

#ifndef SUBGRAPHORDER_H_
#define SUBGRAPHORDER_H_

#include "../basic_types/Order.h"
#include "../rule_engine/StdMatchFinder.h"

namespace uncover {
	namespace subgraphs {

		/**
		 * This class implement the "standard" subgraph ordering. A graph G is a subgraph of a
		 * graph H if there is a total injection from G to H.
		 * @author Jan Stückrath
		 */
		class SubgraphOrder: public basic_types::Order {

			public:

				/**
				 * Creates a new SubgraphOrder object.
				 */
				SubgraphOrder();

				/**
				 * Destroys the current instance of this SubgraphOrder.
				 */
				virtual ~SubgraphOrder();

				/**
				 * Checks whether the smaller graph is a subgraph of the larger graph. If both are isomorphic,
				 * the result is also true.
				 * @param smaller the potential subgraph
				 * @param bigger the potential larger graph
				 * @return true, iff the smaller graph is a subgraph of the larger graph
				 */
				virtual bool isLessOrEq(graphs::Hypergraph const& smaller, graphs::Hypergraph const& bigger) const override;

			private:

				/**
				 * This matchfinder is used to find an injective embedding of the smaller graph in the larger graph.
				 */
				rule_engine::StdMatchFinder matchFinder;

		};

	} /* namespace subgraphs */
} /* namespace uncover */

#endif /* SUBGRAPHORDER_H_ */
