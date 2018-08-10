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

#ifndef RULEPREPARERTEST_H_
#define RULEPREPARERTEST_H_

#include "../basic_types/Scenario.h"
#include "../analysis/RulePreparer.h"
#include "../graphs/GTS.h"

namespace uncover {
	namespace scenarios {

		/**
		 * This scenario tests the rule preparation functionality. The prepared rules are printed on the console.
		 */
		class RulePreparerTest : public basic_types::Scenario{

			public:

				/**
				 * Generates a new RulePreparerTest object.
				 */
				RulePreparerTest();

				/**
				 * Destroys this RulePreparerTest object.
				 */
				virtual ~RulePreparerTest();

				/**
				 * Prepares the rule set previously given with and without optimization and stores the results as XMl files.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * Stores the filename and path of the GTS loaded.
				 */
				string filename;

				/**
				 * Stores the loaded GTS.
				 */
				graphs::GTS_sp gts;

				/**
				 * Stores the RulePreparer used for preparation.
				 */
				analysis::RulePreparer_sp rulePreparer;

		};

	} /* namespace graphs */
} /* namespace uncover */

#endif
