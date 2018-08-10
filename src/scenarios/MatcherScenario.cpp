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

#include "MatcherScenario.h"
#include "scenario_ids.h"
#include "../rule_engine/StdMatchFinder.h"
#include "../io/GTXLReader.h"
#include "../io/GTXLWriter.h"
#include <sstream>

using namespace uncover::rule_engine;
using namespace uncover::io;
using namespace uncover::graphs;
using namespace std;

namespace uncover {
	namespace scenarios {

		MatcherScenario::MatcherScenario() : Scenario(SCN_TEST_MATCHER_NAME,
				"Generates for two given sets of graphs all matchings from one to the other."),
				srcGraphs(nullptr),
				tarGraphs(nullptr),
				resultFilename(),
				injective(false) {
			this->addParameter("source", "The set of graphs (as GXL) from which the matching should be calculated.", true);
			this->addSynonymFor("source", "src");
			this->addParameter("target", "The set of graphs (as GXL) to which the matching should be calculated.", true);
			this->addSynonymFor("target", "tar");
			this->addParameter("result", "The filename in which all these matchings (as GTXL file) should be stored.", true);
			this->addSynonymFor("result", "res");
			this->addParameter("injective", "Set this to true, to only allow injective matchings.", false);
			this->addSynonymFor("injective", "inj");
		}

		MatcherScenario::~MatcherScenario() {}

		void MatcherScenario::run() {
			unique_ptr<GTXLWriter> writer = GTXLWriter::newUniqueWriter();
			StdMatchFinder matcher(injective);
			GTS gts;

			for(vector<Hypergraph_sp>::iterator srcIt = srcGraphs->begin(); srcIt != srcGraphs->end(); ++srcIt) {
				for(vector<Hypergraph_sp>::iterator tarIt = tarGraphs->begin(); tarIt != tarGraphs->end(); ++tarIt) {
					shared_ptr<vector<Mapping>> mappings = matcher.findMatches(**srcIt,**tarIt);
					int counter = 0;
					for(vector<Mapping>::iterator mapIt = mappings->begin(); mapIt != mappings->end(); ++mapIt, ++counter) {
						stringstream ss;
						ss << (*srcIt)->getName() << "-" << counter << "-" << (*tarIt)->getName();
						gts.addStdRule(make_shared<Rule>(ss.str(),make_shared<Hypergraph>(**srcIt),make_shared<Hypergraph>(**tarIt),*mapIt));
					}
				}
			}

			writer->writeGTXL(gts,resultFilename);
		}

		void MatcherScenario::initialize(unordered_map<string,string>& parameters) {
			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			srcGraphs = reader->readGXL(parameters["source"]);
			tarGraphs = reader->readGXL(parameters["target"]);
			resultFilename = parameters["result"];
			if(parameters.count("injective") != 0) {
				if(parameters["injective"].compare("true") == 0) {
					injective = true;
				} // everything else means false
			}
		}

	} /* namespace scenarios */
} /* namespace uncover */
