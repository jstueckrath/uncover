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

#ifndef RULE_PREPARATION_HELPER_H_
#define RULE_PREPARATION_HELPER_H_

#include "../basic_types/globals.h"
#include "../graphs/UQRuleInstance.h"
#include "../graphs/Rule.h"

namespace uncover {
	namespace analysis {

		/**
		 * Creates a new shared Rule using the given name, left side, right side and mapping. The first parameter
		 * is ignored.
		 * @param oldRule the Rule from which the other parameters where generated (ignored)
		 * @param name the name of the new Rule
		 * @param leftGraph the left gragh of the new Rule
		 * @param rightGraph the right graph of the new Rule
		 * @param mapping the Mapping of the new Rule
		 * @return a new shared Rule created from the input
		 */
		graphs::Rule_sp createRuleFrom(graphs::Rule const& oldRule,
				string name,
				graphs::AnonHypergraph_sp leftGraph,
				graphs::AnonHypergraph_sp rightGraph,
				graphs::Mapping const& mapping);

		/**
		 * Creates a new shared UQRuleInstance using the given name, left side, right side and mapping. The
		 * universally quantified nodes are extracted from the given old UQRuleInstance.
		 * @param oldRule the UQRuleInstance from which the other parameters where generated (used for UQ nodes)
		 * @param name the name of the new UQRuleInstance
		 * @param leftGraph the left gragh of the new UQRuleInstance
		 * @param rightGraph the right graph of the new UQRuleInstance
		 * @param mapping the Mapping of the new UQRuleInstance
		 * @return a new shared UQRuleInstance created from the input
		 */
		graphs::UQRuleInstance_sp createRuleFrom(graphs::UQRuleInstance const& oldRule,
				string name,
				graphs::AnonHypergraph_sp leftGraph,
				graphs::AnonHypergraph_sp rightGraph,
				graphs::Mapping const& mapping);

	}
}



#endif /* RULE_PREPARATION_HELPER_H_ */
