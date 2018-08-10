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

#ifndef COMPILE_UNIT_TESTS

#include <string.h>
#include <sstream>
#include <iostream>
#include "basic_types/Scenario.h"
#include "scenarios/ScenarioLoader.h"
#include "logging/message_logging.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "basic_types/Timer.h"

#define ERROR_INIT_TEXT "Error: "
#define WARN_INIT_TEXT "Warning: "
#define UNCOVER_VERSION "1.0.5"
#define NO_ERROR 0
#define ERR_UNKNOWN_PARAMETER 1
#define ERR_NO_SCENARIO 2
#define ERR_SCENARIO_UNKNOWN 3
#define ERR_SCENARIO_INIT 4
#define ERR_SCENARIO_RUN 5

using namespace std;
using namespace boost::program_options;
using namespace uncover::logging;
using namespace uncover::basic_types;
using namespace uncover::scenarios;

// initializing the message logger to be cout
MessageLogger uncover::logging::logger(cout);

/**
 * Formats the given quantity of seconds as: w day(s) x hour(s) y minute(s) z second(s)
 * @param seconds the duration of the time span
 * @return a string representing the time span in a formatted matter
 */
string formatTimeSpan(double seconds) {
	stringstream ss;
	int time = (int)seconds; // an accuracy on one second is enough
	ss << (time / (60*60*24)) << " day(s) ";
	time = time % (60*60*24);
	ss << (time / (60*60)) << " hour(s) ";
	time = time % (60*60);
	ss << (time / 60) << " minute(s) ";
	time = time % 60;
	ss << time << " second(s)";
	return ss.str();
}

/**
 * Splits the given string at every occurrence of a space. Every so obtained substring is appended to the given
 * vector in the order of occurrence in str, i.e. the order on the substrings is equal in str and vec. Furthermore,
 * only substrings with at least one symbol are added, i.e. multiple spaces are treated as if they would be only one.
 * @param str the string to be split
 * @param vec the vector to which the substrings will be appended
 */
void tokenizeAndAddNE(string const& str, vector<string>& vec) {
	size_t oldindex = 0;
	size_t newindex = 0;
	while(true) {
		newindex = str.find(" ",oldindex);
		if(newindex >= str.size()) {
			break;
		}
		string newstring = str.substr(oldindex,newindex-oldindex);
		if(newstring.size() > 0) {
			vec.push_back(newstring);
		}
		oldindex = newindex+1;
	}

	string newstring = str.substr(oldindex);
	if(newstring.size() > 0) {
		vec.push_back(newstring);
	}
}

/**
 * Encapsulates a timer, which is started when the TerminationMessage is created. When the TerminationMessage
 * is destroyed, a log message is generated that the scenario has ended, printing also the elapsed time.
 * @see Timer
 * @author Jan Stückrath
 */
class TerminationMessage {

	public:

		/**
		 * Creates a new TerminationMessage also staring the encapsulated Timer.
		 */
		TerminationMessage() : timer() {
			timer.start();
		}

		/**
		 * Destroys this TerminationMessage. This also prints a log message stating that the scenario has terminated and
		 * printing the elapsed time.
		 */
		~TerminationMessage() {
			logger.newUserInfo() << "Scenario terminated after " << formatTimeSpan(timer.getElapsedTime()) << "." << endLogMessage;
		}

	private:

		/**
		 * Stores a Timer to compute the elapsed time.
		 */
		Timer timer;

};

/**
 * The main function of the program. Processes the command line and config-file parameters and start the
 * chosen scenario.
 * @param argc the number of command line parameters
 * @param argv the command line parameters
 */
