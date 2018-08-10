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

#ifndef SCENARIO_H
#define SCENARIO_H

#include "globals.h"
#include "../basic_types/InitialisationException.h"
#include "../basic_types/RunException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * A scenario is a subprogram using a set of input parameters to execute some algorithms, producing desired output.
		 * Input, output and used algorithms are scenario dependent. Scenarios are usually started from the user and may
		 * have arbitrary long execution times.
		 * @author Jan Stückrath
		 */
		class Scenario
		{

			public:

				/**
				 * The destructor of the current Scenario.
				 */
				virtual ~Scenario();

				/**
				 * Initializes the current Scenario with the given vector of parameters. Each element of 'params' must be of the form
				 * 'key=value' and 'key' has to be a defined parameter of the scenario.
				 * @param params a key-value vector of the scenario parameters
				 */
				void initialize(vector<string>& params);

				/**
				 * Executes the scenario. The scenario has to be fully initialized before calling this function.
				 */
				virtual void run() = 0;

				/**
				 * Returns the name of this Scenario, which is a string beginning with 'scn_'.
				 * @return the name of this Scenario
				 */
				string getName() const;

				/**
				 * Check whether the Scenario was already initialized or not.
				 * @return true iff the Scenario was initialized
				 */
				bool isInitialized() const;

				/**
				 * Returns a full description of the given Scenario. The description thereby contains an abstract description
				 * of the Scenarios functionality and a detailed description of all its parameters.
				 * @return a full descrription ot the given Scenario
				 */
				string getScenarioDesciption() const;

			protected:

				/**
				 * Generates an uninitialized Scenario with the given name and call descriptor.
				 * @param name the name of the current scenario
				 * @param desciptor the call descriptor of the current scenario
				 * @param alternateNames a list of all synonyms for this scenario
				 */
				Scenario(string name, string desciptor, vector<string> const& alternateNames = vector<string>());

				/**
				 * Initializes the scenario with the given mapping, where the keys are parameter names. May throw a
				 * {@link uncover::basic_types::InitialisationException} if parameters are invalid.
				 * @param parameters a mapping containing parameter names as keys and their values
				 */
				virtual void initialize(unordered_map<string,string>& parameters) = 0;

				/**
				 * Defined a new input parameter with the name 'paramName' the description 'description'. The boolean specifies whether
				 * the parameter is mandatory or not. The order in which the parameters are defined using this method also specifies
				 * the order in which the parameters are expected if this scenario is called without parameter names, i.e. if the i-th
				 * input has only a value and no name, the name given at the i-th call of this method is used.
				 * @param paramName a unique name for this parameter
				 * @param description a description of the defines parameter (for documentary purposes)
				 * @param mandatory true if and only if the parameter should be mandatory
				 */
				void addParameter(string paramName, string description, bool mandatory = true);

				/**
				 * Stores that the parameter 'original' is also meant if 'synonym' was given as parameter.
				 * Using a defined parameter as 'synonym' will lead to undefined behavior.
				 * @param original an already defined parameter name
				 * @param synonym a name which will from now on be handled as if 'original' was given
				 */
				void addSynonymFor(string original, string synonym);

				/**
				 * Check if the Scenario was initialized and throws a RunException if it was not.
				 * @see exceptions::RunException
				 */
				void checkInitializationAndThrow() const;

				/**
				 * A unique name of this scenario starting with "scn_".
				 */
				const string scenarioName;

				/**
				 * A vector containing all names for this Scenario, beyond scenarioName.
				 */
				const vector<string> alternateNames;

				/**
				 * Contains a description of how a call of this scenario may look like.
				 * This does not include detailed description of the parameters.
				 */
				const string scenarioDescriptor;

			private:

				/**
				 * Contains the names of all parameters of the Scenario. The position of each name specifies the name of nameless parameter
				 * (if the name of the ith is not given, the name is parameterNames[i]).
				 */
				vector<string> parameterNames;

				/**
				 * Stores detailed description of every parameter of this Scenario and whether the parameter is mandatory or not.
				 */
				unordered_map<string,pair<string,bool>> parameterDescriptor;

				/**
				 * Stores all synonyms for parameters.
				 */
				unordered_map<string,string> synonyms;

				/**
				 * Stores whether the Scenario was initialized or not.
				 */
				bool wasInitialized;

		};

		/**
		 * Alias of a unique pointer to a Scenario object.
		 */
		typedef unique_ptr<Scenario> Scenario_up;

	}
}

#endif
