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

#include "rule_preparation_helper.h"

using namespace uncover::graphs;

namespace uncover {
	namespace analysis {

		Rule_sp createRuleFrom(Rule const& oldRule,
				string name,
				AnonHypergraph_sp leftGraph,
				AnonHypergraph_sp rightGraph,
				Mapping const& mapping) {
			return make_shared<Rule>(name, leftGraph, rightGraph, mapping);
		}

		UQRuleInstance_sp createRuleFrom(UQRuleInstance const& oldRule,
				string name,
				AnonHypergraph_sp leftGraph,
				AnonHypergraph_sp rightGraph,
				Mapping const& mapping) {

			unordered_set<IDType> rightUQVertices;
			for(auto iter = oldRule.getRightUQVertices().begin(); iter != oldRule.getRightUQVertices().end(); ++iter) {

				if(rightGraph->hasVertex(*iter)) {
					rightUQVertices.insert(*iter);
				}

			}

			return make_shared<UQRuleInstance>(name, leftGraph, rightGraph, mapping, oldRule.getLeftUQVertices(), rightUQVertices);
		}

	}
}


