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

#ifndef XMLTESTSCENARIO_H_
#define XMLTESTSCENARIO_H_

#include "../basic_types/Scenario.h"

namespace uncover {
	namespace scenarios {

		/**
		 * This Scenario tests the GTXLReader and GTXLWriter classes on correctness. For that multiple GXL and GTXL
		 * files will be read and written.
		 * @author Jan Stückrath
		 * @see io::GTXLReader
		 * @see io::GTXLWriter
		 */
		class XMLTestScenario: public basic_types::Scenario {

			public:

				/**
				 * Generates a new XMLTestScenario. The generated object still has to be initialized.
				 */
				XMLTestScenario();

				/**
				 * Destroys the XMLTestScenario.
				 */
				virtual ~XMLTestScenario();

				/**
				 * Imports and directly exports GXL and GTXL files as specified by the initialization parameters.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * A list of all GXL filenames to be read.
				 */
				vector<string> gxlFilenames;

				/**
				 * A list of all GTXL filenames to be read.
				 */
				vector<string> gtxlFilenames;

		};

	} /* namespace scenarios */
} /* namespace uncover */

#endif
