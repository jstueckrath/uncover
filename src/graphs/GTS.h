/***************************************************************************
 *   Copyright (C) 2005 by SZS                                             *
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

#ifndef DATA_STRUCTURESGTS_H
#define DATA_STRUCTURESGTS_H

#include "Rule.h"
#include "UQRule.h"
#include "Hypergraph.h"

namespace uncover {
	namespace graphs {

		/**
		 * The GTS class is the data structure for a graph transformation system. It stores initial graphs, standard rules
		 * and universally quantified rules.
		 * @author Jan Stückrath
		 */
		class GTS : public basic_types::Streamable
		{

			public:

				/**
				 * An alias for the map storing standard rules.
				 */
				typedef unordered_map<IDType, Rule_sp> RuleMap;

				/**
				 * An alias for the map storing universally quantified rules.
				 */
				typedef unordered_map<IDType, UQRule_sp> UQRuleMap;

				/**
				 * Generates a new GTS with the given name, standard rule set, quantified rule set and initial graphs.
				 * @param name a sting representing the name of this GTS
				 * @param rules a map of the standard rules of this GTS
				 * @param uqRules a map of the quantified rules of this GTS.
				 * @param initialGraphs a collection of initial graphs of this GTS
				 */
				GTS(string name = "",
						RuleMap const& rules = (RuleMap()),
						UQRuleMap const& uqRules = (UQRuleMap()),
						vector<Hypergraph_sp> const& initialGraphs = vector<Hypergraph_sp>());

				/**
				 * Destroys this GTS instance.
				 */
				~GTS();

				/**
				 * Returns a reference to the collection of initial graphs of this GTS.
				 * @return a reference to the collection of initial graphs of this GTS
				 */
				vector<Hypergraph_sp> const& getInitialGraphs() const;

				/**
				 * Adds the given graph to the collection of initial graphs of this GTS.
				 * @param graph the graph to be added to the initial graphs
				 * @return true, iff the graph was added (i.e. it was not null)
				 */
				bool addInitialGraph(Hypergraph_sp graph);

				/**
				 * Returns the number of initial graphs of this GTS.
				 * @return the number of initial graphs of this GTS
				 */
				size_t getInitialCount() const;

				/**
				 * Returns the name of this GTS.
				 * @return the name of this GTS
				 */
				string getName() const;

				/**
				 * Returns a reference to the standard rules of this GTS.
				 * @return a reference to the standard rules of this GTS
				 */
				RuleMap const& getStdRules() const;

				/**
				 * Adds the given Rule to the standard rules of this GTS.
				 * @param rule the Rule to be added
				 * @return true, iff the rule was added (i.e. it was not null and not already added before)
				 */
				bool addStdRule(Rule_sp rule);

				/**
				 * Returns the number of standard rules stored in this GTS.
				 * @return the number of standard rules stored in this GTS
				 */
				size_t getStdRuleCount() const;

				/**
				 * Returns a reference to the universally quantified rules of this GTS.
				 * @return a reference to the quantified rules of this GTS
				 */
				UQRuleMap const& getUQRules() const;

				/**
				 * Adds the given UQRule to the quantified rules of this GTS.
				 * @param rule the UQRule to be added
				 * @return true, iff the rule was added (i.e. it was not null and not already added before)
				 */
				bool addUQRule(UQRule_sp rule);

				/**
				 * Returns the number of universally quantified rules stored in this GTS.
				 * @return the number of quantified rules of this GTS
				 */
				size_t getUQRuleCount() const;

				/**
				 * Prints a string representation of this GTS to the given steam.
				 * @param ost the stream on which the string will be printed
				 */
				virtual void streamTo(std::ostream& ost) const;

			protected:

				/**
				 * Stores the name of the GTS.
				 */
				string name;

				/**
				 * Stores all initial graphs of this GTS.
				 */
				vector<Hypergraph_sp> initialGraphs;

				/**
				 * Stores all standard rules of this GTS, indexed by ID.
				 */
				RuleMap stdRules;

				/**
				 * Stores all quantified rules of this GTS, indexed by ID.
				 */
				UQRuleMap uqRules;

		};

		/**
		 * Alias for a shared pointer to a GTS object.
		 */
		typedef shared_ptr<GTS> GTS_sp;

		/**
		 * Alias for a shared pointer to a constant GTS object.
		 */
		typedef shared_ptr<const GTS> GTS_csp;

	}
}

#endif
