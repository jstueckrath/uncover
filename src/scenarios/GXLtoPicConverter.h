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

#ifndef HG_CONVERTOR_H
#define HG_CONVERTOR_H

#include "../basic_types/Scenario.h"
#include "../io/GraphDrawer.h"

namespace uncover {
	namespace scenarios {

		/**
		 * This Scenario takes a GXL file and converts the encoded Hypergraphs first to a 'dot'
		 * file and then to the set file format using neato. Graphviz is necessary to run this scenario.
		 * @see <a href="http://www.graphviz.org/">Graphviz homepage</a>
		 * @author Jan Stückrath
		 */
		class GXLtoPicConverter: public basic_types::Scenario
		{

			public:

				/**
				 * Generates a new instance of GXLConverter.
				 */
				GXLtoPicConverter();

				/**
				 * Destroys this instance of GXLConverter.
				 */
				~GXLtoPicConverter();

				/**
				 * Runs this Scenario, first generating a 'dot' file for each input graph and then
				 * converting each to the set file format.
				 */
				virtual void run() override;

			protected:

				virtual void initialize(unordered_map<string,string>& parameters) override;

				/**
				 * Stores the filename of the given GXL file (without extension).
				 */
				string filename;

				/**
				 * Stores the format in which the loaded graphs will be ultimately converted.
				 */
				io::GraphDrawer::PictureFormat exportFormat;

				/**
				 * If set to true, only one file containing all graphs will be generated, instead of one file per graph.
				 */
				bool compact;

		};

	}
}

#endif
