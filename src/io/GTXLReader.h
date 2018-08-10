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

#ifndef GTXLREADER_H_
#define GTXLREADER_H_

#include "../graphs/GTS.h"

namespace uncover {
	namespace io {

		/**
		 * This class imports GXL or GTXL XML files and converts them to Hypergraph and GTS objects respectively.
		 * @author Jan Stückrath
		 * @see graphs::Hypergraph
		 * @see graphs::GTS
		 */
		class GTXLReader {

			public:

				/**
				 * Destroys the GTXLReader object.
				 */
				virtual ~GTXLReader();

				/**
				 * This method takes a path to a GXL XML-file and reads all hypergraphs stored in it.
				 * @param filename a complete path to a GXL file
				 * @param oldIDs if given, this map will be filled with pairs of graph IDs mapped to
				 * 				(old) IDs read from the input files
				 * @return a pointer to a vector of the graphs read
				 * @throws io::XMLioException if the specified file is not found or erroneous
				 */
				virtual shared_ptr<vector<graphs::Hypergraph_sp>> readGXL(string filename,
						unordered_map<IDType,IDType>* oldIDs = nullptr) const = 0;

				/**
				 * This method takes a path to a GTXL XML-file and reads the stored GTS.
				 * @param filename a complete path to a GTXL file
				 * @param oldRuleIDs if given, this map will be filled with pairs of rule IDs mapped to
				 * 				(old) IDs read from the input files
				 * @param oldGraphIDs if given, this map will be filled with pairs of graph IDs mapped to
				 * 				(old) IDs read from the input files
				 * @return a pointer to the read GTS
				 * @throws io::XMLioException if the specified file is not found or erroneous
				 */
				virtual graphs::GTS_sp readGTXL(string filename,
						unordered_map<IDType,IDType>* oldRuleIDs = nullptr,
						unordered_map<IDType,IDType>* oldGraphIDs = nullptr) const = 0;

				/**
				 * Use this method to generate a new GTXLReader object. No initialization is needed before use.
				 * @warning The caller has to properly delete the GTXLReader after use to avoid memory leaks.
				 * @return a pointer to a new GTXLReader object
				 */
				static GTXLReader* newReader();

				/**
				 * Use this method to generate a new GTXLReader object. No initialization is needed before use.
				 * @return a pointer to a new GTXLReader object
				 */
				static unique_ptr<GTXLReader> newUniqueReader();

				/**
				 * Returns true, iff warnings will be logged when they occur.
				 * @return true, iff warnings will be shown
				 */
				bool areWarningsShown();

				/**
				 * Sets whether warning will be logged when they occur.
				 * @param show set to true, if warnings should be logged
				 */
				void setWarningsShown(bool show);

			protected:

				/**
				 * The default constructor is hidden. Use the static methods to generate a new GTXLReader object.
				 * @see GTXLReader#newReader()
				 * @see GTXLReader#newUniqueReader()
				 */
				GTXLReader();

				/**
				 * Specifies whether warnings for ignored XML nodes and attributes should be displayed or not.
				 */
				bool showWarnings;

		};

		/**
		 * Alias for a unique pointer to a GTXLReader.
		 */
		typedef unique_ptr<GTXLReader> GTXLReader_up;

	} /* namespace io */
} /* namespace uncover */

#endif /* GTXLREADER_H_ */
