/***************************************************************************
 *   Copyright (C) 2015 by Jan Stückrath <jan.stueckrath@uni-due.de>       *
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

#ifndef LEQCHECKSCENARIO_H_
#define LEQCHECKSCENARIO_H_

#include "../basic_types/Scenario.h"
#include "../graphs/Hypergraph.h"
#include "../basic_types/Order.h"

namespace uncover {
	namespace scenarios {

		/**
		 * This Scenario allows to check whether graphs of a given set are represented by another given, upward-closed set.
		 * The caller needs to fix the order, give the upward-closed set of graphs as its minimal elements and another
		 * (finite) set of graphs to be checked. The scenario will then print all found relations, i.e. whenever a
		 * minimal graph is smaller than a graph to be checked, and a total of graphs represented. If desired, the scenario
		 * can terminate after the first found relation.
		 *
		 * <p><b>Note:</b> For a meaningful output, the graphs need to have names. Otherwise this Scenario can only output
		 * the IDs automatically generated when reading the graphs!</p>
		 * @author Jan Stückrath
		 */
		class LEQCheckScenario : public basic_types::Scenario {

			public:

				/**
				 * Creates a new LEQCheckScenario without initializing it.
				 */
				LEQCheckScenario();

				/**
				 * Destroys this instance of LEQCheckScenario.
				 */
				virtual ~LEQCheckScenario();

				// Overwriting
				virtual void run() override;

				// Overwriting
				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * Stores whether this Scenario should terminate after the first related pair of graphs was found.
				 */
				bool faststop;

				/**
				 * Stores the Order used by this Scenario.
				 */
				basic_types::Order* order;

				/**
				 * Stores the set of minimal graphs of an upward closed set.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> minimalGraphs;

				/**
				 * Stores the set of graphs for which it will be checked if they are represented by a graph of minimalGraphs.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> checkGraphs;

		};

	} /* namespace scenarios */
} /* namespace uncover */

#endif /* LEQCHECKSCENARIO_H_ */
