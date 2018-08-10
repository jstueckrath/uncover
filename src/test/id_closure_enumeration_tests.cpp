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
#include "../rule_engine/IDClosureEnumerator.h"
#include "../basic_types/standard_operators.h"

using namespace std;
using namespace uncover::rule_engine;
using namespace uncover::basic_types;
using namespace uncover::graphs;

namespace uncover {
	namespace test {

		BOOST_AUTO_TEST_SUITE(id_closure_enumeration_suite)

		BOOST_AUTO_TEST_CASE(id_closure1)
		{

			unordered_set<IDType> allIDs;
			allIDs.insert(1);
			allIDs.insert(2);
			allIDs.insert(3);
			allIDs.insert(4);
			IDPartition partition;
			unordered_set<IDType> p1;
			p1.insert(1);
			p1.insert(2);
			p1.insert(3);
			partition.insert(p1);

			IDClosureEnumerator enumerator(allIDs,partition);
			size_t counter = 0;
			while(!enumerator.ended()) {
				//BOOST_TEST_MESSAGE("obtained the following partition: " << *enumerator);
				++enumerator;
				++counter;
			}

			BOOST_REQUIRE_EQUAL(counter,10);
		}

		BOOST_AUTO_TEST_SUITE_END()

	}
}

#endif
