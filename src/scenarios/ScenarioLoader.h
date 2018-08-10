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

#ifndef SCENARIOLOADER_H_
#define SCENARIOLOADER_H_

#include "../basic_types/Scenario.h"
#include <ostream>

namespace uncover {
	namespace scenarios {

		/**
		 * This class generates Scenario object from their string identifier (without initializing them).
		 * @author Jan Stückrath
		 * @see basic_types::Scenario
		 */
		class ScenarioLoader {

			public:

				/**
				 * Generates a new ScenarioLoader object. No further initialization is required.
				 */
				ScenarioLoader();

				/**
				 * Destroys this instance of ScenarioLoader.
				 */
				virtual ~ScenarioLoader();

				/**
				 * Generates a Scenario object specified by the given string without initializing it. It is first checked
				 * if the given string is a defined name and then if it is an ID. If neither is the case, nullptr is returned.
				 * @param ident the name or ID of the desired Scenario
				 * @return either a pointer on the desired (uninitialized) Scenario object or nullptr
				 */
				basic_types::Scenario_up getScenario(string ident) const;

				/**
				 * Generates a Scenario object specified by the given name without initializing it.
				 * Returns nullptr if there is no Scenario with the given name.
				 * @param name the name of the desired Scenario
				 * @return either a pointer on the desired (uninitialized) Scenario object or nullptr
				 */
				basic_types::Scenario_up getScenarioByName(string name) const;

				/**
				 * Generates a Scenario object specified by the given ID without initializing it.
				 * Returns nullptr if there is no Scenario with the given ID.
				 * @param id the ID of the desired Scenario
				 * @return either a pointer on the desired (uninitialized) Scenario object or nullptr
				 */
				basic_types::Scenario_up getScenarioByID(unsigned int id) const;

				/**
				 * Print a list of all available scenarios to the given stream using the given separator.
				 * @param ost the stream where the scenario list will be printed
				 * @param separator the separator used between every element of the list
				 */
				void getScenarioList(std::ostream& ost, string separator = ",") const;

			private:

				/**
				 * Fills the scnNameToIDMapping with the appropriate (hard coded) values.
				 */
				void initNameToIDMapping ();

				/**
				 * A mapping from scenario names (long and short) to an unique identifier.
				 */
				unordered_map<string,unsigned int> scnNameToIDMapping;

				/**
				 * A set storing all names which should be displayed first when printing the scenario list.
				 */
				unordered_set<string> firstNames;

		};

	}
} /* namespace loader */

#endif /* SCENARIOLOADER_H_ */
