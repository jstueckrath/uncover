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

#include <boost/test/unit_test.hpp>
#include "../subgraphs/SubgraphOrder.h"
#include "../minors/MinorOrder.h"
#include "graph_generator.h"

using namespace std;
using namespace uncover::basic_types;
using namespace uncover::graphs;
using namespace uncover::subgraphs;
using namespace uncover::minors;

namespace uncover {
	namespace test {

		//////////////////////////////////////////////////////////////////////////////////////
		// Minor Ordering Tests
		//////////////////////////////////////////////////////////////////////////////////////

		void testMinorOrder(size_t graph1, size_t graph2, bool leq12, bool leq21) {

			Hypergraph_csp g1 = getGraph(graph1);
			Hypergraph_csp g2 = getGraph(graph2);

			if(g1 == nullptr || g2 == nullptr) {
				BOOST_FAIL("A minor test was started with invalid indices!");
			}

			MinorOrder minorOrder;

			BOOST_REQUIRE_EQUAL(minorOrder.isLessOrEq(*g1,*g2),leq12);

			BOOST_REQUIRE_EQUAL(minorOrder.isLessOrEq(*g2,*g1),leq21);

		}

		BOOST_AUTO_TEST_SUITE(minor_order)

		BOOST_AUTO_TEST_CASE(minor_order1)
		{
			testMinorOrder(6,7,false,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order2)
		{
			testMinorOrder(2,2,true,true);
		}

		BOOST_AUTO_TEST_CASE(minor_order3)
		{
			testMinorOrder(8,9,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order4)
		{
			testMinorOrder(8,10,false,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order5)
		{
			testMinorOrder(8,11,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order6)
		{
			testMinorOrder(12,11,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order7)
		{
			testMinorOrder(12,10,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order8)
		{
			testMinorOrder(1,13,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order9)
		{
			testMinorOrder(1,14,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order10)
		{
			testMinorOrder(8,15,false,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order11)
		{
			testMinorOrder(8,16,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order12)
		{
			testMinorOrder(17,18,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order13)
		{
			testMinorOrder(17,19,false,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order14)
		{
			testMinorOrder(20,21,false,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order15)
		{
			testMinorOrder(23,22,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order16)
		{
			testMinorOrder(24,22,false,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order17)
		{
			testMinorOrder(25,22,true,false);
		}

		BOOST_AUTO_TEST_CASE(minor_order18)
		{
			testMinorOrder(26,22,true,false);
		}


		BOOST_AUTO_TEST_SUITE_END()

		//////////////////////////////////////////////////////////////////////////////////////
		// Subgraph Ordering Tests
		//////////////////////////////////////////////////////////////////////////////////////


	}
}

#endif
