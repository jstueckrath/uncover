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

#ifdef COMPILE_UNIT_TESTS

#include "graph_generator.h"

using namespace uncover::graphs;

namespace uncover {
	namespace test {

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// predefined rules ////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////

		Rule_sp getRule(size_t index) {

			switch(index) {

				case 0: {

					// total, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 0 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 0 Right");
					IDType rightn1 = right->addVertex();

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[leftn3] = rightn1;

					return make_shared<Rule>("Test Rule 0",left,right,mapp);

				}

				case 1: {

					// total, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 1 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();
					IDType leftn4 = left->addVertex();

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 1 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[leftn3] = rightn2;
					mapp[leftn4] = rightn2;

					return make_shared<Rule>("Test Rule 1",left,right,mapp);

				}

				case 2: {

					// total, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 2 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();
					IDType leftn4 = left->addVertex();
					IDType leftn5 = left->addVertex();

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 2 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					IDType rightn3 = right->addVertex();

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[leftn3] = rightn2;
					mapp[leftn4] = rightn2;
					mapp[leftn5] = rightn3;

					return make_shared<Rule>("Test Rule 2",left,right,mapp);

				}

				case 3: {

					// total, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 3 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();
					IDType leftn4 = left->addVertex();
					IDType leftn5 = left->addVertex();
					IDType lefte1 = left->addEdge("A",{leftn1,leftn2});
					IDType lefte2 = left->addEdge("B",{leftn4,leftn5});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 3 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					IDType rightn3 = right->addVertex();
					IDType righte1 = right->addEdge("A",{rightn1,rightn1});
					IDType righte2 = right->addEdge("B",{rightn2,rightn3});
					right->addEdge("A",{rightn2,rightn1});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[leftn3] = rightn2;
					mapp[leftn4] = rightn2;
					mapp[leftn5] = rightn3;
					mapp[lefte1] = righte1;
					mapp[lefte2] = righte2;

					return make_shared<Rule>("Test Rule 3",left,right,mapp);

				}

				case 4: {

					// total, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 4 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();
					IDType leftn4 = left->addVertex();
					IDType lefte1 = left->addEdge("A",{leftn1,leftn3});
					IDType lefte2 = left->addEdge("A",{leftn2,leftn4});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 4 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					IDType righte1 = right->addEdge("A",{rightn1,rightn2});
					IDType righte2 = right->addEdge("A",{rightn1,rightn2});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[leftn3] = rightn2;
					mapp[leftn4] = rightn2;
					mapp[lefte1] = righte1;
					mapp[lefte2] = righte2;

					return make_shared<Rule>("Test Rule 4",left,right,mapp);

				}

				case 5: {

					// total, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 5 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType lefte1 = left->addEdge("A",{leftn1,leftn2});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 5 Right");
					IDType rightn1 = right->addVertex();
					IDType righte1 = right->addEdge("A",{rightn1,rightn1});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[lefte1] = righte1;

					return make_shared<Rule>("Test Rule 5",left,right,mapp);

				}

				case 6: {

					// partial, non-injective

					Rule_sp rule5 = getRule(5);
					AnonHypergraph_sp left = rule5->getLeft();
					IDType n1 = left->addVertex();
					IDType n2 = left->addVertex();
					left->addEdge("A",{n1,n2});
					left->addEdge("A",{n2,n1});
					left->addEdge("B",{left->beginVertices()->first,n1});

					return rule5;

				}

				case 7: {

					// partial, non-injective

					Rule_sp rule5 = getRule(5);
					AnonHypergraph_sp left = rule5->getLeft();
					IDType n1 = left->addVertex();
					IDType n2 = left->addVertex();
					IDType n3 = left->addVertex();
					IDType n4 = left->addVertex();
					left->addEdge("A",{n1,n2});
					left->addEdge("A",{n2,n3});
					left->addEdge("A",{n3,n4});
					left->addEdge("A",{n4,n1});
					left->addEdge("A",{left->beginVertices()->first,n1});
					left->addEdge("A",{n3,left->beginVertices()->first});

					return rule5;

				}

				case 8: {

					// partial, injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 8 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					left->addEdge("DP",{leftn1,leftn2});
					left->addEdge("T",{leftn1});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 8 Right");
					right->addEdge("Termination",{});

					Mapping mapp;

					return make_shared<Rule>("Test Rule 8",left,right,mapp);

				}

				case 9: {

					// partial, injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 9 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();
					IDType lefte1 = left->addEdge("A",{leftn1,leftn2});
					left->addEdge("P",{leftn2,leftn3});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 9 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					IDType rightn3 = right->addVertex();
					IDType righte1 = right->addEdge("A",{rightn1,rightn2});
					right->addEdge("A",{rightn2,rightn3});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn2;
					mapp[leftn3] = rightn3;
					mapp[lefte1] = righte1;

					return make_shared<Rule>("Test Rule 9",left,right,mapp);

				}

				case 10: {

					// partial, non-injective

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 10 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType leftn3 = left->addVertex();
					left->addEdge("A",{leftn1,leftn2});
					left->addEdge("P",{leftn2,leftn3});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 10 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					right->addEdge("A",{rightn1,rightn2});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn1;
					mapp[leftn3] = rightn2;

					return make_shared<Rule>("Test Rule 10",left,right,mapp);

				}

				case 11: {

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 11 Left");
					IDType leftn1 = left->addVertex();
					left->addVertex();

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 11 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					right->addEdge("A",{rightn2});

					Mapping mapp;
					mapp[leftn1] = rightn1;

					return make_shared<Rule>("Test Rule 11",left,right,mapp);

				}

				case 12: {

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 12 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 12 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					right->addEdge("A",{rightn2});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn2;

					return make_shared<Rule>("Test Rule 12",left,right,mapp);

				}

				case 13: {

					Hypergraph_sp left = make_shared<Hypergraph>("Test Rule 13 Left");
					IDType leftn1 = left->addVertex();
					IDType leftn2 = left->addVertex();
					IDType lefte1 = left->addEdge("A",{leftn1,leftn2});
					left->addEdge("A",{leftn1,leftn2});

					Hypergraph_sp right = make_shared<Hypergraph>("Test Rule 13 Right");
					IDType rightn1 = right->addVertex();
					IDType rightn2 = right->addVertex();
					IDType righte1 = right->addEdge("A",{rightn1,rightn2});
					right->addEdge("B",{rightn2});

					Mapping mapp;
					mapp[leftn1] = rightn1;
					mapp[leftn2] = rightn2;
					mapp[lefte1] = righte1;

					return make_shared<Rule>("Test Rule 13",left,right,mapp);

				}

				default:
					return nullptr;

			}

			return nullptr;

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// predefined graphs ///////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////

		Hypergraph_sp getGraph(size_t index) {

			switch(index) {

				case 0: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 0");
					IDType n1 = graph->addVertex();
					graph->addEdge("A",{n1});

					return graph;

				}

				case 1: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 1");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("B",{n2,n3});

					return graph;

				}

				case 2: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 2");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("A",{n2,n1});
					graph->addEdge("B",{n2,n3});
					graph->addEdge("A",{n1,n1});

					return graph;

				}

