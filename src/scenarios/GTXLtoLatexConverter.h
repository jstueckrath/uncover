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

#ifndef GTXLTOLATEXCONVERTER_H_
#define GTXLTOLATEXCONVERTER_H_

#include "../basic_types/Scenario.h"
#include "../io/LatexExporter.h"

namespace uncover {
	namespace scenarios {

		/**
		 * Use this Scenario to create a graphical representation of the given GTXL file. The GTS in the GTXL file is read,
		 * converted into a '.tex' file and then compiled. The compilation can produce either postscript or a pdf file.
		 * The compilation can also be suppressed.
		 * @author Jan Stückrath
		 */
		class GTXLtoLatexConverter: public basic_types::Scenario {

			public:

				/**
				 * Creates a new instance of this Scenario.
				 */
				GTXLtoLatexConverter();

				/**
				 * Destroys the this instance of this Scenario.
				 */
				virtual ~GTXLtoLatexConverter();

				/**
				 * Executes the scenario and converts the given GTXL file into a pdf, drawing all rules and initial graphs using
				 * Graphviz.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

			private:

				/**
				 * Stores the GTS read from the given source file.
				 */
				graphs::GTS_sp gts;

				/**
				 * Stores the filename of the Latex file used as target of the conversion.
				 */
				string targetName;

				/**
				 * Stores in which type the target Latex file will be compiled.
				 */
				io::LatexExporter::TargetType type;

				/**
				 * Stores whether the Latex file should be compiled or not.
				 */
				bool compile;

				/**
				 * Stores any ID substitutions for rules, if specified in the GTS file.
				 */
				unordered_map<IDType,IDType>* ruleIDSubs;

				/**
				 * Stores any ID substitutions for graphs, if specified in the GTS file.
				 */
				unordered_map<IDType,IDType>* graphIDSubs;

		};

	} /* namespace scenarios */
} /* namespace uncover */

#endif /* GTXLTOLATEXCONVERTER_H_ */
