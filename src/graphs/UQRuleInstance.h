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

#ifndef UQRULEINSTANCE_H_
#define UQRULEINSTANCE_H_

#include "AnonRule.h"

namespace uncover {
	namespace graphs {

		/**
		 * The UQRuleInstance is a rule which is generated when UQRules are instantiated. Contrary to normal
		 * Rules, the UQRuleInstance might be not always backward applicable if not the whole neighborhood
		 * of unversally quantified nodes are matched.
		 * @see UQRule
		 * @author Jan Stückrath
		 */
		class UQRuleInstance: public AnonRule {

			friend class UQRule;

			public:

				/**
				 * Creates a new UQRuleInstance from the given AnonRule and with the given sets of universally
				 * quantified vertices.
				 * @param rule the rule encapsulated (its content is copied) in this UQRuleInstances
				 * @param leftUQVertices the set of all IDs of universally quantified vertices of the left-hand
				 * 				side of the given rule
				 * @param rightUQVertices the set of all images of universally quantified vertices of the left-hand
				 * 				side along the given rule
				 */
				UQRuleInstance(AnonRule const& rule,
						unordered_set<IDType> const& leftUQVertices,
						unordered_set<IDType> const& rightUQVertices);

				/**
				 * Creates a new UQRuleInstance from the given rule parts and with the given sets of universally
				 * quantified vertices.
				 * @param name the name of this UQRuleInstance
				 * @param leftGraph the left-hand side of this UQRuleInstance
				 * @param rightGraph the right-hand side of this UQRuleInstance
				 * @param mapping a Mapping of the left-hand side to the right-hand side
				 * @param leftUQVertices the set of all IDs of universally quantified vertices of the left-hand
				 * 				side
				 * @param rightUQVertices the set of all images of universally quantified vertices of the left-hand
				 * 				side along the given Mapping
				 */
				UQRuleInstance(string name,
						AnonHypergraph_sp leftGraph,
						AnonHypergraph_sp rightGraph,
						Mapping const& mapping,
						unordered_set<IDType> const& leftUQVertices,
						unordered_set<IDType> const& rightUQVertices);

				/**
				 * Creates a new UQRuleInstance which is the identity on the given graph and used the given set
				 * as universally quantified vertices.
				 * @param graph the graph which will be left- and right-hand side of the UQRuleInstance
				 * @param uqVertices the set of universally quantified vertices of the left- and right-hand side
				 */
				UQRuleInstance(AnonHypergraph const& graph, unordered_set<IDType> const& uqVertices);

				/**
				 * Destroys this UQRuleInstance
				 */
				virtual ~UQRuleInstance();

				/**
				 * Returns the set of universally quantified vertices of the left-hand side.
				 * @return the set of universally quantified vertices of the left-hand side
				 */
				unordered_set<IDType>& getLeftUQVertices();

				/**
				 * Returns the set of universally quantified vertices of the left-hand side.
				 * @return the set of universally quantified vertices of the left-hand side
				 */
				unordered_set<IDType> const& getLeftUQVertices() const;

				/**
				 * Returns the set of images of universally quantified vertices in the right-hand side.
				 * @return the set of images of universally quantified vertices in the right-hand side
				 */
				unordered_set<IDType>& getRightUQVertices();

				/**
				 * Returns the set of images of universally quantified vertices in the right-hand side.
				 * @return the set of images of universally quantified vertices in the right-hand side
				 */
				unordered_set<IDType> const& getRightUQVertices() const;

				/**
				 * Checks whether this UQRuleInstance is backward applicable or not. It is backward applicable only
				 * if all edges in the given graph which are incident to images of universally quantified vertices
				 * of the right-hand side are in the matching (i.e. have a preimage in the right-hand side).
				 * @param graph the graph to which the this instance is matched backwardly
				 * @param match the match to be checked
				 * @return true iff the rule is backward applicable
				 */
				virtual bool isBackApplicable(AnonHypergraph const& graph, Mapping const& match) const override;

				virtual void fillSplitRule(pair<AnonRule_sp, AnonRule_sp>& rulePair) const override;

			protected:

				/**
				 * Stores all universally quantified IDs in the left-hand side.
				 */
				unordered_set<IDType> leftUQVertices;

				/**
				 * Stores all images of universally quantified IDs in the right-hand side.
				 */
				unordered_set<IDType> rightUQVertices;

		};

		/**
		 * Alias of a shared pointer to a UQRuleIstance.
		 */
		typedef shared_ptr<UQRuleInstance> UQRuleInstance_sp;

		/**
		 * Alias of a shared pointer to a constant UQRuleInstance.
		 */
		typedef shared_ptr<UQRuleInstance const> UQRuleInstance_csp;

	}
}

#endif /* UQRULEINSTANCE_H_ */
