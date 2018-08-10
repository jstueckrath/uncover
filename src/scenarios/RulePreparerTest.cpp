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

#include "RulePreparerTest.h"
#include "scenario_ids.h"
#include "../io/GTXLReader.h"
#include "../io/GTXLWriter.h"
#include "../subgraphs/SubgraphRulePreparer.h"
#include "../minors/MinorRulePreparer.h"
#include "../logging/message_logging.h"
#include "../io/io_helpers.h"
#include "../io/XMLioException.h"
#include "../basic_types/RunException.h"
#include "../analysis/PreparedGTS.h"
#include <sstream>

using namespace uncover::io;
using namespace uncover::graphs;
using namespace uncover::subgraphs;
using namespace uncover::minors;
using namespace uncover::logging;
using namespace uncover::basic_types;
using namespace uncover::analysis;
using namespace std;

namespace uncover {
	namespace scenarios {

		RulePreparerTest::RulePreparerTest() : Scenario(SCN_TEST_RULE_PREPARER_NAME,
				"This scenario tests the rule preparation functionality of this framework. The prepared rules are "
				"exported as a GTS with and without optimization enabled.",
				{SCN_TEST_RULE_PREPARER_SHORT_1}),
				filename(""),
				gts(nullptr),
				rulePreparer(nullptr) {
			this->addParameter("rules", "A gtxl file with rules which will be prepared. (including extension)", true);
			this->addSynonymFor("rules", "r");
			this->addParameter("order", "Specify for which order the rules should be prepared. Possibilities are: "
					"subgraph (default), minor.", false);
			this->addSynonymFor("order","o");
		}

		RulePreparerTest::~RulePreparerTest() {}

		void RulePreparerTest::run() {

			unique_ptr<GTXLWriter> writer = GTXLWriter::newUniqueWriter();

			// test without optimizations
			PreparedGTS withoutOpt(*gts, rulePreparer, false);
			writer->writeGTXL(withoutOpt, removeFileExtension(filename) + "_prep_without_opt.xml");
			logger.newUserInfo() << "Preparing without optimization resulted in " << withoutOpt.getStdRuleCount() << " rules." << endLogMessage;

			// test with optimizations
			PreparedGTS withOpt(*gts, rulePreparer, true);
			writer->writeGTXL(withOpt, removeFileExtension(filename) + "_prep_with_opt.xml");
			logger.newUserInfo() << "Preparing with optimization resulted in " << withOpt.getStdRuleCount() << " rules." << endLogMessage;

		}

		void RulePreparerTest::initialize(unordered_map<string, string>& parameters) {

			this->filename = parameters["rules"];
			unique_ptr<GTXLReader> reader = GTXLReader::newUniqueReader();
			gts = reader->readGTXL(filename);

			if(parameters.count("order")) {
				if(!parameters["order"].compare("subgraph")) {
					rulePreparer = make_shared<SubgraphRulePreparer>();
				} else if(!parameters["order"].compare("minor")) {
					rulePreparer = make_shared<MinorRulePreparer>();
				} else {
					logger.newWarning() << "The order '" << parameters["order"] << "' is unknown. Using 'subgraph' as order!" << endLogMessage;
					rulePreparer = make_shared<SubgraphRulePreparer>();
				}
			} else {
				rulePreparer = make_shared<SubgraphRulePreparer>();
			}

		}

	} /* namespace graphs */
} /* namespace uncover */
