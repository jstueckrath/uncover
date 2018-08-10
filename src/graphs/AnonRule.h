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

#ifndef ANONRULE_H_
#define ANONRULE_H_

#include "AnonHypergraph.h"
#include "Mapping.h"

namespace uncover {
	namespace graphs {

		/**
		 * AnonRule serves as a data structure of a rule object. It stores the left graph, the right graph and the
		 * mapping from the left to the right graph. Additionally it contains some manipulation and auxiliary
		 * functions. Contrary to the Rule class, it does not have a unique identifier.
		 * @see AnonHypergraph
		 * @see Mapping
		 * @see Rule
		 * @author Jan Stückrath
		 */
		class AnonRule : public basic_types::Streamable
		{

			public:

				/**
				 * Generates a new rule with the given name, left graph, right graph and mapping. If these
				 * parameters are not given, an empty string is used as name, empty graphs are used as left and
				 * right sides and an empty mapping is used as mapping. Caution, the left and right graphs are
				 * stores and not copied by this constructor.
				 * @param name the name of the rule (default: empty string)
				 * @param left the left graph of the rule (default: empty graph)
				 * @param right the right graph of the rule (default: empty graph)
				 * @param match the mapping of the left to the right graph (default: empty mapping)
				 */
				AnonRule(string name = "",
						AnonHypergraph_sp left = make_shared<AnonHypergraph>(),
						AnonHypergraph_sp right = make_shared<AnonHypergraph>(),
						Mapping const& match = Mapping());

				/**
				 * Generates a new rule for the given rule, copying its content. The new rule is an independent
				 * copy of the old rule.
				 * @param oldRule the old rule from which the data will be copied
				 */
				AnonRule(AnonRule const& oldRule);

				/**
				 * Generates a new rule using the given graph. A copy of the given graph is generated as left and
				 * as right graph and the mapping is initialized to be the identity.
				 * @param graph the graph which will be used as left and right graph
				 */
				AnonRule(AnonHypergraph const& graph);

				/**
				 * Destroys this rule.
				 */
				virtual ~AnonRule();

				/**
				 * Returns the name of this rule.
				 * @return the name of this rule
				 */
				string getName() const;

				/**
				 * Sets the name of this rule to the given string.
				 * @param name the new name of this rule
				 */
				void setName(string name);

				/**
				 * Returns a pointer to the left graph of this rule.
				 * @return a pointer to the left graph of this rule
				 */
				AnonHypergraph_sp getLeft();

				/**
				 * Returns a pointer to the right graph of this rule.
				 * @return a pointer to the right graph of this rule
				 */
				AnonHypergraph_sp getRight();

				/**
				 * Returns a pointer to the left graph of this rule.
				 * @return a pointer to the left graph of this rule
				 */
				AnonHypergraph_csp getLeft() const;

				/**
				 * Returns a pointer to the right graph of this rule.
				 * @return a pointer to the right graph of this rule
				 */
				AnonHypergraph_csp getRight() const;

				/**
				 * Sets the left graph of this rule to be the given graph. Caution, the given graph is used
				 * directly and not copied. Furthermore, the mapping and right graph are not changed.
				 * @param graph the new left graph of this rule
				 */
				void setLeft(AnonHypergraph_sp graph);

				/**
				 * Sets the right graph of this rule to be the given graph. Caution, the given graph is used
				 * directly and not copied. Furthermore, the mapping and left graph are not changed.
				 * @param graph the new right graph of this rule
				 */
				void setRight(AnonHypergraph_sp graph);

				/**
				 * Returns a reference to the mapping of this rule.
				 * @return a reference to the mapping of this rule
				 */
				Mapping& getMapping();

				/**
				 * Returns a reference to the mapping of this rule.
				 * @return a reference to the mapping of this rule
				 */
				Mapping const& getMapping() const;

				/**
				 * Replaces the mapping of this rule by the given mapping. The given mapping is copied and not
				 * used directly.
				 * @param map the new mapping of this rule
				 */
				void setMapping(Mapping const& map);

				/**
				 * Returns true, if this rule is backward applicable to the given graph, i.e. there is at least one
				 * graph to which this rule can be applied and the application results in the given graph. The given
				 * mapping is used as co-match from the right side of the rule to the given graph.
				 * Caution, this functions assumes that the match is a valid total morphism, but does not perfom
				 * checks.
				 * @param graph the graph to which the rule should be applied backwards
				 * @param match the mapping used as co-match from the right graph to the given graph
				 * @return true, iff the rule is backward applicable to the given graph using the match
				 */
				virtual bool isBackApplicable(AnonHypergraph const& graph, Mapping const& match) const;

				/**
				 * Splits this rule into a total and a partial part and stores both subrules in the given pair.
				 * The first rule p is the partial rule and the second rule t is the total rule, such that
				 * the composition t(p(x)) is the old rule.
				 * @param rulePair a reference to a pair of rule pointer, which will be set to the actual pointer
				 * 				to the split rules
				 */
				virtual void fillSplitRule(pair<shared_ptr<AnonRule>, shared_ptr<AnonRule>>& rulePair) const;

				/**
				 * Checks and returns true, iff the rule is injective. A rule is injective if two elements of the
				 * left graph are never mapped to the same element of the right graph. Note that a rule need not be
				 * total to be injective.
				 * @return true, iff the rule is injective
				 */
				bool isInjective() const;

				/**
				 * Check whether the given match is conflict-free wrt. this rule, i.e. there are no two nodes or edges
				 * with the same image under the match such that one of these nodes is deleted by the rule and the other is
				 * preserved.
				 * @param match the match to be checked; it must match all nodes of the left-hand side to some graph (which we do
				 *        not need to know)
				 * return true iff the match is conflict-free wrt. this rule
				 */
				bool isConflictFree(Mapping const& match) const;

				virtual void streamTo(std::ostream& ost) const override;

			protected:

				/**
				 * Takes two rules and changes them to be the two split rules of this rule. The first rule (partial)
				 * has to be the identity on the left side of this rule and the second rule (total) has to be a copy
				 * of this rule.
				 * @param partial the identity rule on the left graph of this rule
				 * @param total a copy of this rule
				 */
				void adjustSplitRule(AnonRule& partial, AnonRule& total) const;

				/**
				 * Stores the name of this rule.
				 */
				string name;

				/**
				 * Stores a pointer to the left graph of this rule.
				 */
				AnonHypergraph_sp left;

				/**
				 * Stores a pointer to the right graph of this rule.
				 */
				AnonHypergraph_sp right;

				/**
				 * Stores the mapping from the left to the right graph of this rule.
				 */
				Mapping interface;

		};

		/**
		 * Alias for a shared pointer to an AnonRule.
		 */
		typedef shared_ptr<AnonRule> AnonRule_sp;

		/**
		 * Alias for a shared pointer to a constant AnonRule.
		 */
		typedef shared_ptr<const AnonRule> AnonRule_csp;

	} /* namespace graphs */
} /* namespace uncover */

#endif /* ANONRULE_H_ */
