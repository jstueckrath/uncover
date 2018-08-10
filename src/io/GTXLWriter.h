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

#ifndef GTXLWRITER_H_
#define GTXLWRITER_H_

#include "../graphs/GTS.h"
#include <memory>
#include <boost/filesystem.hpp>

namespace uncover {
	namespace io {

		/**
		 * This class exports GXL or GTXL XML files by converting them from Hypergraph and GTS objects respectively.
		 * @author Jan Stückrath
		 */
		class GTXLWriter {

			public:

				/**
				 * Destroys the GTXLWriter object.
				 */
				virtual ~GTXLWriter();

				/**
				 * This method takes a collection of Hypergraphs and stores the collection as a GXL XML-file with the given filename.
				 * If the file already exists, it is overwritten (if possible).
				 * @param graphs a pointer to a vector of Hypergraphs to be stored
				 * @param filename the filename of the resulting GXL file
				 */
				virtual void writeGXL(vector<graphs::Hypergraph_sp> const& graphs, string filename) const;

				/**
				 * This method takes a collection of Hypergraphs and stores the collection as a GXL XML-file with the given filename.
				 * If the file already exists, it is overwritten (if possible).
				 * @param graphs a pointer to a vector of Hypergraphs to be stored
				 * @param filename the filename of the resulting GXL file
				 */
				virtual void writeGXL(vector<graphs::Hypergraph_sp> const& graphs, boost::filesystem::path& filename) const = 0;

				/**
				 * This method takes a single Hypergraph and stores it as a GXL XML-file with the given filename.
				 * If the file already exists, it is overwritten (if possible).
				 * @param graph a pointer to the Hypergraph to be stored
				 * @param filename the filename of the resulting GXL file
				 */
				virtual void writeGXL(graphs::Hypergraph_sp graph, string filename) const;

				/**
				 * This method takes a single Hypergraph and stores it as a GXL XML-file with the given filename.
				 * If the file already exists, it is overwritten (if possible).
				 * @param graph a pointer to the Hypergraph to be stored
				 * @param filename the filename of the resulting GXL file
				 */
				virtual void writeGXL(graphs::Hypergraph_sp graph, boost::filesystem::path& filename) const;

				/**
				 * This method takes a GTS object and stores it as a GTXL XML-file with the given filename.
				 * If the file already exists, it is overwritten (if possible).
				 * @param gts the GTS object to be stored
				 * @param filename the filename of the resulting GTXL file
				 */
				virtual void writeGTXL(graphs::GTS const& gts, string filename) const;

				/**
				 * This method takes a GTS object and stores it as a GTXL XML-file with the given filename.
				 * If the file already exists, it is overwritten (if possible).
				 * @param gts the GTS object to be stored
				 * @param filename the filename of the resulting GTXL file
				 */
				virtual void writeGTXL(graphs::GTS const& gts, boost::filesystem::path& filename) const = 0;

				/**
				 * Sets whether the graph IDs used internally will be stored in the resulting file.
				 * @param value use 'true' if the graph IDs will be stored
				 */
				void setGraphIDStorage(bool value);

				/**
				 * Returns whether internal graph IDs will be exported.
				 * @return true, iff internal graph IDs will exported
				 */
				bool getGraphIDStorage() const;

				/**
				 * Sets whether internal rule IDs should be stores in the resulting file.
				 * @param value use 'true if the rule IDs should be stored
				 */
				void setRuleIDStorage(bool value);

				/**
				 * Returns whether internal rule IDs will be exported.
				 * @return ture, iff internal rule IDs will be exported
				 */
				bool getRuleIDStorage() const;

				/**
				 * Use this method to generate a new GTXLWriter object. No initialization is needed before use.
				 * @warning The caller has to properly delete the GTXLWriter after use to avoid memory leaks.
				 * @return a pointer to a new GTXLWriter object
				 */
				static GTXLWriter* newWriter();

				/**
				 * Use this method to generate a new GTXLWriter object. No initialization is needed before use.
				 * @return a pointer to a new GTXLWriter object
				 */
				static unique_ptr<GTXLWriter> newUniqueWriter();

			protected:

				/**
				 * The default constructor is hidden.
				 */
				GTXLWriter();

				/**
				 * A value of true will cause the internally used ID of a graph to be stored as its name in the
				 * exported file.
				 */
				bool storeGraphIDs;

				/**
				 * A value of true will cause the internally used ID of a rule to be stored as its name in the
				 * exported file.
				 */
				bool storeRuleIDs;

		};

	} /* namespace io */
} /* namespace uncover */

#endif /* GTXLWRITER_H_ */
