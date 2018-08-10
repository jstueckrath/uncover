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

#include "ScenarioLoader.h"
#include "scenario_ids.h"
#include "GXLtoPicConverter.h"
#include "XMLTestScenario.h"
#include "BackwardAnalysisScenario.h"
#include "RulePreparerTest.h"
#include "GTXLtoLatexConverter.h"
#include "BackwardStepTest.h"
#include "MatcherScenario.h"
#include "LeaderElectionRuleCreater.h"
#include "LEQCheckScenario.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>

using namespace uncover::basic_types;
using namespace std;

namespace uncover {
	namespace scenarios {

		ScenarioLoader::ScenarioLoader() : scnNameToIDMapping(), firstNames() {
			this->initNameToIDMapping();
		}

		ScenarioLoader::~ScenarioLoader() {}

		Scenario_up ScenarioLoader::getScenario(string ident) const {

			if(scnNameToIDMapping.count(ident)) {
				return getScenarioByID(scnNameToIDMapping.at(ident));
			} else {
				try {
					unsigned int id = boost::lexical_cast<unsigned int>(ident);
					return getScenarioByID(id);
				}	catch(boost::bad_lexical_cast& ex) {
					return nullptr;
				}
			}

		}

		Scenario_up ScenarioLoader::getScenarioByName(string name) const {
			if(scnNameToIDMapping.count(name) == 0) {
				return nullptr;
			}
			return getScenarioByID(scnNameToIDMapping.at(name));
		}

		Scenario_up ScenarioLoader::getScenarioByID(unsigned int id) const {
			switch(id) {
				case SCN_BACKWARD_ANALYSIS_ID:
					return unique_ptr<Scenario>(new BackwardAnalysisScenario());
				case SCN_GTXL_LATEX_CONVERTER_ID:
					return unique_ptr<Scenario>(new GTXLtoLatexConverter());
				case SCN_GXL_PIC_CONVERTER_ID:
					return unique_ptr<Scenario>(new GXLtoPicConverter());
				case SCN_LEQ_CHECK_ID:
					return unique_ptr<Scenario>(new LEQCheckScenario());
				case SCN_LE_RULE_CREATOR_ID:
					return unique_ptr<Scenario>(new LeaderElectionRuleCreater());
				case SCN_TEST_XML_ID:
					return unique_ptr<Scenario>(new XMLTestScenario());
				case SCN_TEST_RULE_PREPARER_ID:
					return unique_ptr<Scenario>(new RulePreparerTest());
				case SCN_TEST_BACKWARD_STEP_ID:
					return unique_ptr<Scenario>(new BackwardStepTest());
				case SCN_TEST_MATCHER_ID:
					return unique_ptr<Scenario>(new MatcherScenario());
				default:
					return nullptr;
			}
		}

		void ScenarioLoader::initNameToIDMapping () {

			scnNameToIDMapping[SCN_BACKWARD_ANALYSIS_NAME] = SCN_BACKWARD_ANALYSIS_ID;
			scnNameToIDMapping[SCN_BACKWARD_ANALYSIS_SHORT_1] = SCN_BACKWARD_ANALYSIS_ID;
			firstNames.insert(SCN_BACKWARD_ANALYSIS_NAME);

			scnNameToIDMapping[SCN_GXL_PIC_CONVERTER_NAME] = SCN_GXL_PIC_CONVERTER_ID;
			scnNameToIDMapping[SCN_GXL_PIC_CONVERTER_SHORT_1] = SCN_GXL_PIC_CONVERTER_ID;
			firstNames.insert(SCN_GXL_PIC_CONVERTER_NAME);

			scnNameToIDMapping[SCN_GTXL_LATEX_CONVERTER_NAME] = SCN_GTXL_LATEX_CONVERTER_ID;
			scnNameToIDMapping[SCN_GTXL_LATEX_CONVERTER_SHORT_1] = SCN_GTXL_LATEX_CONVERTER_ID;
			firstNames.insert(SCN_GTXL_LATEX_CONVERTER_NAME);

			scnNameToIDMapping[SCN_LEQ_CHECK_NAME] = SCN_LEQ_CHECK_ID;
			scnNameToIDMapping[SCN_LEQ_CHECK_SHORT_1] = SCN_LEQ_CHECK_ID;
			firstNames.insert(SCN_LEQ_CHECK_NAME);

			scnNameToIDMapping[SCN_LE_RULE_CREATOR_NAME] = SCN_LE_RULE_CREATOR_ID;
			scnNameToIDMapping[SCN_LE_RULE_CREATOR_SHORT_1] = SCN_LE_RULE_CREATOR_ID;
			firstNames.insert(SCN_LE_RULE_CREATOR_NAME);

			scnNameToIDMapping[SCN_TEST_XML_NAME] = SCN_TEST_XML_ID;
			firstNames.insert(SCN_TEST_XML_NAME);

			scnNameToIDMapping[SCN_TEST_RULE_PREPARER_NAME] = SCN_TEST_RULE_PREPARER_ID;
			scnNameToIDMapping[SCN_TEST_RULE_PREPARER_SHORT_1] = SCN_TEST_RULE_PREPARER_ID;
			firstNames.insert(SCN_TEST_RULE_PREPARER_NAME);

			scnNameToIDMapping[SCN_TEST_BACKWARD_STEP_NAME] = SCN_TEST_BACKWARD_STEP_ID;
			scnNameToIDMapping[SCN_TEST_BACKWARD_STEP_SHORT_1] = SCN_TEST_BACKWARD_STEP_ID;
			firstNames.insert(SCN_TEST_BACKWARD_STEP_NAME);

			scnNameToIDMapping[SCN_TEST_MATCHER_NAME] = SCN_TEST_MATCHER_ID;
			firstNames.insert(SCN_TEST_MATCHER_NAME);

		}

		void ScenarioLoader::getScenarioList(ostream& ost, string separator) const {
			string desc1 = "ID ";
			string desc2 = "; Names: ";
			string desc3 = ", ";

			ost << "List of available scenarios:\n----------------------------\n";

			vector<unsigned int> allIDs;
			for(auto& p : scnNameToIDMapping) {
				allIDs.push_back(p.second);
			}
			sort(allIDs.begin(),allIDs.end());

			bool printsep = false;
			unsigned int lastID = 0;
			for(auto& id : allIDs) {
				// ignore duplicates
				if(id == lastID) {
					continue;
				} else {

					if(printsep) {
						ost << separator;
					} else {
						printsep = true;
					}

					lastID = id;
					string firstName = "";
					vector<string> otherNames;
					for(auto& p : scnNameToIDMapping) {
						if(p.second == id) {
							if(firstNames.count(p.first)) {
								firstName = p.first;
							} else {
								otherNames.push_back(p.first);
							}
						}
					}

					ost << desc1 << id << desc2;
					bool skipdesc3 = false;
					if(firstName.compare("")) {
						ost << firstName;
					} else {
						skipdesc3 = true;
					}

					for(auto& name : otherNames) {
						if(skipdesc3) {
							skipdesc3 = false;
						} else {
							ost << desc3;
						}
						ost << name;
					}

				}
			}

		}

	}
} /* namespace loader */
