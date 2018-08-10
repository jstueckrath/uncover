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

#define SHOW_TPOC1 false
#define SHOW_TPOC2 false
#define SHOW_TPOC3 false
#define SHOW_TPOC4 false
#define SHOW_TPOC5 false
#define SHOW_TPOC6 false
#define SHOW_TPOC7 false

#define SHOW_SPOC1 false
#define SHOW_SPOC2 false
#define SHOW_SPOC3 false
#define SHOW_SPOC4 false
#define SHOW_SPOC5 false
#define SHOW_SPOC6 false
#define SHOW_SPOC7 false
#define SHOW_SPOC8 false

#define SHOW_MPOC1 false
#define SHOW_MPOC2 false
#define SHOW_MPOC3 false
#define SHOW_MPOC4 false
#define SHOW_MPOC5 false
#define SHOW_MPOC6 false

#include <boost/test/unit_test.hpp>
#include "../rule_engine/TotalPOCEnumerator.h"
#include "../basic_types/standard_operators.h"
#include "../subgraphs/SubgraphPOCEnumerator.h"
#include "../subgraphs/SubgraphRulePreparer.h"
#include "../minors/MinorPOCEnumerator.h"
#include "../minors/MinorRulePreparer.h"
#include "graph_generator.h"

using namespace std;
using namespace uncover::rule_engine;
using namespace uncover::basic_types;
using namespace uncover::graphs;
using namespace uncover::subgraphs;
using namespace uncover::minors;
using namespace uncover::analysis;

namespace uncover {
	namespace test {

		template<typename E>
		void printEnumeratorValue(E const& enumerator, size_t counter) {
			BOOST_TEST_MESSAGE("******************************");
			BOOST_TEST_MESSAGE("enumeration " << counter);
			BOOST_TEST_MESSAGE("*****");
			BOOST_TEST_MESSAGE("with match " << enumerator.getCurrentCoMatch());
			BOOST_TEST_MESSAGE("*****");
			BOOST_TEST_MESSAGE("obtained the following graph:\n" << *enumerator.getGraph());
			BOOST_TEST_MESSAGE("*****");
			BOOST_TEST_MESSAGE("obtained the following mapping:\n" << *enumerator.getMapping());
			BOOST_TEST_MESSAGE("******************************");
		}

		void printEnumeratorValue(SubgraphPOCEnumerator const& enumerator, size_t counter) {
			BOOST_TEST_MESSAGE("******************************");
			BOOST_TEST_MESSAGE("enumeration " << counter);
			BOOST_TEST_MESSAGE("*****");
			BOOST_TEST_MESSAGE("obtained the following graph:\n" << **enumerator);
			BOOST_TEST_MESSAGE("******************************");
		}

		void printEnumeratorValue(MinorPOCEnumerator const& enumerator, size_t counter) {
			BOOST_TEST_MESSAGE("******************************");
			BOOST_TEST_MESSAGE("enumeration " << counter);
			BOOST_TEST_MESSAGE("*****");
			BOOST_TEST_MESSAGE("obtained the following graph:\n" << **enumerator);
			BOOST_TEST_MESSAGE("******************************");
		}

		template<typename E>
		void executeTestI(E& enumerator, size_t shouldBePOCsInj, bool print) {

			size_t actualPOCsInj = 0;
			while(!enumerator.ended()) {
				++actualPOCsInj;
				if(print) printEnumeratorValue(enumerator,actualPOCsInj);
				++enumerator;
			}

			BOOST_REQUIRE_EQUAL(actualPOCsInj,shouldBePOCsInj);

		}

		template<typename E>
		void executeTestNI(E& enumerator, size_t shouldBePOCsNonInj, bool print) {

			size_t actualPOCsNonInj = 0;
			while(!enumerator.ended()) {
				++actualPOCsNonInj;
				if(print) printEnumeratorValue(enumerator,actualPOCsNonInj);
				++enumerator;
			}

			BOOST_REQUIRE_EQUAL(actualPOCsNonInj,shouldBePOCsNonInj);

		}

		void performTPOCTest(Rule_csp rule, Hypergraph_csp target, bool injective, size_t count, bool print) {

			TotalPOCEnumerator enumerator(rule,target,injective);
			if(injective) {
				executeTestI<TotalPOCEnumerator>(enumerator,count,print);
			} else {
				executeTestNI<TotalPOCEnumerator>(enumerator,count,print);
			}

		}

