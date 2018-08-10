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

#ifndef SUBGRAPHRULEPREPARER_H_
#define SUBGRAPHRULEPREPARER_H_

#include "../analysis/RulePreparer.h"

namespace uncover {
	namespace subgraphs {

		/**
		 * This class prepares given rules to be used together with the subgraph order in the backward search.
		 * For each given rule L->R the rule L->R->S is added instead for every subgraph morphism R->S.
		 * @author Jan Stückrath
		 * @see SubgraphOrder
		 */
		class SubgraphRulePreparer: public analysis::RulePreparer {

			public:

				/**
				 * Creates a new instance of this rule preparer.
				 */
				SubgraphRulePreparer();

				/**
				 * Destroys the current instance of this preparer.
				 */
				virtual ~SubgraphRulePreparer();

				virtual void prepareRule(graphs::Rule const& rule,
						basic_types::CollectionCallback<graphs::Rule_sp>& callback) const override;

				virtual void prepareRule(graphs::UQRuleInstance const& rule,
						basic_types::CollectionCallback<graphs::UQRuleInstance_sp>& callback) const override;

				virtual bool isNoOpRule(graphs::AnonRule const& rule) const override;

		};

		/**
		 * Alias for a shared pointer of a SubgraphRulePreparer.
		 */
		typedef shared_ptr<SubgraphRulePreparer> SubgraphRulePreparer_sp;

		/**
		 * Alias for a shared pointer of a constant SubgraphRulePreparer.
		 */
		typedef shared_ptr<SubgraphRulePreparer const> SubgraphRulePreparer_csp;

	} /* namespace subgraphs */
} /* namespace uncover */

#endif /* SUBGRAPHRULEPREPARER_H_ */
