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

#include "GTXLtoLatexConverter.h"
#include "scenario_ids.h"
#include "../io/io_helpers.h"
#include "../io/LatexExporter.h"
#include "../io/GTXLReader.h"
#include "../io/XMLioException.h"
#include "../basic_types/InitialisationException.h"
#include "../logging/message_logging.h"
#include <sstream>

using namespace uncover::io;
using namespace uncover::graphs;
using namespace uncover::logging;
using namespace uncover::basic_types;
using namespace std;

namespace uncover {
	namespace scenarios {

		GTXLtoLatexConverter::GTXLtoLatexConverter() : Scenario(SCN_GTXL_LATEX_CONVERTER_NAME,
				"This scenario draws the contents of a GTXL file using Latex.",
				{SCN_GTXL_LATEX_CONVERTER_SHORT_1}),
				type(LatexExporter::TargetType::PDF),
				compile(true),
				ruleIDSubs(nullptr),
				graphIDSubs(nullptr) {
			this->addParameter("gts", "The filename of a GTXL (including extension).", true);
			this->addParameter("format", "The output format in which the generated latex file will be compiled. "
					"Possibilities are: ps, pdf (default)", false);
			this->addSynonymFor("format", "f");
			this->addParameter("target", "The filename of the Latex file which will be generated (including extension). "
					"If not provided, the gts name is used.", false);
			this->addSynonymFor("target", "t");
			this->addParameter("compile", "Set this parameter to 'false' to suppress the automatic compilation of "
					"the generated Latex file. (default is 'true')", false);
			this->addSynonymFor("compile", "c");
		}

		GTXLtoLatexConverter::~GTXLtoLatexConverter() {
			if(ruleIDSubs) delete ruleIDSubs;
			if(graphIDSubs) delete graphIDSubs;
		}

		void GTXLtoLatexConverter::run() {
			LatexExporter exporter;
			exporter.setCompilation(compile);
			exporter.writeToLatex(*gts, targetName, type, ruleIDSubs, graphIDSubs);
		}

		void GTXLtoLatexConverter::initialize(unordered_map<string,string>& parameters) {

			// what name to use for the new file?
			if(parameters.count("target")) {
				targetName = parameters["target"];
			} else {
				targetName = removeFileExtension(parameters["gts"]) + ".tex";
			}

			// create files for what format?
			if(parameters.count("format")) {
				if(!parameters["format"].compare("ps")) {
					type = LatexExporter::TargetType::PS;
				} else if(!parameters["format"].compare("pdf")){
					type = LatexExporter::TargetType::PDF;
				} else {
					logger.newWarning() << "The given format '" << parameters["format"] << "' is unknown. 'pdf' will be used." << endLogMessage;
					type = LatexExporter::TargetType::PDF;
				}
			}

			// compile the created file?
			if(parameters.count("compile")) {
				if(!parameters["compile"].compare("false")) {
					compile = false;
				} else if(parameters["compile"].compare("true")) {
					compile = true;
				} else {
					logger.newWarning() << "Invalid value '" << parameters["compile"] << "' for parameter compile. 'true' will be used." << endLogMessage;
					compile = true;
				}
			}

			// read the GTS from file
			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			try {
				ruleIDSubs = new unordered_map<IDType,IDType>();
				graphIDSubs = new unordered_map<IDType,IDType>();
				gts = reader->readGTXL(parameters["gts"],ruleIDSubs,graphIDSubs);
				if(ruleIDSubs->size() == 0) {
					delete ruleIDSubs;
					ruleIDSubs = nullptr;
				}
				if(graphIDSubs->size() == 0) {
					delete graphIDSubs;
					graphIDSubs = nullptr;
				}
			} catch (XMLioException& ex) {
				stringstream ss;
				ss << "Failed to load the GTS to be converted: " << ex.what();
				throw InitialisationException(ss.str());
			}

		}

	} /* namespace scenarios */
} /* namespace uncover */