				case 3: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 3");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n1,n2});

					return graph;

				}

				case 4: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 4");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n1,n1});
					graph->addEdge("A",{n2,n2});
					graph->addEdge("B",{n1,n2});

					return graph;

				}

				case 5: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 5");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("Termination",{});

					return graph;

				}

				case 6: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 6");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n2,n2});
					graph->addEdge("DP",{n2,n1});
					graph->addEdge("T",{n2});

					return graph;

				}

				case 7: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 7");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n2,n2});
					graph->addEdge("DP",{n1,n1});

					return graph;

				}

				case 8: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 8");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n2});
					graph->addEdge("B",{n2,n2,n3});

					return graph;

				}

				case 9: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 9");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n3});
					graph->addEdge("B",{n2,n2,n4});
					graph->addEdge("C",{n2,n3});

					return graph;

				}

				case 10: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 10");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					IDType n6 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n5});
					graph->addEdge("B",{n3,n6,n4});
					graph->addEdge("C",{n2,n3});
					graph->addEdge("C",{n5,n6});

					return graph;

				}

				case 11: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 11");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					IDType n6 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n5});
					graph->addEdge("B",{n3,n6,n4});
					graph->addEdge("C",{n2,n3,n5,n6});

					return graph;

				}

				case 12: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 12");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n3});
					graph->addEdge("B",{n2,n3,n4});

					return graph;

				}

				case 13: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 13");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("B",{n3,n4});
					graph->addEdge("C",{n1,n2,n4});
					graph->addEdge("C",{n2,n3,n4});

					return graph;

				}

				case 14: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 14");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("B",{n3,n4});
					graph->addEdge("C",{n1,n2,n4,n5});
					graph->addEdge("C",{n2,n3,n4,n5});

					return graph;

				}

				case 15: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 15");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n2});
					graph->addEdge("B",{n2,n2,n1});

					return graph;

				}

				case 16: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 16");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					IDType n6 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n5});
					graph->addEdge("B",{n3,n6,n4});
					graph->addEdge("C",{n2,n3,n5});
					graph->addEdge("C",{n3,n5,n6});

					return graph;

				}

				case 17: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 17");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					graph->addEdge("A",{n1,n1,n2,n2});

					return graph;

				}

				case 18: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 18");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n3,n4});
					graph->addEdge("B",{n1,n2,n5});
					graph->addEdge("B",{n3,n4,n5});

					return graph;

				}

				case 19: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 19");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					graph->addEdge("A",{n1,n2,n3,n4});
					graph->addEdge("B",{n1,n3,n5});
					graph->addEdge("B",{n2,n4,n5});

					return graph;

				}

				case 20: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 20");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					graph->addEdge("A",{n1,n1});
					graph->addEdge("B",{n2,n3});

					return graph;

				}

				case 21: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 21");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("B",{n2,n3});
					graph->addEdge("B",{n3,n4});
					graph->addEdge("B",{n4,n5});
					graph->addEdge("B",{n5,n1});

					return graph;

				}

				case 22: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 22");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType n3 = graph->addVertex();
					IDType n4 = graph->addVertex();
					IDType n5 = graph->addVertex();
					graph->addEdge("A",{n1,n2});
					graph->addEdge("A",{n4,n5});
					graph->addEdge("B",{n1,n3});
					graph->addEdge("B",{n2,n3});
					graph->addEdge("B",{n3,n4});
					graph->addEdge("B",{n3,n5});

					return graph;

				}

				case 23: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 23");
					IDType n1 = graph->addVertex();
					graph->addEdge("A",{n1,n1});
					graph->addEdge("A",{n1,n1});

					return graph;

				}

				case 24: {

					Hypergraph_sp graph = getGraph(23);
					graph->setName("Test Graph 24");
					graph->addVertex();

					return graph;

				}

				case 25: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 25");
					IDType n1 = graph->addVertex();
					graph->addEdge("A",{n1,n1});

					return graph;

				}

				case 26: {

					Hypergraph_sp graph = getGraph(25);
					graph->setName("Test Graph 26");
					graph->addVertex();

					return graph;

				}

				case 27: {

					Hypergraph_sp graph = make_shared<Hypergraph>("Test Graph 27");
					IDType n1 = graph->addVertex();
					IDType n2 = graph->addVertex();
					IDType e1 = graph->addEdge("A",{n1,n2});
					graph->addEdge("B",{n2});

					return graph;

				}

				default:
					return nullptr;

			}

			return nullptr;

		}

	}
}

#endif