int main ( int argc, char** argv )
{

	// Declare the supported options.
	options_description desc("Possible options");
	desc.add_options()
	    		("help,h", "Displays a description of all defined parameters.")
	    		("scn,s", value<vector<string> >()->composing()->multitoken(),
	    				"Specifies the scenario which will be started. Expects a list of strings dependent on the chosen scenario. "
	    				"First string has to be the scenario name and the following are the scenario parameters. "
	    				"Available scenario can be printed via [--scenarios] and their scenario descriptions can be printed "
	    				"with [--scenario-usage].")
	    		("output-level,o", value<int>(),
	    				"Use this option to set the quantity of output. Values can vary from -1 (no output) "
	    				"to 4 (debug output) where 0 is normal user output.")
	    		("scenarios,l", "Lists all the scenarios currently provided by this analysis tool.")
	    		("scenario-usage,u", value<string>(), "Prints out a usage description for the given scenario. The argument may either be "
	    				"the scenarios name, a synonym or its ID.")
	    		("scenario-file,f", value<string>(),
	    				"Starts a scenario from a given configuration file. This must be a text file, where every line contains a "
	    				"key-value pair (separated by '='). The keys have to be the scenario parameters and must contain all "
	    				"necessary parameters. Additionally one key has to be 'scenario' with the scenario name as value. The "
	    				"order of the pairs is arbitrary, but the keys may not be omitted.")
	    		("version,v", "Displays the version number as well as copyright and license information.");

	// ******************** parsing the arguments ********************
	string scenario("");
	vector<string> scenario_input;

	try {
		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);

		// if a file is used, the value of 'scn' will falsely be given as string instead of vector<string>
		bool usedScnFile = false;
		if(vm.count("scenario-file")) {
			store(parse_config_file<char>(vm["scenario-file"].as<string>().c_str(),desc),vm);
			usedScnFile = true;
		}
		notify(vm);

		// print help description
		if(vm.count("help")) {
			cout << "UnCoVer (Using Coverability for Verification):" << endl;
			cout << "----------------------------------------------" << endl;
			cout << "This program executes a predefined scenario. The scenario parameters can be either given directly on the console "
					"(--scn) or by giving a configuration file (--scenario-file). Only one of these options may be used. See the option "
					"list below for listing available scenarios or their description and further configuration." << endl << endl;
			cout << desc << endl;
			return NO_ERROR;
		}

		// print version information
		if(vm.count("version")) {
			cout << "UnCoVer (Using coverability for Verification) version " << UNCOVER_VERSION << endl;
	    cout << "Copyright (C) 2014-2015 Jan Stückrath" << endl;
	    cout << "License GPLv2: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>" << endl;
	    cout << "UnCoVer is free software and comes with ABSOLUTELY NO WARRANTY. You are welcome to change and redistribute it." << endl;
			return NO_ERROR;
		}

		// print list of all defined scenarios
		if(vm.count("scenarios")) {
			ScenarioLoader scnLoader;
			scnLoader.getScenarioList(cout,"\n");
			cout << endl;
			return NO_ERROR;
		}

		// print a scenario description for the given scenario
		if(vm.count("scenario-usage")) {
			string scenario = vm["scenario-usage"].as<string>();
			ScenarioLoader scnLoader;
			Scenario_up scn = scnLoader.getScenario(scenario);
			if(scn) {
				cout << scn->getScenarioDesciption() << endl;
				return NO_ERROR;
			} else {
				cout << ERROR_INIT_TEXT << "No usage defined for the given scenario. The scenario is unknown." << endl;
				cout << "See --scenarios (or -l) for a list of all available scenarios." << endl;
				return ERR_SCENARIO_UNKNOWN;
			}
		}

		// set log level
		if(vm.count("output-level")) {
			int oLevel = vm["output-level"].as<int>();
			if(oLevel < -1 || oLevel > 4) {
				cout << WARN_INIT_TEXT << "Invalid output level '" << oLevel << "'. Using default." << endLogMessage;
			} else {
				gl_LogLevel = LogTypes(oLevel);
			}
		}

		// read parameters and scenario from console
		if(vm.count("scn")) {
			scenario_input = vm["scn"].as<vector<string>>();

			// if true, scenario_input probably has size 1 and everything is stored in one string instead of a vector of strings
			if(usedScnFile) {
				vector<string> newInput;
				for(size_t i = 0; i < scenario_input.size(); ++i) {
					tokenizeAndAddNE(scenario_input.at(i),newInput);
				}
				scenario_input = newInput;
			}

			// if this is not true, an error will be thrown later
			if(scenario_input.size()) {
				scenario = *(scenario_input.begin());
				scenario_input.erase(scenario_input.begin());
			}
		}

	} catch(boost::program_options::error& e)
  {
    cout << ERROR_INIT_TEXT << "An error occurred while parsing parameters: " << e.what() << endl;
    cout << "See --help (or -h) for a usage description." << endl;
    return ERR_UNKNOWN_PARAMETER;
  }

	// ******************** loading scenario ********************

	if(!scenario.compare("")) {
		cout << ERROR_INIT_TEXT << "No Scenario given! See --scenarios (or -l) for a list of all available scenarios, ";
		cout << "or see --help (or -h) for a usage description." << endl;
		return ERR_NO_SCENARIO;
	}

	ScenarioLoader scnLoader;
	Scenario_up scn = scnLoader.getScenario(scenario);

	if(scn==nullptr) {
		cout << ERROR_INIT_TEXT << "The given scenario '" << scenario;
		cout << "' could not be loaded. No such scenario exists." << endl;
		cout << " See --scenarios (or -l) for a list of all available scenarios." << endl;
		return ERR_SCENARIO_UNKNOWN;
	}

	// *********************************************************************************************
	// finished parsing command line parameters, starting analysis *********************************
	// *********************************************************************************************

	TerminationMessage message;

	// ******************** initializing scenario ********************
	try {
		scn->initialize(scenario_input);
	} catch (InitialisationException& ex) {
		logger.newError() << ex << endLogMessage;
		return ERR_SCENARIO_INIT;
	}

	// ******************** running scenario ********************

  logger.newUserInfo() << "Running scenario: " << scn->getName() << endLogMessage;
  try {
  	scn->run();
  } catch (RunException& ex) {
  	logger.newError() << ex << endLogMessage;
  	return ERR_SCENARIO_RUN;
  }

	return NO_ERROR;
}

#endif
