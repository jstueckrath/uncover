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

#ifdef COMPILE_UNIT_TESTS

#ifndef GRAPH_GENERATOR_H_
#define GRAPH_GENERATOR_H_

#include "../graphs/Rule.h"
#include "../graphs/Hypergraph.h"

namespace uncover {
	namespace test {

		/**
		 * Generates a predefined rule identified by the given index. For each call of this method a
		 * new Rule object is generated.
		 * @param index the number of the rule
		 * @return the predefined rule with the given index
		 */
		extern graphs::Rule_sp getRule(size_t index);

		/**
		 * Generates a predefined graph identified by the given index. For each call of this method a
		 * new Hypergraph object is generated.
		 * @param index the number of the graph
		 * @return the predefined graph with the given index
		 */
		extern graphs::Hypergraph_sp getGraph(size_t index);

	}
}

#endif /* GRAPH_GENERATOR_H_ */

#endif
