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
#include <string.h>

#include "GTS.h"

using namespace std;

namespace uncover {
	namespace graphs {

		GTS::GTS(string name,
				unordered_map<IDType, Rule_sp> const& rules,
				unordered_map<IDType, UQRule_sp> const& uqRules,
				vector<Hypergraph_sp> const& initialGraphs)
		: name(name), initialGraphs(initialGraphs), stdRules(rules), uqRules(uqRules) {}

		GTS::~GTS() {}

		unordered_map<IDType, shared_ptr<Rule>> const& GTS::getStdRules() const {
			return this->stdRules;
		}

		bool GTS::addStdRule(Rule_sp rule) {
			if(rule != nullptr && stdRules.count(rule->getID()) == 0) {
				stdRules[rule->getID()] = rule;
				return true;
			} else {
				return false;
			}
		}

		size_t GTS::getStdRuleCount() const {
			return this->stdRules.size();
		}

		vector<shared_ptr<Hypergraph>> const& GTS::getInitialGraphs() const {
			return initialGraphs;
		}

		size_t GTS::getInitialCount() const {
			return initialGraphs.size();
		}

		string GTS::getName() const {
			return name;
		}

		unordered_map<IDType, UQRule_sp> const& GTS::getUQRules() const {
			return this->uqRules;
		}

		bool GTS::addUQRule(UQRule_sp rule) {
			if(rule != nullptr && uqRules.count(rule->getID()) == 0) {
				uqRules[rule->getID()] = rule;
				return true;
			} else {
				return false;
			}
		}

		size_t GTS::getUQRuleCount() const {
			return uqRules.size();
		}

		bool GTS::addInitialGraph(Hypergraph_sp graph) {
			if(graph) {
				for(vector<Hypergraph_sp>::const_iterator it = initialGraphs.cbegin();
						it != initialGraphs.cend(); ++it) {
					if((*it)->getID() == graph->getID()) return false;
				}
				initialGraphs.push_back(graph);
				return true;
			} else {
				return false;
			}
		}

		void GTS::streamTo(std::ostream& ost) const {
			ost << "GTS(name=" << name << ", ";
			ost << initialGraphs.size() << " initial graphs, ";
			ost << stdRules.size() << " normal rules, ";
			ost << uqRules.size() << " UQ rules)";
		}

	}
}

