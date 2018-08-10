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

#ifndef MATCHERSCENARIO_H_
#define MATCHERSCENARIO_H_

#include "../basic_types/Scenario.h"
#include "../graphs/Hypergraph.h"

namespace uncover {
	namespace scenarios {

		/**
		 * This scenario takes two sets of graphs and computes all matching from one set to the other.
		 * @author Jan Stückrath
		 */
		class MatcherScenario: public basic_types::Scenario {

			public:

				/**
				 * Generates a new MatcherScenario object.
				 */
				MatcherScenario();

				/**
				 * Destroys this MatcherScenario object.
				 */
				virtual ~MatcherScenario();

				/**
				 * Computes all matchings from the source graph set to the target graph set. The resulting matches are
				 * saved as an GTS file.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * Stores the set of all source graphs.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> srcGraphs;

				/**
				 * Stores the set of all target graphs.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> tarGraphs;

				/**
				 * Stores the filename used to save the computed matchings.
				 */
				string resultFilename;

				/**
				 * Stores whether injective or conflict-free matching should be used.
				 */
				bool injective;

		};

	} /* namespace scenarios */
} /* namespace uncover */

#endif /* MATCHERSCENARIO_H_ */
