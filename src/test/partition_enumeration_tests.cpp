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
#include "../basic_types/IDPartitionEnumerator.h"
#include "../basic_types/standard_operators.h"

using namespace std;
using namespace uncover::basic_types;

namespace uncover {
	namespace test {

		BOOST_AUTO_TEST_SUITE(partition_enumeration_suite)

		BOOST_AUTO_TEST_CASE(id_enumeration)
		{
			vector<IDType> testIDs = {1,2,3,5,5};

			size_t counter = 0;
			for(IDPartitionEnumerator enumerator(testIDs); !enumerator.ended(); ++enumerator) {
				++counter;
				//BOOST_TEST_MESSAGE(counter << ": " << *enumerator);
			}
			//BOOST_TEST_MESSAGE("Finished the loop without problems. Produced: " << counter << " equivalences.");
			BOOST_REQUIRE_EQUAL(counter, 15);
		}

		BOOST_AUTO_TEST_CASE(transitive_closure1)
		{
			IDPartition part1 = {{1},{3,4},{7}};
			IDPartition part2 = {{2},{5},{6}};
			IDPartition shouldBeResult = {{1},{2},{3,4},{5},{6},{7}};

			IDPartition closure = idpart_transitive_closure(part1, part2);

			//BOOST_TEST_MESSAGE("Transitive closure of " << part1 << " and " << part2 << " resulted in: " << closure);
			BOOST_REQUIRE_EQUAL(shouldBeResult,closure);
		}

		BOOST_AUTO_TEST_SUITE_END()

	}
}

#endif
