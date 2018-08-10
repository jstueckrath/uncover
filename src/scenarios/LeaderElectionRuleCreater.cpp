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

#include "LeaderElectionRuleCreater.h"
#include "scenario_ids.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "../logging/message_logging.h"
#include "../basic_types/InitialisationException.h"
#include "../io/GTXLWriter.h"

using namespace uncover::logging;
using namespace uncover::basic_types;
using namespace uncover::io;
using namespace uncover::graphs;
using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace uncover {
	namespace scenarios {

		LeaderElectionRuleCreater::LeaderElectionRuleCreater() : Scenario(SCN_LE_RULE_CREATOR_NAME,
			"This scenario generates a GTS for the leader election example. The number of processes as well "
			"as a filename must be given. The generated GTS is written as a GTXL file to the given location.",
			{SCN_LE_RULE_CREATOR_SHORT_1}), processCount(0), name() {

			this->addParameter("count","The number of processes for which the GTS should be created.",true);
			this->addSynonymFor("count","c");
			this->addParameter("name","The path and name used to store the GTXL file.",true);
			this->addSynonymFor("name","n");

		}

		LeaderElectionRuleCreater::~LeaderElectionRuleCreater() {}

		void LeaderElectionRuleCreater::run() {

			stringstream str;
			str << "leader election for " << processCount << " processes";
			GTS gts(str.str());
			str.str(string());
			str.clear();

			// add: initial graph
			Hypergraph_sp initgraph = make_shared<Hypergraph>("Initial leader election ring");
			IDType firstNode = initgraph->addVertex();
			IDType lastNode = firstNode;

			for(unsigned int i = 1; i < processCount; ++i) {
				IDType newNode = initgraph->addVertex();
				str << "P" << i;
				initgraph->addEdge(str.str(),{lastNode,newNode});
				lastNode = newNode;
				str.str(string());
				str.clear();
			}
			str << "P" << processCount;
			initgraph->addEdge(str.str(),{lastNode,firstNode});
			str.str(string());
			str.clear();

			gts.addInitialGraph(initgraph);

			// add: generate message rule
			for(unsigned int i = 1; i <= processCount; ++i) {

				AnonHypergraph_sp leftGraph = make_shared<AnonHypergraph>();
				IDType node1 = leftGraph->addVertex();
				IDType node2 = leftGraph->addVertex();
				str << "P" << i;
				IDType edge1 = leftGraph->addEdge(str.str(),{node1,node2});
				str.str(string());
				str.clear();

				AnonHypergraph_sp rightGraph = make_shared<AnonHypergraph>(*leftGraph);
				str << "M" << i;
				rightGraph->addEdge(str.str(),{node2});
				str.str(string());
				str.clear();

				Mapping map;
				map[node1] = node1;
				map[node2] = node2;
				map[edge1] = edge1;

				str << "P" << i << " generates message";
				gts.addStdRule(make_shared<Rule>(str.str(),leftGraph,rightGraph,map));
				str.str(string());
				str.clear();

			}

			// add: forward message if ID is smaller
			for(unsigned int i = 1; i <= processCount; ++i) {
				for(unsigned int j = i+1; j <= processCount; ++j) {

					AnonHypergraph_sp leftGraph = make_shared<AnonHypergraph>();
					IDType node1 = leftGraph->addVertex();
					IDType node2 = leftGraph->addVertex();
					str << "P" << j;
					IDType edge1 = leftGraph->addEdge(str.str(),{node1,node2});
					str.str(string());
					str.clear();

					AnonHypergraph_sp rightGraph = make_shared<AnonHypergraph>(*leftGraph);
					str << "M" << i;
					rightGraph->addEdge(str.str(),{node2});

					leftGraph->addEdge(str.str(),{node1});
					str.str(string());
					str.clear();

					Mapping map;
					map[node1] = node1;
					map[node2] = node2;
					map[edge1] = edge1;

					str << "P" << j << " forwards message of P" << i;
					gts.addStdRule(make_shared<Rule>(str.str(),leftGraph,rightGraph,map));
					str.str(string());
					str.clear();

				}
			}

			// add: receive message and declare leader
			for(unsigned int i = 1; i <= processCount; ++i) {

				AnonHypergraph_sp leftGraph = make_shared<AnonHypergraph>();
				IDType node1 = leftGraph->addVertex();
				IDType node2 = leftGraph->addVertex();
				str << "P" << i;
				leftGraph->addEdge(str.str(),{node1,node2});
				str.str(string());
				str.clear();
				str << "M" << i;
				leftGraph->addEdge(str.str(),{node1});
				str.str(string());
				str.clear();

				AnonHypergraph_sp rightGraph = make_shared<AnonHypergraph>();
				IDType node1b = rightGraph->addVertex();
				IDType node2b = rightGraph->addVertex();
				rightGraph->addEdge("L",{node1b,node2b});

				Mapping map;
				map[node1] = node1b;
				map[node2] = node2b;

				str << "P" << i << " receives message";
				gts.addStdRule(make_shared<Rule>(str.str(),leftGraph,rightGraph,map));
				str.str(string());
				str.clear();

			}

			// add: leave ring
			for(unsigned int i = 1; i <= processCount; ++i) {

				AnonHypergraph_sp leftGraph = make_shared<AnonHypergraph>();
				IDType node1 = leftGraph->addVertex();
				IDType node2 = leftGraph->addVertex();
				str << "P" << i;
				leftGraph->addEdge(str.str(),{node1,node2});
				str.str(string());
				str.clear();

				AnonHypergraph_sp rightGraph = make_shared<AnonHypergraph>();
				IDType node12 = rightGraph->addVertex();

				Mapping map;
				map[node1] = node12;
				map[node2] = node12;

				str << "P" << i << " leaves ring";
				gts.addStdRule(make_shared<Rule>(str.str(),leftGraph,rightGraph,map));
				str.str(string());
				str.clear();

			}

			// add: leader leaves the ring
			AnonHypergraph_sp leftGraph = make_shared<AnonHypergraph>();
			IDType node1 = leftGraph->addVertex();
			IDType node2 = leftGraph->addVertex();
			leftGraph->addEdge("L",{node1,node2});

			AnonHypergraph_sp rightGraph = make_shared<AnonHypergraph>();
			IDType node12 = rightGraph->addVertex();

			Mapping map;
			map[node1] = node12;
			map[node2] = node12;

			gts.addStdRule(make_shared<Rule>("Leader leaves the ring",leftGraph,rightGraph,map));

			auto writer = GTXLWriter::newUniqueWriter();
			writer->writeGTXL(gts,name);

		}

		void LeaderElectionRuleCreater::initialize(unordered_map<string, string>& parameters) {

			name = parameters["name"];

			try {
				processCount = lexical_cast<unsigned int>(parameters["count"]);

				if(processCount == 0) {
					throw InitialisationException("A process count of 0 is no valid input.");
				}

			}	catch(bad_lexical_cast &) {

				stringstream str;
				str << "Invalid number '" << parameters["count"] << "' given for the number of processes.";

				throw InitialisationException(str.str());
			}

		}

	} /* namespace scenarios */
} /* namespace uncover */
