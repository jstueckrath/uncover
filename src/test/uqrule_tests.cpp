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
#include "../graphs/UQRule.h"
#include "../graphs/Hypergraph.h"
//#include "../basic_types/standard_operators.h"

using namespace std;
using namespace uncover::basic_types;
using namespace uncover::graphs;

namespace uncover {
	namespace test {

		BOOST_AUTO_TEST_SUITE(uqrule_tests)

		BOOST_AUTO_TEST_CASE(instantiation_test1)
		{
			UQRule rule("test rule", make_shared<Hypergraph>(), make_shared<Hypergraph>(), Mapping());

			// generate main rule
			IDType vidl1 = rule.getMainRule().getLeft()->addVertex();
			IDType vidl2 = rule.getMainRule().getLeft()->addVertex();
			IDType eidl1 = rule.getMainRule().getLeft()->addEdge("A",{vidl1,vidl2});
			IDType vidr1 = rule.getMainRule().getRight()->addVertex();
			IDType vidr2 = rule.getMainRule().getRight()->addVertex();
			IDType eidr1 = rule.getMainRule().getRight()->addEdge("A",{vidr1,vidr2});
			rule.getMainRule().getMapping()[vidl1] = vidr1;
			rule.getMainRule().getMapping()[vidl2] = vidr2;
			rule.getMainRule().getMapping()[eidl1] = eidr1;

			// generate universal quantification
			Rule quantRule("test quantification", make_shared<Hypergraph>(), make_shared<Hypergraph>());
			IDType vidl1q = quantRule.getLeft()->addVertex();
			IDType vidl2q = quantRule.getLeft()->addVertex();
			IDType vidl3q = quantRule.getLeft()->addVertex();
			IDType eidl1q = quantRule.getLeft()->addEdge("A",{vidl1q,vidl2q});
			IDType eidl2q = quantRule.getLeft()->addEdge("B",{vidl1q,vidl3q});
			IDType vidr1q = quantRule.getRight()->addVertex();
			IDType vidr2q = quantRule.getRight()->addVertex();
			IDType vidr4q = quantRule.getRight()->addVertex();
			IDType eidr1q = quantRule.getRight()->addEdge("A",{vidr1q,vidr2q});
			IDType eidr3q = quantRule.getRight()->addEdge("B",{vidr2q,vidr4q});
			quantRule.getMapping()[vidl1q] = vidr1q;
			quantRule.getMapping()[vidl2q] = vidr2q;
			quantRule.getMapping()[eidl1q] = eidr1q;
			Mapping quantMap;
			quantMap[vidl1q] = vidl1;
			quantMap[vidl2q] = vidl2;
			quantMap[eidl1q] = eidl1;

			rule.addQuantification(pair<Rule, Mapping>(quantRule, quantMap));

			//BOOST_TEST_MESSAGE("Main Rule:\n" << rule.getMainRule());
			vector<UQRule::Quantification>& quanties = rule.getQuantifications();
			//BOOST_TEST_MESSAGE("Quantification:\n(" << quanties.back().first << ";" << quanties.back().second << ")");

			UQRuleInstance_sp instantiation = rule.createInstance({1});

			//BOOST_TEST_MESSAGE(*instantiation);

			// there has to be an assertion for this to be a real test
		}

		BOOST_AUTO_TEST_SUITE_END()

	}
}

#endif



