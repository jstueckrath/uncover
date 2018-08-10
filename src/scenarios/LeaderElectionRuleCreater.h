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

#ifndef LEADERELECTIONRULECREATER_H_
#define LEADERELECTIONRULECREATER_H_

#include "../basic_types/Scenario.h"

namespace uncover {
	namespace scenarios {

		/**
		 * When called this Scenario generates the GTS for the leader election example. For this the number of processes
		 * must be set to some natural number and the filename must be given, to which the GTS will be written (as a
		 * GTXL file).
		 * @author Jan Stückrath
		 */
		class LeaderElectionRuleCreater : public basic_types::Scenario {

			public:

				/**
				 * Creates a new instance of this scenario. The scenario still has to be initialized before it can be executed.
				 */
				LeaderElectionRuleCreater();

				/**
				 * Destroys this instance of the  LeaderElectionRuleCreater.
				 */
				virtual ~LeaderElectionRuleCreater();

				/**
				 * Creates a GTS for the leader election example with the given number of processes and writes it to the
				 * filesystem as a GTXL file.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * Stores the number of processes for which GTS will be created.
				 */
				unsigned int processCount;

				/**
				 * Stores the filename to which the GTS will be written.
				 */
				string name;

		};

	} /* namespace scenarios */
} /* namespace uncover */

#endif /* LEADERELECTIONRULECREATER_H_ */
