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

#include "BackwardAnalysisScenario.h"
#include "scenario_ids.h"
#include "../io/GTXLReader.h"
#include "../io/GTXLWriter.h"
#include "../io/XMLioException.h"
#include "../minors/MinorOrder.h"
#include "../minors/MinorRulePreparer.h"
#include "../minors/MinorPOCEnumerator.h"
#include "../subgraphs/SubgraphOrder.h"
#include "../subgraphs/SubgraphRulePreparer.h"
#include "../subgraphs/SubgraphPOCEnumerator.h"
#include "../analysis/InstantWriterResultHandler.h"
#include "../basic_types/InitialisationException.h"
#include "../logging/message_logging.h"
#include <sstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace uncover::logging;
using namespace uncover::io;
using namespace uncover::minors;
using namespace uncover::subgraphs;
using namespace uncover::basic_types;
using namespace uncover::graphs;
using namespace uncover::analysis;
using namespace boost::filesystem;
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace uncover {
	namespace scenarios {

		BackwardAnalysisScenario::BackwardAnalysisScenario() : Scenario(SCN_BACKWARD_ANALYSIS_NAME,
				"This scenario performs a backward search algorithm. It takes a graph transformation system and a "
				"set of error graphs as input and computes the set of graphs, from which the given graphs are coverable. "
				"Use the parameters to specify the order and result storage.", {SCN_BACKWARD_ANALYSIS_SHORT_1}),
				backwardAnalysis(nullptr), resultHandler(nullptr), resultFolder("") {
			this->addParameter("gts", "An XML file (in GTXL) containing the GTS which will be analyzed.", true);
			this->addParameter("error-graph", "A GXL file containing the set of error graph which should not be coverable.", true);
			this->addSynonymFor("error-graph", "err");
			this->addParameter("order", "Specifies which wqo should be used. The given GTS has to be well-structured "
					"with respect to the given order. Possibilities: minor, subgraph[?] (? can be a natural number or '-')", true);
			this->addSynonymFor("order", "or");
			this->addParameter("result-folder", "A folder where all intermediate results will be stored.", false);
			this->addSynonymFor("result-folder", "res");
			this->addParameter("timeout", "If the scenario takes longer than the given timeout (in seconds), "
					"it will be terminated (soft termination, i.e. may run longer).",false);
			this->addSynonymFor("timeout", "to");
			this->addParameter("check-initial", "If set to true, after every backward step it will be checked whether one "
					"of the initial graphs is represented by the calculated graph set and the analysis will stop if the check "
					"succeeds. Default is 'false'.", false);
			this->addSynonymFor("check-initial","ci");
			this->addParameter("matching", "Defines the type of matchings used. Available are: 'conflictfree' (default, alias: 'cf') "
					"and 'injective' (alias: 'inj')", false);
			this->addSynonymFor("matching", "m");
		}

		BackwardAnalysisScenario::~BackwardAnalysisScenario() {
			if(backwardAnalysis) delete backwardAnalysis;
		}

		void BackwardAnalysisScenario::initialize(unordered_map<string,string>& parameters) {

			// read GTS from file
			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			GTS_sp gts = nullptr;
			try {
				gts = reader->readGTXL(parameters["gts"]);
			} catch (XMLioException& ex) {
				stringstream ss;
				ss << "Failed to load the given GTS: " << ex.what();
				throw InitialisationException(ss.str());
			}

			// read error graphs from file
			shared_ptr<vector<Hypergraph_sp>> errorGraphs = nullptr;
			try {
				errorGraphs = reader->readGXL(parameters["error-graph"]);
			} catch (XMLioException& ex) {
				stringstream ss;
				ss << "Failed to load the given error graphs: " << ex.what();
				throw InitialisationException(ss.str());
			}

			// initialize ResultHandler
			if(parameters.count("result-folder")) {
				resultFolder = path(parameters["result-folder"]);
				if(exists(resultFolder) && is_directory(resultFolder)) {
					resultHandler = make_shared<InstantWriterResultHandler>(resultFolder);
				} else {
					resultFolder = path("");
					logger.newError() << "The given result folder '" + parameters["result-folder"];
					logger.contMsg() << "' does not exist or is no folder. Deactivating result logging!" << endLogMessage;
				}
			}

			// read timeout if given
			unsigned int timeout = 0;
			if(parameters.count("timeout")) {
				try {
					timeout = lexical_cast<unsigned int>(parameters["timeout"]);
				}	catch(bad_lexical_cast& ex) {
						logger.newWarning() << "Invalid timeout '" << parameters["timeout"] << "'. No timeout used." << endLogMessage;
				}
			}

			// read if initial graphs should be checked
			bool checkInitial = false;
			if(parameters.count("check-initial")) {
				if(!parameters["check-initial"].compare("true")) {
					checkInitial = true;
				} else if(!parameters["check-initial"].compare("false")) {
					checkInitial = false;
				} else {
					logger.newWarning() << "Invalid value '" << parameters["check-initial"] << "' for check-initial parameter.";
					logger.contMsg() << " Using default (false)." << endLogMessage;
				}
			}

			bool injective = false;
			if(parameters.count("matching")) {
				if(!parameters["matching"].compare("injective") || !parameters["matching"].compare("inj")) {
					injective = true;
				} else if(!parameters["matching"].compare("conflictfree") || !parameters["matching"].compare("cf")) {
					injective = false;
				} else {
					logger.newWarning() << "Invalid value '" << parameters["matching"] << "' for matching parameter.";
					logger.contMsg() << " Using default (conflictfree)." << endLogMessage;
				}
			}

			// read order specific components and initialize BackwardAnalysis object
			boost::regex subgraphExpression("subgraph\\[([0-9]+|-)\\]");
			boost::smatch subgraphResults;
			if(!parameters["order"].compare("minor")) {

				if(injective) {
					logger.newError() << "Injective matchings not yet supported by minor order,";
					logger.contMsg() << " using conflict-free matches." << endLogMessage;
				}

				if(gts->getUQRuleCount() > 0) {
					logger.newCriticalError() << "The minor ordering can currently not handle universally "
							"quantified rules. Aborting analysis." << endLogMessage;
					throw InitialisationException("The minor ordering cannot be analyzed when using universally quantified rules!");
				}

				backwardAnalysis = new BackwardAnalysis(
						gts,
						errorGraphs,
						make_shared<MinorOrder>(),
						make_shared<MinorRulePreparer>(),
						make_shared<MinorPOCEnumerator>(),
						checkInitial,
						resultHandler,
						timeout);

			} else if(boost::regex_match(parameters["order"], subgraphResults, subgraphExpression)) {

				if(subgraphResults.size() != 2) {
					stringstream ss;
					ss << "Failed to initialize scenario '" << this->scenarioName << "', because of invalid parameter count " << subgraphResults.size() << " of subgraph order.";
					throw InitialisationException(ss.str());
				} else {

					// check for bound on path length
					string subgraphBound = *(++subgraphResults.begin());
					int bound = -1;
					if(subgraphBound.compare("-")) {
						try {
							bound = lexical_cast<int>(subgraphBound);
						}	catch(bad_lexical_cast &) {
							logger.newWarning() << "Invalid bound '" << subgraphBound << "' given. No bound on path length will be used." << endLogMessage;
							bound = -1;
						}
					}

					if(!injective && gts->getUQRuleCount() > 0) {
						throw InitialisationException("The subgraph ordering cannot be analyzed when using universally "
								"quantified rules together with conflict-free matches!");
					}

					backwardAnalysis = new BackwardAnalysis(
							gts,
							errorGraphs,
							make_shared<SubgraphOrder>(),
							make_shared<SubgraphRulePreparer>(),
							make_shared<SubgraphPOCEnumerator>(injective, bound),
							checkInitial,
							resultHandler,
							timeout);

				}
			} else {
				throw InitialisationException("Failed to initialize scenario '" + this->scenarioName + "', because the order '" + parameters["order"] + "' is unknown.");
			}
		}

		void BackwardAnalysisScenario::run() {
			shared_ptr<vector<Hypergraph_sp>> errorGraphs = backwardAnalysis->performAnalysis();
			unique_ptr<GTXLWriter> writer = GTXLWriter::newUniqueWriter();
			logger.newUserInfo() << "Writing error graphs (" << errorGraphs->size() << " in total)." << endLogMessage;

			path filename("");
			if(resultFolder.empty()) {
				filename = path("ErrorGraphs.xml");
			} else {
				filename = resultFolder;
				filename /= "ErrorGraphs.xml";
			}
			writer->writeGXL(*errorGraphs, filename);
		}

	} /* namespace scenarios */
} /* namespace uncover */
