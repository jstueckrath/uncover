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

#ifndef ORDER_H_
#define ORDER_H_

#include "../graphs/Hypergraph.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This class presents a generic ordering relation on Hypergraphs.
		 * @author Jan Stückrath
		 */
		class Order {

			public:

				/**
				 * This function takes two Hypergraph objects and checks whether the first is smaller than or equal to
				 * the second, according to the specific implementation of Order.
				 * @param smaller the Hypergraph which should be smaller
				 * @param bigger the Hypergraph which should be larger
				 * @return true iff the first Hypergraph is smaller than or equal to the second
				 */
				virtual bool isLessOrEq(graphs::Hypergraph const& smaller, graphs::Hypergraph const& bigger) const = 0;

				/**
				 * Destroys this Order object.
				 */
				virtual ~Order() {};

			protected:

				/**
				 * Calling this constructor except from a subclass, will produce problems with the virtual functions.
				 */
				Order() {};

		};

		/**
		 * Alias for a shared pointer to an Order object.
		 */
		typedef shared_ptr<Order> Order_sp;

		/**
		 * Alias for a shared pointer to a constant Order object.
		 */
		typedef shared_ptr<Order const> Order_csp;

	}
}
#endif /* ORDER_H_ */
