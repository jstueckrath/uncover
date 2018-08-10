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

#ifndef INSTANTWRITERRESULTHANDLER_H_
#define INSTANTWRITERRESULTHANDLER_H_

#include "BackwardResultHandler.h"
#include "../io/GTXLWriter.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace uncover {
	namespace analysis {

		/**
		 * The InstantWriterResultHandler implements the BackwardResultHandler by writing the given
		 * information as soon as possible to the filesystem. For this only a storage folder has to be given.
		 * @author Jan Stückrath
		 * @see BackwardResultHandler
		 */
		class InstantWriterResultHandler: public BackwardResultHandler {

			public:

				/**
				 * Describes the extent to which the information should be stored.
				 */
				enum Extent {
					/** Stores all given information. */
					FULL = 0,
					/** Stores only the GTSs and the IDs of graph, but not the graphs themselves. */
					ID_FILES_AND_RULES = 10,
					/** Stores only the IDs of graph, but not the graphs or GTSs. */
					ID_FILES = 20};

				/**
				 * Creates a new InstantWriterResultHandler storing all information to the given folder.
				 * @param foldername a path to the storage folder
				 */
				InstantWriterResultHandler(boost::filesystem::path& foldername);

				/**
				 * Destroys this instance of InstantWriterResultHandler.
				 */
				virtual ~InstantWriterResultHandler();

				virtual void setOldGTS(graphs::GTS_csp gts) override;

				virtual void setPreparedGTS(PreparedGTS_csp gts) override;

				virtual void setFirstGraphs(shared_ptr<const vector<graphs::Hypergraph_sp>> graphs) override;

				virtual void wasDeletedBecauseOf(IDType deletedGraph, IDType reasonGraph) override;

				virtual void wasNoValidGraph(IDType deletedGraph) override;

				virtual void wasGeneratedByFrom(graphs::Hypergraph_sp genGraph, IDType rule, IDType srcGraph) override;

				virtual void startingNewBackwardStep() override;

				virtual void finishedBackwardStep() override;

			private:

				/**
				 * This constructor cannot/should not be called.
				 */
				InstantWriterResultHandler();

				/**
				 * Stores the path to the storage folder.
				 */
				boost::filesystem::path foldername;

				/**
				 * A GTXLWriter used to write the information to the file system.
				 */
				unique_ptr<io::GTXLWriter> gtxlWriter;

				/**
				 * The file stream where information about the generation of new graphs are stored.
				 */
				boost::filesystem::ofstream graphGenerationFile;

				/**
				 * The file stream where information about the deletion of old graphs are stored.
				 */
				boost::filesystem::ofstream graphDeletionFile;

				/**
				 * A collection temporary storing all graphs generated in the current backward step.
				 */
				vector<graphs::Hypergraph_sp> currentStepGraphs;

				/**
				 * A counter storing the index of the current backward step.
				 */
				unsigned int stepnr;

				/**
				 * Stores the extent to which information is stored.
				 */
				InstantWriterResultHandler::Extent extent;

		};

	} /* namespace analysis */
} /* namespace uncover */
#endif /* INSTANTWRITERRESULTHANDLER_H_ */
