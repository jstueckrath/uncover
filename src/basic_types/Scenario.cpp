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

#ifndef ALGORITHM_SCENARIO_CPP
#define ALGORITHM_SCENARIO_CPP

#include <fstream>
#include <sstream>
#include "Scenario.h"
#include "../logging/message_logging.h"
#include "../scenarios/scenario_ids.h"

using namespace std;
using namespace uncover::basic_types;
using namespace uncover::logging;

namespace uncover {
	namespace basic_types {

		Scenario::Scenario(string name, string desciptor, vector<string> const& alternateNames) : scenarioName(name),
				alternateNames(alternateNames), scenarioDescriptor(desciptor),
				parameterNames(), parameterDescriptor(), synonyms(), wasInitialized(false) {}

		Scenario::~Scenario() {}

		void Scenario::initialize(vector<string>& input) {
			logger.newDebug() << "Starting initialization of scenario '" << this->scenarioName << "'." << endLogMessage;

			if(input.size() > parameterNames.size()) {
				stringstream ss;
				ss << "There are more inputs than parameters the scenario allows.";
				ss << " Scenario was: " << scenarioName << "; Parameters where:";
				for(vector<string>::iterator it = input.begin(); it != input.end(); ++it) {
					ss << " " << *it;
				}
				throw InitialisationException(ss.str());
			}

			// will later contain the result
			unordered_map<string,string> parameters;
			// this loop will parse the inputs and generate a key-value pair for each input
			vector<string>::iterator inputIt = input.begin();
			vector<string>::iterator paramIt = parameterNames.begin();
			while(inputIt != input.end()) {

				size_t pos = inputIt->find_first_of('=');

				// if a key is given, it is used, else the parameter expected at this position is used as key
				string key;
				if(pos == string::npos || pos == 0) {
					key = *paramIt;
				} else {
					key = inputIt->substr(0,pos);
				}
				// changes a possible synonym to the real name or stop if key is unknown
				if(synonyms.count(key) == 0) {
					throw InitialisationException(((string)"The parameter \"") + key + "\" is undefined for this scenario.");
				} else {
					key = synonyms[key];
				}

				// value is the substring behind the first occurrence of '=', or the full string if there is no '='
				string value;
				if(pos == string::npos) {
					value = *inputIt;
				} else if(pos+1 != inputIt->size()) {
					value = inputIt->substr(pos+1,inputIt->size()-pos-1);
				}

				if(parameters.count(key)) {
					throw InitialisationException(((string)"The input \"") + key + ((string)"\" was specified multiple times."));
				}

				parameters[key] = value;
				inputIt++;
				paramIt++;
			}

			// check if all mandatory inputs are given
			unordered_map<string,pair<string,bool>>::iterator checkIt = parameterDescriptor.begin();
			while(checkIt != parameterDescriptor.end()) {
				if(checkIt->second.second && (parameters.count(checkIt->first) == 0)) {
					throw InitialisationException(((string)"Some mandatory inputs are missing, e.g.: ") + checkIt->first);
				}
				checkIt++;
			}

			// This will throw an exception if failing the initialization
			this->initialize(parameters);
			wasInitialized = true;
			logger.newDebug() << "Scenario '" << this->scenarioName << "' initialized successfully." << endLogMessage;
		}

		string Scenario::getName() const {
			return this->scenarioName;
		}

		bool Scenario::isInitialized() const {
			return wasInitialized;
		}

		string Scenario::getScenarioDesciption() const {

			// print overall scenario description
			stringstream ss;
			ss << "Scenario description of '" << this->scenarioName << "':\n---------------------------";
			for(size_t i = 0; i < this->scenarioName.length(); ++i) {
				ss << "-";
			}
			ss << "\n" << scenarioDescriptor;

			// print synonyms of scenario name
			auto nameIt = alternateNames.cbegin();
			if(nameIt != alternateNames.cend()) {
				ss << "\nSynonyms: " << *nameIt;
				++nameIt;
			}
			for(; nameIt != alternateNames.cend(); ++nameIt) {
				ss << ", " << *nameIt;
			}

			// print all available parameters
			if(parameterNames.size()) {
				ss << "\n\nParameter descriptions (in order):\n----------------------------------";
			}
			unsigned int counter = 1;
			for(auto iter = parameterNames.cbegin(); iter != parameterNames.cend(); ++iter, ++counter) {
				auto& p = parameterDescriptor.at(*iter);
				if(counter > 1) {
					ss << endl;
				}
				ss << endl << "(" << counter << ") " << *iter;
				for(auto synIt = synonyms.cbegin(); synIt != synonyms.cend(); ++synIt) {
					if(!synIt->second.compare(*iter) && synIt->first.compare(*iter)) {
						ss << ", " << synIt->first;
					}
				}
				if(p.second) {
					ss << " (required):\n";
				} else {
					ss << ":\n";
				}
				ss << p.first;
			}
			return ss.str();
		}

		void Scenario::addParameter(string paramName, string description, bool mandatory) {
			parameterNames.push_back(paramName);
			parameterDescriptor[paramName] = make_pair(description,mandatory);
			synonyms[paramName] = paramName;
		}

		void Scenario::addSynonymFor(string original, string synonym) {
			synonyms[synonym] = original;
		}

		void Scenario::checkInitializationAndThrow() const {
			if(!wasInitialized) throw RunException("The scenario '" + scenarioName + "' was not initialized before run() was called.");
		}

	}
}

#endif
