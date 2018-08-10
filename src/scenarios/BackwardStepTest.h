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

#ifndef BACKWARDSTEPTEST_H_
#define BACKWARDSTEPTEST_H_

#include "../basic_types/Scenario.h"
#include "../graphs/GTS.h"

namespace uncover {
	namespace scenarios {

		/**
		 * This test scenario takes a given set of rules and a given set of error graphs and performs a single backward
		 * step, including rule preparation.
		 * @author Jan Stückrath
		 */
		class BackwardStepTest: public basic_types::Scenario {

			public:

				/**
				 * Generates a new BackwardStepTest scenario without initializing its parameters.
				 */
				BackwardStepTest();

				/**
				 * Destroys this BackwardStepTest scenario.
				 */
				virtual ~BackwardStepTest();

				/**
				 * Performs a single backward step with the rules and graphs with which this object was initialized.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * This enumeration stores which order should be used.
				 */
				enum Order {
					/** The minor ordering. */
					MINOR,
					/** The subgraph ordering. */
					SUBGRAPH};

				/**
				 * Stores the path and filename of the rules read.
				 */
				string rulesFilename;

				/**
				 * Stores the loaded GTS.
				 */
				graphs::GTS_sp gts;

				/**
				 * Stores the loaded error graphs.
				 */
				shared_ptr<vector<graphs::Hypergraph_sp>> errorGraphs;

				/**
				 * Stores whether optimization will be used or not.
				 */
				bool optimize;

				/**
				 * Stores whether matches are injective or conflict-free.
				 */
				bool injective;

				/**
				 * Stores the order used.
				 */
				BackwardStepTest::Order order;

		};

	} /* namespace scenarios */
} /* namespace uncover */

#endif /* BACKWARDSTEPTEST_H_ */
