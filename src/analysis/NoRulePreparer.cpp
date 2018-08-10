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

#include "NoRulePreparer.h"

using namespace uncover::graphs;
using namespace uncover::basic_types;

namespace uncover {
	namespace analysis {

		NoRulePreparer::NoRulePreparer() {}

		NoRulePreparer::~NoRulePreparer() {}

		void NoRulePreparer::prepareRule(Rule const& rule, CollectionCallback<Rule_sp>& callback) const {
			Rule_sp newRule = make_shared<Rule>(rule);
			callback.insert(newRule);
		}

		void NoRulePreparer::prepareRule(UQRuleInstance const& rule, CollectionCallback<UQRuleInstance_sp>& callback) const {
			UQRuleInstance_sp newRule = make_shared<UQRuleInstance>(rule);
			callback.insert(newRule);
		}

		bool NoRulePreparer::isNoOpRule(AnonRule const& rule) const {
			return false;
		}

	} /* namespace analysis */
} /* namespace uncover */
