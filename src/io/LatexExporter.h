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

#ifndef LATEXEXPORTER_H_
#define LATEXEXPORTER_H_

#include "../graphs/GTS.h"
#include "GraphDrawer.h"
#include <fstream>

namespace uncover {
	namespace io {

		/**
		 * Provides a possibility to generate Latex files from GTS objects or graph sets. The generated Latex file
		 *  will be automatically be converted to a PDF or postscript file. This compilation can also be deactivated.
		 * @author Jan Stückrath
		 */
		class LatexExporter {

			public:

				/**
				 * This Enumeration Type specifies for which resulting file formats the Latex file will be generated
				 * and finally compiled.
				 */
				enum TargetType {
					/** Postscript file format. */
					PS,
					/** pdf file format. */
					PDF};

				/**
				 * Generates a new LatexExporter with default values.
				 */
				LatexExporter();

				/**
				 * Destroys this instance of a LatexExporter.
				 */
				virtual ~LatexExporter();

				/**
				 * Creates (and compiles if not deactivated) a Latex file representing the given GTS. Depending on the given
				 * TargetType, a subfolder is created to store all images of graphs and a file displaying all initial graphs
				 * and rules is generated.
				 * @param gts the GTS which should be layouted
				 * @param filename the name of the file in which the latex code will be written (including extension)
				 * @param ruleIDSubs if given, not the rule IDs are printed, but their mapping
				 * @param graphIDSubs if given, not the graph IDs are printed, but their mapping
				 * @param target the TargetType to be generated
				 */
				void writeToLatex(graphs::GTS const& gts,
						string filename,
						LatexExporter::TargetType target = TargetType::PDF,
						unordered_map<IDType, IDType> const* ruleIDSubs = nullptr,
						unordered_map<IDType, IDType> const* graphIDSubs = nullptr) const;

				/**
				 * Creates (and compiles if not deactivated) a Latex file containing all given graphs. Depending on the given
				 * TargetType, a subfolder is created to store all images of graphs which will subsequently be merged to a
				 * single file.
				 * @param graphs the graphs which should be layouted
				 * @param filename the name of the file in which the latex code will be written (including extension)
				 * @param target the TargetType to be generated
				 * @param nameSubs every graph with a mapping in this map will have its value (in this map) be displayed in
				 * 				addition to its name
				 */
				void writeToLatex(vector<graphs::Hypergraph_sp> const& graphs,
						string filename,
						LatexExporter::TargetType target = TargetType::PDF,
						unordered_map<IDType, IDType> const* nameSubs = nullptr) const;

				/**
				 * Sets a scaling factor for all images. Semantic of this factor can depend on chosen TargetType.
				 * @param scale the scaling factor
				 */
				void setScale(double scale);

				/**
				 * Returns the scaling factor currently used.
				 * @return the scaling factor currently used
				 */
				double getScale() const;

				/**
				 * Sets whether the Latex file should be compiled after its generation or not.
				 * @param compile true activates compilation, false deactivates
				 */
				void setCompilation(bool compile);

				/**
				 * Returns whether the Latex file will be compiled after its generation.
				 * @return true iff the Latex file will be compiled after its generation
				 */
				bool willCompile() const;

			private:

				/**
				 * Implementation of both writeToLatex(...) functions. If the given GTS is not null (has priority),
				 * it will be layouted. If a graph collection is given, it will be layouted and the ruleIDSubs
				 * parameter will be ignored. Either 'gts' or 'graphs' has to be defined!
				 * @see writeToLatex(graphs::GTS const&, string, LatexExporter::TargetType,unordered_map<IDType, IDType> const*,unordered_map<IDType, IDType> const*)
				 * @see writeToLatex(vector<graphs::Hypergraph_sp> const&, string, LatexExporter::TargetType, unordered_map<IDType, IDType> const*)
				 * @param gts the gts to be layouted; may be null if graphs parameter is not
				 * @param graphs the graph collection to be layouted; may be null if gts parameter is not
				 * @param filename the Latex filename to be created (including extension)
				 * @param target the target format
				 * @param ruleIDSubs if layouting a gts, the mapping of IDs of rules is printed instead of the normal IDs
				 * @param graphIDSubs if layouting a graph or gts, the mapping of IDs of graphs is printed instead of the normal IDs
				 */
				void writeToLatex(graphs::GTS const* gts,
						vector<graphs::Hypergraph_sp> const* graphs,
						string filename,
						LatexExporter::TargetType target,
						unordered_map<IDType, IDType> const* ruleIDSubs,
						unordered_map<IDType, IDType> const* graphIDSubs) const;

				/**
				 * Writes the header information of the Latex document (the preamble) to the given filestream. This
				 * contains all necessary definitions except any includes for packages needed for the image format
				 * and the definition of the \imgImport command.
				 * @param of the filestream to be used
				 */
				void writeLatexHeader(std::ofstream& of) const;

				/**
				 * Creates an image file for each graph in the collection and writes a Latex file including these graphs
				 * to the given stream. These graphs are stored in a folder named by the given filename appended with
				 * '_files' (which is created if non-existent).
				 * @param graphs the collection of graphs to be layouted
				 * @param of the file stream to which the Latex file will be written
				 * @param filename the name of the resulting Latex file (including extension)
				 * @param format the picture format used for generating the graph files
				 * @param nameSubs the value of IDs of graphs mapped by this parameter is exported in addition to the name
				 */
				void writeLatexAndGraphs(vector<graphs::Hypergraph_sp> const& graphs,
						std::ofstream& of,
						string filename,
						GraphDrawer::PictureFormat format,
						unordered_map<IDType, IDType> const* nameSubs) const;

				/**
				 * Writes the body (including \begin{document} and \end{document} to the given filestream. This body
				 * includes Latex code layouting the given GTS and referencing the files created by writeGraphs(...).
				 * @see LatexExporter::writeGraphs(graphs::GTS&, string, GraphDrawer::PictureFormat)
				 * @param gts the GTS to be layouted
				 * @param of the filestream to which the Latex body will be written
				 * @param filename the filename used to open the given filestream
				 * @param ext the extension of the used image format (not including the point)
				 * @param ruleIDSubs if given, the mapping of IDs of rules is printed instead of the normal IDs
				 * @param graphIDSubs if given, the mapping of IDs of graphs is printed instead of the normal IDs
				 */
				void writeLatexBody(graphs::GTS const& gts,
						std::ofstream& of,
						string filename,
						string ext,
						unordered_map<IDType, IDType> const* ruleIDSubs,
						unordered_map<IDType, IDType> const* graphIDSubs) const;

				/**
				 * Creates an image file for each graph stored in the given gts object. These graphs are stored in
				 * a folder named by the given filename appended with '_files' (which is created if non-existent).
				 * @param gts the GTS containing the graphs to be written
				 * @param filename the name of the Latex file which is being generated (including extension)
				 * @param format a PictureFormat value representing the generated image format
				 */
				void writeGraphs(graphs::GTS const& gts, string filename, GraphDrawer::PictureFormat format) const;

				/**
				 * Returns either the string or <noname> if the string was empty.
				 * @param the string to be checked
				 * @return the input string if non-empty, <noname> otherwise
				 */
				string getNameOrNoName(string name) const;

				/**
				 * Stores a scaling factor used when including the image files.
				 */
				double scale;

				/**
				 * Stores whether the generated Latex file will be compiled or not.
				 */
				bool compile;

		};

	} /* namespace io */
} /* namespace uncover */

#endif /* LATEXEXPORTER_H_ */
