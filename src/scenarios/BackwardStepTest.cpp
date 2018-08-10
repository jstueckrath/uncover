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

#include "BackwardStepTest.h"
#include <sstream>
#include "scenario_ids.h"
#include "../io/GTXLReader.h"
#include "../io/GTXLWriter.h"
#include "../io/io_helpers.h"
#include "../subgraphs/SubgraphRulePreparer.h"
#include "../subgraphs/SubgraphOrder.h"
#include "../subgraphs/SubgraphPOCEnumerator.h"
#include "../minors/MinorRulePreparer.h"
#include "../minors/MinorOrder.h"
#include "../minors/MinorPOCEnumerator.h"
#include "../analysis/NoRulePreparer.h"
#include "../graphs/MinGraphList.h"
#include "../logging/message_logging.h"
#include "../analysis/PreparedGTS.h"
#include "../basic_types/InitialisationException.h"

using namespace uncover::io;
using namespace uncover::graphs;
using namespace uncover::subgraphs;
using namespace uncover::minors;
using namespace uncover::logging;
using namespace uncover::rule_engine;
using namespace uncover::analysis;
using namespace uncover::basic_types;
using namespace std;

namespace uncover {
	namespace scenarios {

		BackwardStepTest::BackwardStepTest() :
				Scenario(SCN_TEST_BACKWARD_STEP_NAME,
						"Performs a single backward step. This test only uses the standard rules of the given GTS.",
						{SCN_TEST_BACKWARD_STEP_SHORT_1}),
				rulesFilename(""),
				gts(nullptr),
				errorGraphs(nullptr),
				optimize(true),
				injective(false),
				order(BackwardStepTest::Order::SUBGRAPH) {
			this->addParameter("rules", "The rules used in the backward step.", true);
			this->addSynonymFor("rules", "r");
			this->addParameter("graphs", "The error graphs on which the backward step is performed.", true);
			this->addSynonymFor("graphs", "g");
			this->addParameter("order", "Defines what order will be used. Possibilities are: minor, subgraph", true);
			this->addSynonymFor("order", "o");
			this->addParameter("injective", "Whether the matches used must be injective or conflict-free. Possibilities: true, false (default).", false);
			this->addSynonymFor("injective", "inj");
			this->addParameter("optimize", "Set whether optimization is used or not (default: true).", false);
			this->addSynonymFor("optimize", "opt");
		}

		BackwardStepTest::~BackwardStepTest() {}

		void BackwardStepTest::initialize(unordered_map<string,string>& parameters) {

			// load rules and graphs
			rulesFilename = parameters["rules"];
			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			gts = reader->readGTXL(parameters["rules"]);
			if(gts == nullptr) {
				throw InitialisationException("GTS could not be loaded due to unknown reason.");
			}
			errorGraphs = reader->readGXL(parameters["graphs"]);
			if(errorGraphs == nullptr) {
				throw InitialisationException("Error graphs could not be loaded due to unknown reason.");
			}

			// check for optimize
			if(parameters.count("optimize")) {
				if(!parameters["optimize"].compare("false")) {
					optimize = false;
				} else if(!parameters["optimize"].compare("true")) {
					optimize = true;
				} else {
					logger.newWarning() << "Unknown value '" << parameters["optimize"] << "' for parameter 'optimize'. ";
					logger.contMsg() << "Using default (true)." << endLogMessage;
					optimize = true;
				}
			}

			// check for injective
			if(parameters.count("injective")) {
				if(!parameters["injective"].compare("false")) {
					injective = false;
				} else if(!parameters["injective"].compare("true")) {
					injective = true;
				} else {
					logger.newWarning() << "Unknown value '" << parameters["injective"] << "' for parameter 'injective'. ";
					logger.contMsg() << "Using default (false)." << endLogMessage;
					injective = true;
				}
			}

			// check for order
			if(!parameters["order"].compare("minor")) {
				order = BackwardStepTest::Order::MINOR;
				if(injective) {
					logger.newWarning() << "Injective matchings are not supported by the minor ordering. Using conflict-free matches." << endLogMessage;
				}
			} else if(!parameters["order"].compare("subgraph")) {
				order = BackwardStepTest::Order::SUBGRAPH;
			} else {
				throw InitialisationException("Unknown order '" + parameters["order"] + "'. Aborting test.");
			}

		}

		void BackwardStepTest::run() {

			unique_ptr<GTXLWriter> writer = GTXLWriter::newUniqueWriter();

			PreparedGTS_sp outGTS;
			MinPOCEnumerator_sp pocEnumerator;
			switch(order) {

				case BackwardStepTest::Order::SUBGRAPH:
					outGTS = make_shared<PreparedGTS>(*gts, make_shared<SubgraphRulePreparer>(), optimize);
					pocEnumerator = make_shared<SubgraphPOCEnumerator>(injective,-1);
					break;

				case BackwardStepTest::Order::MINOR:
					outGTS = make_shared<PreparedGTS>(*gts, make_shared<MinorRulePreparer>(), optimize);
					pocEnumerator = make_shared<MinorPOCEnumerator>();
					break;

			}

			writer->writeGTXL(*outGTS,removeFileExtension(removeFilePath(rulesFilename)) + "_new.xml");

			for(auto ruleIt = outGTS->getStdRules().cbegin(); ruleIt != outGTS->getStdRules().cend(); ++ruleIt) {
				for(auto itG = errorGraphs->cbegin(); itG != errorGraphs->cend(); ++itG) {

					logger.newUserInfo() << "Performing backward step of rule with id " << ruleIt->first << " to graph '";
					logger.contMsg() << (*itG)->getName() << "' (id=" << (*itG)->getID() << ")" << endLogMessage;

					vector<Hypergraph_sp> result;
					pocEnumerator->resetWith(ruleIt->second,*itG);
					while(!pocEnumerator->ended()) {
						result.push_back(**pocEnumerator);
						++(*pocEnumerator);
					}

					logger.newUserInfo() << "A total of " << result.size() << " graphs where generated. The IDs are: ";
					for(auto resIt = result.cbegin(); resIt != result.cend(); ++resIt) {
						logger.contMsg() << (*resIt)->getID() << "; ";
					}
					logger.endMsg();

					stringstream ss;
					ss << "graph" << (*itG)->getID() << ".xml";
					writer->writeGXL(result, ss.str());

				}
			}

		}

	} /* namespace scenarios */
} /* namespace uncover */
