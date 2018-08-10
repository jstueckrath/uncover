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

#include "LEQCheckScenario.h"
#include "scenario_ids.h"
#include "../minors/MinorOrder.h"
#include "../subgraphs/SubgraphOrder.h"
#include "../basic_types/InitialisationException.h"
#include "../io/GTXLReader.h"
#include "../io/XMLioException.h"
#include "../logging/message_logging.h"
#include <sstream>

using namespace uncover::basic_types;
using namespace uncover::graphs;
using namespace uncover::io;
using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace scenarios {

		LEQCheckScenario::LEQCheckScenario() : Scenario(SCN_LEQ_CHECK_NAME,
				"Checks if a graphs of a given set are represented by given upward-closed class of graphs. The first "
				"parameter defines the order, the second parameter must specify the minimal elements of an upward-closed "
				"class of graphs, and the third parameter is the set of graphs to be checked. More precisely this "
				"scenario will check if there exists a graph G in the first set which is smaller or equal to a graph H in "
				"the second set. If 'faststop' is set to false, then the scenario will not stop after the first H found. "
				"If the involved graphs have names, these will be printed if a related pair or graphs was found.",
				{SCN_LEQ_CHECK_SHORT_1}), faststop(false), order(nullptr), minimalGraphs(nullptr), checkGraphs(nullptr) {

			this->addParameter("order","Sets the order used for the check. May be 'minor' or 'subgraph'.",true);
			this->addSynonymFor("order", "o");

			this->addParameter("upclosedset", "Filename of a GXL file. The stored graphs will be interpreted as the minimal "
					"elements of an upward-closed class of graphs.",true);
			this->addSynonymFor("upclosedset","u");

			this->addParameter("checkset", "Filename of a GXL file. For each stored graph it will be checked if it is "
					"represented by the other given set of graphs.", true);
			this->addSynonymFor("checkset","c");

			this->addParameter("faststop", "May be 'true' or 'false' (default). If 'false', this check will be performed for "
					"all graphs of the checkset. If 'true', the scenario will terminate after the first related pair was found.", false);
			this->addSynonymFor("faststop","s");

		}

		LEQCheckScenario::~LEQCheckScenario() {
			if(order) delete order;
		}

		void LEQCheckScenario::initialize(unordered_map<string, string>& parameters) {

			// initialize order
			if(!parameters["order"].compare("minor")) {
				order = new minors::MinorOrder();
			} else if(!parameters["order"].compare("subgraph")) {
				order = new subgraphs::SubgraphOrder();
			} else {
				stringstream ss;
				ss << "Failed to recognize the given order '" << parameters["order"] << "'. Available options are: minor, subgraph";
				throw InitialisationException(ss.str());
			}

			unique_ptr<io::GTXLReader> reader = io::GTXLReader::newUniqueReader();

			// read upward-closed graphs from file
			try {
				minimalGraphs = reader->readGXL(parameters["upclosedset"]);
			} catch (XMLioException& ex) {
				stringstream ss;
				ss << "Failed to load minimal graphs of the upward-closed set: " << ex.what();
				throw InitialisationException(ss.str());
			}

			// read graphs to be checked from file
			try {
				checkGraphs = reader->readGXL(parameters["checkset"]);
			} catch (XMLioException& ex) {
				stringstream ss;
				ss << "Failed to load graphs set to be checked: " << ex.what();
				throw InitialisationException(ss.str());
			}

			// read optional parameter faststop
			if(parameters.count("faststop")) {
				if(!parameters["faststop"].compare("true")) {
					faststop = true;
				} else if(!parameters["faststop"].compare("false")) {
					faststop = true;
				} else {
					logger.newWarning() << "Unrecognized value '" << parameters["faststop"] << "' for parameter faststop. ";
					logger.contMsg() << "Using default ('false')." << endLogMessage;
					faststop = false;
				}
			} else {
				faststop = false;
			}

		}

		void LEQCheckScenario::run() {

			size_t count = 0;
			size_t graphCount = 0;
			bool newGraph = true;
			for(auto graphToCheck : *checkGraphs) {

				newGraph = true;

				for(auto miniGraph : *minimalGraphs) {

					if(order->isLessOrEq(*miniGraph, *graphToCheck)) {

						++count;
						if(newGraph) {
							++graphCount;
							newGraph = false;
						}

						if(!miniGraph->getName().empty() && !graphToCheck->getName().empty()) {
							logger.newUserInfo() << "The graph '" << graphToCheck->getName() << "' has found to be represented ";
							logger.contMsg() << "by the minimal graph '" << miniGraph->getName() << "'." << endLogMessage;
						} else if(!miniGraph->getName().empty()) {
							logger.newUserInfo() << "An unnamed graph (id=" << graphToCheck->getID() << ") was represented by ";
							logger.contMsg() << "the minimal graph '" << miniGraph->getName() << "'." << endLogMessage;
						} else if(!graphToCheck->getName().empty()) {
							logger.newUserInfo() << "The graph '" << graphToCheck->getName() << "' has found to be represented ";
							logger.contMsg() << "by an unnamed minimal graph (id=" << miniGraph->getID() << ")." << endLogMessage;
						} else {
							logger.newUserInfo() << "The unnamed graph (id=" << graphToCheck->getID() << ") has found to be ";
							logger.contMsg() << "represented by an unnamed minimal graph (id=" << miniGraph->getID() << ")." << endLogMessage;
						}

						if(faststop) {
							logger.newUserInfo() << "Skipped further checks due to faststop." << endLogMessage;
							return;
						}

					}

				}
			}

			logger.newUserInfo() << "In total " << count;
			if(count == 1) {
				logger.contMsg() << " relation between graphs was found and ";
			} else {
				logger.contMsg() << " relations between graphs where found and ";
			}
			if(graphCount == 1) {
				logger.contMsg() << graphCount << " graph is represented by minimal elements." << endLogMessage;
			} else {
				logger.contMsg() << graphCount << " distinct graphs are represented by minimal elements." << endLogMessage;
			}

		}

	} /* namespace scenarios */
} /* namespace uncover */
