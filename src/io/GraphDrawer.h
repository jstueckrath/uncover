/***************************************************************************
 *   Copyright (C) 2006 by Vitali Kozioura                                 *
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

#ifndef GRAPHDRAWER_H
#define GRAPHDRAWER_H

#include "../graphs/AnonHypergraph.h"

namespace uncover {
	namespace io {

		/**
		 * This class provides static methods for Converting GXL files to the 'dot' format. This format can then be
		 * converted to several picture formats using the 'Graphviz' tool.
		 * @see <a href="http://www.graphviz.org/">Graphviz homepage</a>
		 * @author Jan Stückrath
		 * @author Vitali Kozioura
		 */
		class GraphDrawer
		{

			public:

				/**
				 * This enumeration specifies all supported picture formats.
				 */
				enum PictureFormat {
					/** PictureFormat for postscript files. */
					PicPS,
					/** PictureFormat for png graphics files. */
					PicPNG,
					/** PictureFormat for pdf graphics files. */
					PicPDF};

				/**
				 * A collection of layouts used to distinguish different vertices and edges visually. BorderLayouts are
				 * used to visually distinguish multiple quantification in UQRules.
				 */
				enum BorderStyle {
					/** Blue color style. */
					BorderBlue,
					/** Green color style. */
					BorderGreen,
					/** Red color style. */
					BorderRed,
					/** Yellow color style. */
					BorderYellow};

				/**
				 * Destroys an instance of GraphDrawer.
				 */
				virtual ~GraphDrawer();

				/**
				 * Takes the given Hypergraph and converts it to the 'dot' format. Each element of the graph can be
				 * labeled with some given string and drawn in a special border style. The 'dot' file is stored using
				 * the given filename.
				 * @param graph the Hypergraph to be converted
				 * @param filename the filename used to store the 'dot' file (including extension)
				 * @param labels a map with up to one label for each drawn element
				 * @param borderStyles a map assigning a BorderStyle to elements of the graph
				 */
				static void writeGraphToDotFile(
						graphs::AnonHypergraph const& graph,
						string filename,
						unordered_map<IDType,string> const& labels = (unordered_map<IDType,string>()),
						unordered_map<IDType,GraphDrawer::BorderStyle> const& borderStyles = (unordered_map<IDType, GraphDrawer::BorderStyle>()));

				/**
				 * Takes the given 'dot' file and converts it to a picture file using neato. Which picture file type
				 * should be used, must be specified. An optional parameter can be used to automatically append an
				 * appropriate file extension.
				 * @see GraphDrawer::PictureFormat
				 * @param dotFile filename of the 'dot' file to be converted (including extension)
				 * @param outFile filename of the output file (including extension)
				 * @param format the picture format that will be generated
				 * @param genExt if this parameter is true, the standard extension for the given format will be appended
				 * 							 to the given output filename (default is false)
				 */
				static void convertDotTo(string dotFile, string outFile, PictureFormat format, bool genExt = false);

				/**
				 * Returns the filename extension normally used for the given picture format.
				 * @param format the picture format for which the extension should be returned
				 * @return the filename extension normally used for the given picture format
				 */
				static string getStandardExtension(PictureFormat format);

			private:

				/**
				 * GraphDrawer is not instantiable (all methods are static).
				 */
				GraphDrawer();

				/**
				 * Generates a string representing the given BorderStyle in the 'dot' format.
				 * @param style the BorderStyle to be converted
				 * @return the 'dot' parameter drawing a vertex or edge in the given BorderStyle
				 */
				static string BorderStyleToString(GraphDrawer::BorderStyle style);

		};

	};
};

#endif
