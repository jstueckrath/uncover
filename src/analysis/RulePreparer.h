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

#ifndef RULEPREPARATION_H_
#define RULEPREPARATION_H_

#include "../graphs/UQRuleInstance.h"
#include "../graphs/Rule.h"
#include "../basic_types/CollectionCallback.h"

namespace uncover {
	namespace analysis {

		/**
		 * This class takes a rule and prepares it such that no special case handling is necessary while
		 * performing the analysis. This is done by concatenating all possible order morphisms to the right
		 * of this rule, effectively generating a set of new rules. The prepared rules can be applied
		 * backwards straightforwardly for the backward search to be sound and complete.
		 * @author Jan Stückrath
		 */
		class RulePreparer {

			public:

				/**
				 * Destroys the current RulePreparer instance.
				 */
				virtual ~RulePreparer() {};

				/**
				 * Takes a single rule and creates the prepared set of rules for this rule. Each newly generated
				 * rule is added via the given callback function.
				 * @param rule the rule which should be prepared
				 * @param callback the callback defining where the newly generated rules will be added
				 */
				virtual void prepareRule(graphs::Rule const& rule,
						basic_types::CollectionCallback<graphs::Rule_sp>& callback) const = 0;

				/**
				 * Takes a single instantiation of a universally quantified rule and creates the prepared set of
				 * rules for this instantiation. Each newly generated rule is added via the given callback
				 * function. Takes the special properties of universally quantified rules into account.
				 * @param rule an instantiation of a universally quantified rule which should be prepared
				 * @param callback the callback defining where the newly generated rules will be added
				 */
				virtual void prepareRule(graphs::UQRuleInstance const& rule,
						basic_types::CollectionCallback<graphs::UQRuleInstance_sp>& callback) const = 0;

				/**
				 * Checks if the given rule will only produce graphs already represented when the backward analysis
				 * is applied, i.e. its application is not necessary in the first place. Does not (necessarily)
				 * check if the rule is a valid morphism! This function is only used for optimization. A special
				 * treatment of instantiations of universally quantified rules in not necessary for this check.
				 * @param rule the rule to be checked
				 * @return true iff the given rule can be ignored by the backward analysis without loss of correctness
				 */
				virtual bool isNoOpRule(graphs::AnonRule const& rule) const = 0;

				/**
				 * Sets whether the rule preparation will be optimized, i.e. the number of produced rules will be minimized.
				 * @param opt true, iff the rule preparation should be optimized
				 */
				void setOptimization(bool opt) { optimize = true; }

				/**
				 * Returns whether the rule preparation will be optimized, i.e. the number if produced ruless will be
				 * minimized.
				 * @return true, iff the rule preparation will be optimized
				 */
				bool willOptimize() const { return optimize; }

			protected:

				/**
				 * A RulePreparer can only be instantiated by a derived class.
				 */
				RulePreparer() : optimize(true) {}

				/**
				 * Stores whether the rule preparation will be optimized.
				 */
				bool optimize;

		};

		/**
		 * Alias for a shared pointer of a RulePreparer.
		 */
		typedef shared_ptr<RulePreparer> RulePreparer_sp;

		/**
		 * Alias for a shared pointer of a constant RulePreparer.
		 */
		typedef shared_ptr<RulePreparer const> RulePreparer_csp;

	} /* namespace basic_types */
} /* namespace uncover */

#endif /* RULEPREPARATION_H_ */
