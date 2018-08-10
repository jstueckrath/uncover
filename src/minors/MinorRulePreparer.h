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

#ifndef MINORRULEPREPARER_H_
#define MINORRULEPREPARER_H_

#include "../analysis/RulePreparer.h"

namespace uncover {
	namespace minors {

		/**
		 * This class prepares given rules to be used together with the minor order in the backward search.
		 * Given rules L->R are thereby taken and the rule L->R->M is added instead for every minor morphism R->M.
		 * @author Jan Stückrath
		 * @see MinorOrder
		 */
		class MinorRulePreparer : public analysis::RulePreparer {

			public:

				/**
				 * Generates a new instance of a MinorRulePreparer. No further instantiation is needed.
				 */
				MinorRulePreparer();

				/**
				 * Destroys the current instance of this class.
				 */
				virtual ~MinorRulePreparer();

				virtual void prepareRule(graphs::Rule const& rule,
						basic_types::CollectionCallback<graphs::Rule_sp>& callback) const override;

				virtual void prepareRule(graphs::UQRuleInstance const& rule,
						basic_types::CollectionCallback<graphs::UQRuleInstance_sp>& callback) const override;

				virtual bool isNoOpRule(graphs::AnonRule const& rule) const override;

		};

		/**
		 * Alias for a shared pointer of a MinorRulePreparer.
		 */
		typedef shared_ptr<MinorRulePreparer> MinorRulePreparer_sp;

		/**
		 * Alias for a shared pointer of a constant MinorRulePreparer.
		 */
		typedef shared_ptr<MinorRulePreparer const> MinorRulePreparer_csp;

	} /* namespace preparers */
} /* namespace uncover */

#endif /* MINORRULEPREPARER_H_ */