		void performSPOCTest(Rule_csp rule,
				Hypergraph_csp target,
				bool injective,
				size_t count,
				bool print,
				PreparedGTS_csp gts = nullptr) {

			SubgraphPOCEnumerator enumerator(injective,-1);
			enumerator.setPreparedGTS(gts);
			enumerator.resetWith(rule,target);
			if(injective) {
				executeTestI<SubgraphPOCEnumerator>(enumerator,count,print);
			} else {
				executeTestNI<SubgraphPOCEnumerator>(enumerator,count,print);
			}

		}

		void performMPOCTest(Rule_csp rule,
				Hypergraph_csp target,
				size_t count,
				bool print,
				PreparedGTS_csp gts = nullptr) {

			MinorPOCEnumerator enumerator;
			enumerator.setPreparedGTS(gts);
			enumerator.resetWith(rule,target);
			executeTestNI<MinorPOCEnumerator>(enumerator,count,print);

		}

		//////////////////////////////////////////////////////////////////////////////////////
		// Total POC Tests
		//////////////////////////////////////////////////////////////////////////////////////

		BOOST_AUTO_TEST_SUITE(total_poc_enumeration)

		BOOST_AUTO_TEST_CASE(total_poc_enumerator1)
		{

			Rule_csp rule = getRule(0);

			Hypergraph_csp target = getGraph(0);

			performTPOCTest(rule,target,false,10,SHOW_TPOC1);

			performTPOCTest(rule,target,true,3,SHOW_TPOC1);

		}

		BOOST_AUTO_TEST_CASE(total_poc_enumerator2)
		{

			Rule_csp rule = getRule(1);

			Hypergraph_csp target = getGraph(0);

			performTPOCTest(rule,target,false,25,SHOW_TPOC2);

			performTPOCTest(rule,target,true,0,SHOW_TPOC2);

		}

		BOOST_AUTO_TEST_CASE(total_poc_enumerator3)
		{

			Rule_csp rule = getRule(2);

			Hypergraph_csp target = getGraph(1);

			performTPOCTest(rule,target,false,847,SHOW_TPOC3);

			// maybe add injective test at a later time

		}

		BOOST_AUTO_TEST_CASE(total_poc_enumerator4)
		{

			Rule_csp rule = getRule(3);

			Hypergraph_csp target = getGraph(2);

			performTPOCTest(rule,target,false,9,SHOW_TPOC4);

			performTPOCTest(rule,target,true,4,SHOW_TPOC4);

		}

		BOOST_AUTO_TEST_CASE(total_poc_enumerator5)
		{

			Rule_csp rule = getRule(4);

			Hypergraph_csp target = getGraph(3);

			performTPOCTest(rule,target,false,1,SHOW_TPOC5);

			performTPOCTest(rule,target,true,0,SHOW_TPOC5);

		}

		BOOST_AUTO_TEST_CASE(total_poc_enumerator6)
		{

			Rule_csp rule = getRule(1);

			Hypergraph_csp target = getGraph(3);

			performTPOCTest(rule,target,false,68,SHOW_TPOC6);

			performTPOCTest(rule,target,true,8,SHOW_TPOC6);

		}

		BOOST_AUTO_TEST_CASE(total_poc_enumerator7)
		{

			Rule_csp rule = getRule(5);

			Hypergraph_csp target = getGraph(4);

			performTPOCTest(rule,target,false,6,SHOW_TPOC7);

			performTPOCTest(rule,target,true,4,SHOW_TPOC7);

		}

		BOOST_AUTO_TEST_SUITE_END()

		//////////////////////////////////////////////////////////////////////////////////////
		// Subgraph POC Tests
		//////////////////////////////////////////////////////////////////////////////////////

