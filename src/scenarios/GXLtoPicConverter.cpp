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

#include "GXLtoPicConverter.h"

#include "../io/GTXLReader.h"
#include "../io/GraphDrawer.h"
#include "../logging/message_logging.h"
#include "../io/io_helpers.h"
#include "../io/LatexExporter.h"
#include "scenario_ids.h"
#include <sstream>

using namespace uncover::logging;
using namespace uncover::graphs;
using namespace uncover::io;
using namespace std;

namespace uncover {
	namespace scenarios {

		GXLtoPicConverter::GXLtoPicConverter() : Scenario(SCN_GXL_PIC_CONVERTER_NAME,
				"This scenario takes a set of hypergraphs as 'gxl' file, generates a 'dot' file from each and converts "
				"this 'dot' file to a specifiable file format using neato. This scenario can only be run if Graphviz "
				"is installed!",
				{SCN_GXL_PIC_CONVERTER_SHORT_1}), filename(), exportFormat(GraphDrawer::PictureFormat::PicPDF), compact(false)
		{
			this->addParameter("filename", "A GXL file (including extension). Mostly the same name is used for output files.", true);
			this->addSynonymFor("filename", "f");
			this->addParameter("format", "Specifies which file format will be generated from the 'dot' files. Possibilities: "
					"ps, pdf (default)", false);
			this->addSynonymFor("format", "o");
			this->addParameter("compact", "Set this parameter to true, to generate only one file listing all graphs, instead "
					"of generating one file per graph. Default is 'false'.", false);
			this->addSynonymFor("compact", "c");
		}

		GXLtoPicConverter::~GXLtoPicConverter() {}

		void GXLtoPicConverter::initialize(unordered_map<string,string>& parameters) {
			filename = parameters["filename"];

			if(parameters.count("format")) {
				if(!parameters["format"].compare("ps")) {
					exportFormat = GraphDrawer::PictureFormat::PicPS;
				} else if (!parameters["format"].compare("pdf")) {
					exportFormat = GraphDrawer::PictureFormat::PicPDF;
				} else {
					logger.newWarning() << "File format '" << parameters["format"] << "' unknown. Using default." << endLogMessage;
				}
			}

			if(parameters.count("compact")) {
				if(!parameters["compact"].compare("true")) {
					compact = true;
				} else if (!parameters["compact"].compare("false")) {
					compact = false;
				} else {
					logger.newWarning() << "Only values 'true' or 'false' are allowed for the compact parameter." << endLogMessage;
				}
			}
		}

		void GXLtoPicConverter::run()
		{
			logger.newUserInfo() << "Starting conversion of '" << filename << "' to '";
			logger.contMsg() << GraphDrawer::getStandardExtension(exportFormat) << "'." << endLogMessage;

			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			unordered_map<IDType, IDType> oldIDMap;
			shared_ptr<vector<Hypergraph_sp>> graphs = reader->readGXL(filename, &oldIDMap);
			string filenameStub = removeFileExtension(filename);

			if(graphs) {

				if(compact) {

					LatexExporter exporter;
					switch(exportFormat) {
						case GraphDrawer::PictureFormat::PicPS:
							exporter.writeToLatex(*graphs, filenameStub + ".tex", LatexExporter::TargetType::PS, &oldIDMap);
							break;
						case GraphDrawer::PictureFormat::PicPDF:
							exporter.writeToLatex(*graphs, filenameStub + ".tex", LatexExporter::TargetType::PDF, &oldIDMap);
							break;
						default:
							logger.newError() << "The given picture format is not supported as compact representation. Aborting export." << endLogMessage;
							break;
					}

				} else {

					if(graphs->size()) {

						unsigned int counter = 0;
						stringstream ss;
						ss << filenameStub;
						for(vector<Hypergraph_sp>::iterator it = graphs->begin(); it != graphs->end(); ++it) {
							if(oldIDMap.count((*it)->getID())) {
								ss << "-id" << oldIDMap[(*it)->getID()];
							} else {
								ss << "-new" << counter;
								++counter;
							}
							GraphDrawer::writeGraphToDotFile(**it, ss.str() + ".dot");
							GraphDrawer::convertDotTo(ss.str() + ".dot", ss.str(), exportFormat, true);
							ss.str("");
							ss.clear();
							ss << filenameStub;
						}
						logger.newUserInfo() << "Conversion completed." << endLogMessage;

					} else {
						logger.newUserInfo() << "No graphs could be read from the given gxl file." << endLogMessage;
					}

				}

			} else {
				logger.newUserInfo() << "No graphs could be read from the given gxl file." << endLogMessage;
			}
		}

	}
}
