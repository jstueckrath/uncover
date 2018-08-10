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

#include "XMLTestScenario.h"
#include "scenario_ids.h"
#include <boost/lexical_cast.hpp>
#include "../logging/message_logging.h"
#include "../io/GTXLReader.h"
#include "../io/GTXLWriter.h"

#define GXL_COUNT "gxl-count"
#define GTXL_COUNT "gtxl-count"
#define GXL_STUB "gxl-stub"
#define GTXL_STUB "gtxl-stub"

using namespace std;
using namespace uncover::logging;
using namespace uncover::graphs;
using namespace uncover::io;

namespace uncover {
	namespace scenarios {

		XMLTestScenario::XMLTestScenario() : Scenario(SCN_TEST_XML_NAME,
				"This scenario tests The GXL and GTXL import and export functionality of this framework.", {}) {
			this->addParameter(GXL_STUB, "Base name of the GXL input files without number and extension.", false);
			this->addSynonymFor(GXL_STUB, "gxls");
			this->addParameter(GXL_COUNT, "Number of GXL files to test (default is 0). If this is not 0, "
					"'gxl-file-stub' has to be given.", false);
			this->addSynonymFor(GXL_COUNT, "gxl");
			this->addParameter(GTXL_STUB, "Base name of the GTXL input files without number and extension.", false);
			this->addSynonymFor(GTXL_STUB, "gtxls");
			this->addParameter(GTXL_COUNT, "Number of GTXL files to test (default is 0). If this is not 0, "
					"'gtxl-file-stub' has to be given.", false);
			this->addSynonymFor(GTXL_COUNT, "gtxl");
		}

		XMLTestScenario::~XMLTestScenario() {}

		void XMLTestScenario::run() {
			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			unique_ptr<GTXLWriter> writer = GTXLWriter::newUniqueWriter();

			logger.newUserInfo() << "Now testing GXL import and export..." << endLogMessage;

			for(vector<string>::iterator iter = gxlFilenames.begin(); iter != gxlFilenames.end(); iter++) {
				try {
					// Test GXL import
					logger.newUserInfo() << "Trying to load '" << (*iter) << "'." << endLogMessage;
					shared_ptr<vector<shared_ptr<Hypergraph>>> readGraphs = reader->readGXL(*iter);
					logger.newUserInfo() << readGraphs->size() << " hypergraph(s) were successfully loaded from '" << (*iter) << "'." << endLogMessage;
					int i = 1;
					for(vector<shared_ptr<Hypergraph>>::iterator iter2 = readGraphs->begin(); iter2 != readGraphs->end(); iter2++, i++) {
						logger.newUserInfo() << "Graph " << i << ": " << *(*iter2) << endLogMessage;
					}

					// Test GXL export
					string newName(iter->substr(0,iter->size()-4) + "new.xml");
					logger.newUserInfo() << "Trying to save '" << *iter << "' to '" << newName << "'." << endLogMessage;
					writer->writeGXL(*readGraphs, newName);
					logger.newUserInfo() << readGraphs->size() << " hypergraph(s) successfully written to '" << newName << "'." << endLogMessage;
					if(reader->readGXL(newName)->size() != readGraphs->size()) {
						logger.newError() << "Export control check failed. Importing the exported graphs resulted in a different number of graphs." << endLogMessage;
					} else {
						logger.newUserInfo() << "Export control check successful." << endLogMessage;
					}

				} catch (exception &e) {
					logger.newError() << "'" << (*iter) << "' could not be loaded: " << e.what() << endLogMessage;
				}
			}

			logger.newUserInfo() << "Now testing GTXL import and export..." << endLogMessage;

			for(vector<string>::iterator iter = gtxlFilenames.begin(); iter != gtxlFilenames.end(); iter++) {
				try {
					// Test GTXL import
					logger.newUserInfo() << "Trying to load '" << (*iter) << "'." << endLogMessage;
					shared_ptr<GTS> gts = reader->readGTXL(*iter);
					if(gts != NULL) {
						logger.newUserInfo() << "The GTS was successfully loaded from '" << (*iter) << "': " << *gts << endLogMessage;

						// Test GTXL export
						string newName(iter->substr(0,iter->size()-4) + "new.xml");
						logger.newUserInfo() << "Trying to save '" << *iter << "' to '" << newName << "'." << endLogMessage;
						writer->writeGTXL(*gts, newName);
						logger.newUserInfo() << "GTS has been written successfully." << endLogMessage;
						shared_ptr<GTS> newGTS = reader->readGTXL(newName);
						if(gts->getName() != newGTS->getName() || gts->getStdRules().size() != newGTS->getStdRules().size() ||
								gts->getInitialGraphs().size() != newGTS->getInitialGraphs().size()) {
							logger.newError() << "Export control check failed. Importing the exported GTS resulted in a different GTS." << endLogMessage;
						} else {
							logger.newUserInfo() << "Export control check successful." << endLogMessage;
						}
					}
				} catch (exception &e) {
					logger.newError() << "'" << (*iter) << "' could not be loaded: " << e.what() << endLogMessage;
				}
			}
		}

		void XMLTestScenario::initialize(unordered_map<string, string>& parameters) {

			// prepare GXL import
			int count(0);
			if(parameters.count(GXL_COUNT)) {
				try {
					count = boost::lexical_cast<int>(parameters[GXL_COUNT]);
				}
				catch( const boost::bad_lexical_cast & )
				{
					logger.newError() << "Error while initializing scenario '" << this->scenarioName
												<< "'. Parameter '" << GXL_COUNT << "' is not an integer. Default used." << endLogMessage;
				}
			}
			if(count > 0 && parameters.count(GXL_STUB) == 0) {
				logger.newError() << "Error while initializing scenario '" << this->scenarioName << "'. Parameter '" << GXL_COUNT
						<< "' is given, but '" << GXL_STUB << "' is missing. GXL will be ignored." << endLogMessage;
			} else {
				for(int i = 0; i < count; i++) {
					gxlFilenames.push_back(parameters[GXL_STUB] + (i < 10 ? "0" : "") + boost::lexical_cast<string>(i) + ".xml");
				}
			}

			// prepare GTXL import
			count = 0;
			if(parameters.count(GTXL_COUNT)) {
				try {
					count = boost::lexical_cast<int>(parameters[GTXL_COUNT]);
				}
				catch( const boost::bad_lexical_cast & )
				{
					logger.newError() << "Error while initializing scenario '" << this->scenarioName
												<< "'. Parameter '" << GTXL_COUNT << "' is not an integer. Default used." << endLogMessage;
				}
			}
			if(count > 0 && parameters.count(GTXL_STUB) == 0) {
				logger.newError() << "Error while initializing scenario '" << this->scenarioName
											<< "'. Parameter '" << GTXL_COUNT << "' is given, but '" << GTXL_STUB << "' is missing. GXL will be ignored." << endLogMessage;
			} else {
				for(int i = 0; i < count; i++) {
					gtxlFilenames.push_back(parameters[GTXL_STUB] + (i < 10 ? "0" : "") + boost::lexical_cast<string>(i) + ".xml");
				}
			}
		}

	}/* namespace scenarios */
} /* namespace uncover */