		BOOST_AUTO_TEST_SUITE(subgraph_poc_enumeration)

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator1)
		{

			Rule_csp rule = getRule(5);

			Hypergraph_csp target = getGraph(4);

			performSPOCTest(rule,target,false,6,SHOW_SPOC1);

			performSPOCTest(rule,target,true,4,SHOW_SPOC1);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator2)
		{

			Rule_csp rule = getRule(1);

			Hypergraph_csp target = getGraph(3);

			performSPOCTest(rule,target,false,68,SHOW_SPOC2);

			performSPOCTest(rule,target,true,8,SHOW_SPOC2);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator3)
		{

			Rule_csp rule = getRule(1);

			Hypergraph_csp target = getGraph(0);

			performSPOCTest(rule,target,false,25,SHOW_SPOC3);

			performSPOCTest(rule,target,true,0,SHOW_SPOC3);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator4)
		{

			Rule_csp rule = getRule(6);

			Hypergraph_csp target = getGraph(4);

			performSPOCTest(rule,target,false,18,SHOW_SPOC4);

			performSPOCTest(rule,target,true,4,SHOW_SPOC4);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator5)
		{

			Rule_csp rule = getRule(7);

			Hypergraph_csp target = getGraph(4);

			performSPOCTest(rule,target,false,216,SHOW_SPOC5);

			performSPOCTest(rule,target,true,4,SHOW_SPOC5);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator6)
		{

			GTS gts;
			gts.addStdRule(getRule(11));
			SubgraphRulePreparer_sp preparer = make_shared<SubgraphRulePreparer>();
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(gts, preparer);

			BOOST_REQUIRE_EQUAL(prepGTS->getStdRuleCount(),4);

			// the prepared GTS now contains four rules which have 1-2 nodes and 0-1 edges
			Rule_csp rule_n2e1 = nullptr;
			Rule_csp rule_n2e0 = nullptr;
			Rule_csp rule_n1e1 = nullptr;
			Rule_csp rule_n1e0 = nullptr;
			for(auto it = prepGTS->getStdRules().cbegin(); it != prepGTS->getStdRules().cend(); ++it) {
				if(it->second->getRight()->getVertexCount() == 2) {
					if(it->second->getRight()->getEdgeCount() == 1) {
						rule_n2e1 = it->second;
					} else {
						rule_n2e0 = it->second;
					}
				} else {
					if(it->second->getRight()->getEdgeCount() == 1) {
						rule_n1e1 = it->second;
					} else {
						rule_n1e0 = it->second;
					}
				}
			}

			BOOST_REQUIRE(rule_n2e1 != nullptr);
			BOOST_REQUIRE(rule_n2e0 != nullptr);
			BOOST_REQUIRE(rule_n1e1 != nullptr);
			BOOST_REQUIRE(rule_n1e0 != nullptr);

			Hypergraph_csp target = getGraph(0);

			performSPOCTest(rule_n2e1,target,false,0,SHOW_SPOC6,prepGTS);
			performSPOCTest(rule_n2e1,target,false,0,SHOW_SPOC6,nullptr);
			performSPOCTest(rule_n2e1,target,true,0,SHOW_SPOC6);

			performSPOCTest(rule_n2e0,target,false,0,SHOW_SPOC6,prepGTS);
			performSPOCTest(rule_n2e0,target,false,0,SHOW_SPOC6,nullptr);
			performSPOCTest(rule_n2e0,target,true,0,SHOW_SPOC6);

			performSPOCTest(rule_n1e1,target,false,1,SHOW_SPOC6,prepGTS);
			performSPOCTest(rule_n1e1,target,false,2,SHOW_SPOC6,nullptr);
			performSPOCTest(rule_n1e1,target,true,1,SHOW_SPOC6);

			performSPOCTest(rule_n1e0,target,false,0,SHOW_SPOC6,prepGTS);
			performSPOCTest(rule_n1e0,target,false,0,SHOW_SPOC6,nullptr);
			performSPOCTest(rule_n1e0,target,true,0,SHOW_SPOC6);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator7)
		{

			GTS gts;
			gts.addStdRule(getRule(12));
			SubgraphRulePreparer_sp preparer = make_shared<SubgraphRulePreparer>();
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(gts, preparer);

			BOOST_REQUIRE_EQUAL(prepGTS->getStdRuleCount(),2);

			// the prepared GTS now contains two rules which have 1-2 nodes and 1 edge
			Rule_csp rule_n2e1 = nullptr;
			Rule_csp rule_n1e1 = nullptr;
			for(auto it = prepGTS->getStdRules().cbegin(); it != prepGTS->getStdRules().cend(); ++it) {
				if(it->second->getRight()->getVertexCount() == 2) {
					rule_n2e1 = it->second;
				} else {
					rule_n1e1 = it->second;
				}
			}

			BOOST_REQUIRE(rule_n2e1 != nullptr);
			BOOST_REQUIRE(rule_n1e1 != nullptr);

			Hypergraph_csp target = getGraph(0);

			performSPOCTest(rule_n2e1,target,false,1,SHOW_SPOC7,prepGTS);
			performSPOCTest(rule_n2e1,target,false,1,SHOW_SPOC7,nullptr);
			performSPOCTest(rule_n2e1,target,true,0,SHOW_SPOC7);

			performSPOCTest(rule_n1e1,target,false,1,SHOW_SPOC7,prepGTS);
			performSPOCTest(rule_n1e1,target,false,1,SHOW_SPOC7,nullptr);
			performSPOCTest(rule_n1e1,target,true,1,SHOW_SPOC7);

		}

