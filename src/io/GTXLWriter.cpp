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

#include "GTXLWriter.h"
#include "GTXLWriterXerces.h"

using namespace uncover::graphs;
using namespace boost::filesystem;

namespace uncover {
	namespace io {

		GTXLWriter* GTXLWriter::newWriter() {
			return new GTXLWriterXerces();
		}

		unique_ptr<GTXLWriter> GTXLWriter::newUniqueWriter() {
			return unique_ptr<GTXLWriter>(new GTXLWriterXerces());
		}

		GTXLWriter::GTXLWriter() : storeGraphIDs(true), storeRuleIDs(true) {}

		GTXLWriter::~GTXLWriter() {}

		void GTXLWriter::writeGXL(vector<graphs::Hypergraph_sp> const& graphs, string filename) const {
			path filenamePath(filename);
			this->writeGXL(graphs,filenamePath);
		}

		void GTXLWriter::writeGXL(Hypergraph_sp graph, string filename) const {
			path filenamePath(filename);
			this->writeGXL(graph, filenamePath);
		}

		void GTXLWriter::writeGXL(Hypergraph_sp graph, path& filename) const {
			vector<Hypergraph_sp> temp;
			temp.push_back(graph);
			this->writeGXL(temp, filename);
		}

		void GTXLWriter::writeGTXL(graphs::GTS const& gts, string filename) const {
			path filenamePath(filename);
			this->writeGTXL(gts, filenamePath);
		}

		void GTXLWriter::setGraphIDStorage(bool value) {
			storeGraphIDs = value;
		}

		bool GTXLWriter::getGraphIDStorage() const {
			return storeGraphIDs;
		}

		void GTXLWriter::setRuleIDStorage(bool value) {
			storeRuleIDs = value;
		}

		bool GTXLWriter::getRuleIDStorage() const {
			return storeRuleIDs;
		}

	} /* namespace io */
} /* namespace uncover */
