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

#include "InstantWriterResultHandler.h"
#include <sstream>

using namespace uncover::io;
using namespace uncover::graphs;
using namespace boost::filesystem;
using namespace std;

typedef uncover::analysis::InstantWriterResultHandler::Extent Extent;

namespace uncover {
	namespace analysis {

		// cannot/should not be called
		InstantWriterResultHandler::InstantWriterResultHandler()
			: foldername(""),
				gtxlWriter(nullptr),
				graphGenerationFile(),
				graphDeletionFile(),
				currentStepGraphs(),
				stepnr(0),
				extent(Extent::FULL) {}

		InstantWriterResultHandler::InstantWriterResultHandler(path& foldername)
			: foldername(foldername),
				gtxlWriter(GTXLWriter::newUniqueWriter()),
				graphGenerationFile(),
				graphDeletionFile(),
				currentStepGraphs(),
				stepnr(0),
				extent(Extent::FULL){

			gtxlWriter->setGraphIDStorage(true);
			path filename(foldername);
			filename /= "generation.txt";
			graphGenerationFile.open(filename);
			filename = foldername;
			filename /= "deletion.txt";
			graphDeletionFile.open(filename);
		}

		InstantWriterResultHandler::~InstantWriterResultHandler() {
			if(graphGenerationFile.is_open()) {
				graphGenerationFile.close();
			}
			if(graphDeletionFile.is_open()) {
				graphDeletionFile.close();
			}
		}

		void InstantWriterResultHandler::setOldGTS(GTS_csp gts) {
			if(extent <= Extent::ID_FILES_AND_RULES) {
				path filename(foldername);
				filename /= "oldrules.xml";
				gtxlWriter->writeGTXL(*gts,filename);
			}
		}

		void InstantWriterResultHandler::setPreparedGTS(PreparedGTS_csp gts) {
			if(extent <= Extent::ID_FILES_AND_RULES) {
				path filename(foldername);
				filename /= "newrules.xml";
				gtxlWriter->writeGTXL(*gts,filename);
			}
		}

		void InstantWriterResultHandler::setFirstGraphs(shared_ptr<const vector<Hypergraph_sp>> graphs) {
			if(extent <= Extent::FULL) {
				path filename(foldername);
				filename /= "initialgraphs.xml";
				gtxlWriter->writeGXL(*graphs, filename);
			}
		}

		void InstantWriterResultHandler::wasDeletedBecauseOf(IDType deletedGraph, IDType reasonGraph) {
			graphDeletionFile << deletedGraph << "," << reasonGraph << "\n";
		}

		void InstantWriterResultHandler::wasNoValidGraph(IDType deletedGraph) {
			graphDeletionFile << deletedGraph << ",non-valid\n";
		}

		void InstantWriterResultHandler::wasGeneratedByFrom(Hypergraph_sp genGraph, IDType rule, IDType srcGraph) {
			if(extent <= Extent::FULL) {
				currentStepGraphs.push_back(genGraph);
			}
			graphGenerationFile << genGraph->getID() << "," << rule << "," << srcGraph << "\n";
		}

		void InstantWriterResultHandler::startingNewBackwardStep() {
			++stepnr;
			graphDeletionFile << "starting backward step " << stepnr << "\n";
			graphGenerationFile << "starting backward step " << stepnr << "\n";
		}

		void InstantWriterResultHandler::finishedBackwardStep() {
			if(extent <= Extent::FULL) {
				path filename(foldername);
				stringstream ss;
				ss << "graphs-step" << stepnr << ".xml";
				filename /= ss.str();
				gtxlWriter->writeGXL(currentStepGraphs, filename);
				currentStepGraphs.clear();
			}
		}

	} /* namespace analysis */
} /* namespace uncover */