		BOOST_AUTO_TEST_CASE(subgraph_poc_enumerator8)
		{

			GTS gts;
			gts.addStdRule(getRule(13));
			SubgraphRulePreparer_sp preparer = make_shared<SubgraphRulePreparer>();
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(gts, preparer);

			BOOST_REQUIRE_EQUAL(prepGTS->getStdRuleCount(),3);

			// the prepared GTS now contains three rules which have 1-2 nodes and 1-2 edges
			Rule_csp rule_n2e2 = nullptr;
			Rule_csp rule_n2e1 = nullptr;
			Rule_csp rule_n1e1 = nullptr;
			for(auto it = prepGTS->getStdRules().cbegin(); it != prepGTS->getStdRules().cend(); ++it) {
				if(it->second->getRight()->getVertexCount() == 2) {
					if(it->second->getRight()->getEdgeCount() == 2) {
						rule_n2e2 = it->second;
					} else {
						rule_n2e1 = it->second;
					}
				} else {
					rule_n1e1 = it->second;
				}
			}

			BOOST_REQUIRE(rule_n2e2 != nullptr);
			BOOST_REQUIRE(rule_n2e1 != nullptr);
			BOOST_REQUIRE(rule_n1e1 != nullptr);

			Hypergraph_csp target = getGraph(27);

			performSPOCTest(rule_n2e2,target,false,1,SHOW_SPOC8,prepGTS);
			performSPOCTest(rule_n2e2,target,false,1,SHOW_SPOC8,nullptr);
			performSPOCTest(rule_n2e2,target,true,1,SHOW_SPOC8);

			performSPOCTest(rule_n2e1,target,false,2,SHOW_SPOC8,prepGTS);
			performSPOCTest(rule_n2e1,target,false,4,SHOW_SPOC8,nullptr);
			performSPOCTest(rule_n2e1,target,true,1,SHOW_SPOC8);

			performSPOCTest(rule_n1e1,target,false,1,SHOW_SPOC8,prepGTS);
			performSPOCTest(rule_n1e1,target,false,2,SHOW_SPOC8,nullptr);
			performSPOCTest(rule_n1e1,target,true,1,SHOW_SPOC8);

		}

		BOOST_AUTO_TEST_SUITE_END()

		//////////////////////////////////////////////////////////////////////////////////////
		// Minor POC Tests
		//////////////////////////////////////////////////////////////////////////////////////

		BOOST_AUTO_TEST_SUITE(minor_poc_enumeration)

		BOOST_AUTO_TEST_CASE(minor_poc_enumerator1)
		{

			Rule_csp rule = getRule(8);

			Hypergraph_csp target = getGraph(5);

			performMPOCTest(rule,target,2,SHOW_MPOC1);

		}

		BOOST_AUTO_TEST_CASE(minor_poc_enumerator2)
		{

			Rule_csp rule = getRule(9);

			Hypergraph_csp target = getGraph(6);

			performMPOCTest(rule,target,0,SHOW_MPOC2);

		}

		BOOST_AUTO_TEST_CASE(minor_poc_enumerator3)
		{

			Rule_csp rule = getRule(10);

			Hypergraph_csp target = getGraph(6);

			performMPOCTest(rule,target,9,SHOW_MPOC3);

		}

		BOOST_AUTO_TEST_CASE(minor_poc_enumerator4)
		{

			GTS gts;
			gts.addStdRule(getRule(11));
			MinorRulePreparer_sp preparer = make_shared<MinorRulePreparer>();
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(gts, preparer);

			BOOST_REQUIRE_EQUAL(prepGTS->getStdRuleCount(),4);

			// the prepared GTS now contains four rules which have 1-2 nodes and 0-1 edges
			Rule_csp rule_n2e1 = nullptr;
			Rule_csp rule_n2e0 = nullptr;
			Rule_csp rule_n1e1 = nullptr;
			Rule_csp rule_n1e0 = nullptr;
			for(auto it = prepGTS->getStdRules().cbegin(); it != prepGTS->getStdRules().cend(); ++it) {
				if(it->second->getRight()->getVertexCount() == 2) {
					if(it->second->getRight()->getEdgeCount() == 1) {
						rule_n2e1 = it->second;
					} else {
						rule_n2e0 = it->second;
					}
				} else {
					if(it->second->getRight()->getEdgeCount() == 1) {
						rule_n1e1 = it->second;
					} else {
						rule_n1e0 = it->second;
					}
				}
			}

			BOOST_REQUIRE(rule_n2e1 != nullptr);
			BOOST_REQUIRE(rule_n2e0 != nullptr);
			BOOST_REQUIRE(rule_n1e1 != nullptr);
			BOOST_REQUIRE(rule_n1e0 != nullptr);

			Hypergraph_csp target = getGraph(0);

			performMPOCTest(rule_n2e1,target,0,SHOW_MPOC4,prepGTS);
			performMPOCTest(rule_n2e1,target,0,SHOW_MPOC4,nullptr);

			performMPOCTest(rule_n2e0,target,0,SHOW_MPOC4,prepGTS);
			performMPOCTest(rule_n2e0,target,0,SHOW_MPOC4,nullptr);

			performMPOCTest(rule_n1e1,target,1,SHOW_MPOC4,prepGTS);
			performMPOCTest(rule_n1e1,target,2,SHOW_MPOC4,nullptr);

			performMPOCTest(rule_n1e0,target,0,SHOW_MPOC4,prepGTS);
			performMPOCTest(rule_n1e0,target,0,SHOW_MPOC4,nullptr);

		}

