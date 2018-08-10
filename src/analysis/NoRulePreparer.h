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

#ifndef NORULEPREPARER_H_
#define NORULEPREPARER_H_

#include "RulePreparer.h"

namespace uncover {
	namespace analysis {

		/**
		 * This RulePreparer returns all rules unprepared. Use this RulePreparer only for testing purposes.
		 * @author Jan Stückrath
		 */
		class NoRulePreparer : public RulePreparer {

			public:

				/**
				 * Generates a new NoRulePreparer.
				 */
				NoRulePreparer();

				/**
				 * Destroys this NoRulePreparer.
				 */
				virtual ~NoRulePreparer();

				/**
				 * Generates a copy of the given Rule and sends it to the callback object.
				 * @param rule the Rule to be prepared
				 * @param callback the callback to which the copy of the given rule will be sent
				 */
				virtual void prepareRule(graphs::Rule const& rule,
						basic_types::CollectionCallback<graphs::Rule_sp>& callback) const override;

				/**
				 * Generates a copy of the given UQRuleInstance and sends it to the callback object.
				 * @param rule the UQRuleInstance to be prepared
				 * @param callback the callback to which the copy of the given rule will be sent
				 */
				virtual void prepareRule(graphs::UQRuleInstance const& rule,
						basic_types::CollectionCallback<graphs::UQRuleInstance_sp>& callback) const override;

				/**
				 * Always returns false.
				 * @param rule this parameter is ignored
				 * @return false
				 */
				virtual bool isNoOpRule(graphs::AnonRule const& rule) const override;

		};

		/**
		 * Alias for a shared pointer to a NoRulePreparer object.
		 */
		typedef shared_ptr<NoRulePreparer> NoRulePreparer_sp;

		/**
		 * Alias for a shared pointer to a constant NoRulePreparer object.
		 */
		typedef shared_ptr<NoRulePreparer const> NoRulePreparer_csp;

	} /* namespace analysis */
} /* namespace uncover */

#endif /* NORULEPREPARER_H_ */