		BOOST_AUTO_TEST_CASE(minor_poc_enumerator5)
		{

			GTS gts;
			gts.addStdRule(getRule(12));
			MinorRulePreparer_sp preparer = make_shared<MinorRulePreparer>();
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(gts, preparer);

			BOOST_REQUIRE_EQUAL(prepGTS->getStdRuleCount(),2);

			// the prepared GTS now contains two rules which have 1-2 nodes and 1 edge
			Rule_csp rule_n2e1 = nullptr;
			Rule_csp rule_n1e1 = nullptr;
			for(auto it = prepGTS->getStdRules().cbegin(); it != prepGTS->getStdRules().cend(); ++it) {
				if(it->second->getRight()->getVertexCount() == 2) {
					rule_n2e1 = it->second;
				} else {
					rule_n1e1 = it->second;
				}
			}

			BOOST_REQUIRE(rule_n2e1 != nullptr);
			BOOST_REQUIRE(rule_n1e1 != nullptr);

			Hypergraph_csp target = getGraph(0);

			performMPOCTest(rule_n2e1,target,1,SHOW_MPOC5,prepGTS);
			performMPOCTest(rule_n2e1,target,1,SHOW_MPOC5,nullptr);

			performMPOCTest(rule_n1e1,target,1,SHOW_MPOC5,prepGTS);
			performMPOCTest(rule_n1e1,target,1,SHOW_MPOC5,nullptr);

		}

		BOOST_AUTO_TEST_CASE(minor_poc_enumerator6)
		{

			GTS gts;
			gts.addStdRule(getRule(13));
			MinorRulePreparer_sp preparer = make_shared<MinorRulePreparer>();
			PreparedGTS_sp prepGTS = make_shared<PreparedGTS>(gts, preparer);

			BOOST_REQUIRE_EQUAL(prepGTS->getStdRuleCount(),4);

			// the prepared GTS now contains three rules which have 1-2 nodes and 1-2 edges
			Rule_csp rule_n2e2 = nullptr;
			Rule_csp rule_n2e1 = nullptr;
			Rule_csp rule_n1e1d = nullptr;
			Rule_csp rule_n1e1c = nullptr;
			for(auto it = prepGTS->getStdRules().cbegin(); it != prepGTS->getStdRules().cend(); ++it) {
				if(it->second->getRight()->getVertexCount() == 2) {
					if(it->second->getRight()->getEdgeCount() == 2) {
						rule_n2e2 = it->second;
					} else {
						rule_n2e1 = it->second;
					}
				} else {
					if(it->second->getMapping().isInjective()) {
						rule_n1e1d = it->second;
					} else {
						rule_n1e1c = it->second;
					}
				}
			}

			BOOST_REQUIRE(rule_n2e2 != nullptr);
			BOOST_REQUIRE(rule_n2e1 != nullptr);
			BOOST_REQUIRE(rule_n1e1d != nullptr);
			BOOST_REQUIRE(rule_n1e1c != nullptr);

			Hypergraph_csp target = getGraph(27);

			performMPOCTest(rule_n2e2,target,1,SHOW_MPOC6,prepGTS);
			performMPOCTest(rule_n2e2,target,1,SHOW_MPOC6,nullptr);

			performMPOCTest(rule_n2e1,target,2,SHOW_MPOC6,prepGTS);
			performMPOCTest(rule_n2e1,target,4,SHOW_MPOC6,nullptr);

			performMPOCTest(rule_n1e1d,target,1,SHOW_MPOC6,prepGTS);
			performMPOCTest(rule_n1e1d,target,2,SHOW_MPOC6,nullptr);

			performMPOCTest(rule_n1e1d,target,1,SHOW_MPOC6,prepGTS);
			performMPOCTest(rule_n1e1d,target,2,SHOW_MPOC6,nullptr);

		}

		BOOST_AUTO_TEST_SUITE_END()

	}
}

#endif


