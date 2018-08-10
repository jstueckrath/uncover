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

#ifndef IDTOIDENUMERATOR_H_
#define IDTOIDENUMERATOR_H_

#include "../basic_types/globals.h"
#include "../basic_types/InvalidStateException.h"

namespace uncover {
	namespace rule_engine {

		/**
		 * This enumerator encapsulates a mapping of an ID to a set of IDs. It is initialized
		 * by a source ID and a set of target IDs. After initialization it maps the source ID
		 * to one of the target IDs. By incrementing, this mapping can be changed to the next
		 * target ID (no order guaranteed). The enumerator ends if every target ID was mapped
		 * to exactly once.
		 * @author Jan Stückrath
		 */
		class IDtoIDEnumerator {

			public:

				/**
				 * Generates a new IDtoIDEnumerator with the given source ID and target IDs. If
				 * the set of target IDs is empty, the enumerator immediately ends.
				 * @param sourceID the ID which will me mapped from
				 * @param targetIDs a set of IDs to be mapped to
				 */
				IDtoIDEnumerator(IDType sourceID, unordered_set<IDType> const& targetIDs);

				/**
				 * Move constructor. Generates a new IDtoIDEnumerator with the data of the old one.
				 * The old enumerator is invalidated and will throw exceptions if used again.
				 * @param oldEnum the enumerator of which the data will be moved
				 */
				IDtoIDEnumerator(IDtoIDEnumerator&& oldEnum);

				/**
				 * Destroys the current instance of this enumerator.
				 */
				virtual ~IDtoIDEnumerator();

				/**
				 * Retrieves the next ID to map to, if possible. This operator does nothing if
				 * the enumerator has ended.
				 * @return a reference to this enumerator
				 */
				IDtoIDEnumerator& operator++();

				/**
				 * Resets the enumeration to start from the beginning.
				 */
				void reset();

				/**
				 * Returns true iff the enumerator could not retrieve another ID to map to.
				 * @return true iff there where no more IDs to map to
				 */
				bool ended() const;

				/**
				 * Returns the ID to map from.
				 * @return the ID to map from
				 */
				IDType const& getSource() const;

				/**
				 * Return the current ID to map to.
				 * @return the current ID to map to
				 */
				IDType const& getTarget() const;

			private:

				/**
				 * This constructor cannot/should not be called.
				 */
				IDtoIDEnumerator();

				/**
				 * Stores the ID mapped from.
				 */
				const IDType source;

				/**
				 * Stores the set of IDs mapped to.
				 */
				unordered_set<IDType> const* targets;

				/**
				 * Stores an iterator to the current ID mapped to.
				 */
				unordered_set<IDType>::const_iterator iter;

		};

	} /* namespace rule_engine */
} /* namespace uncover */

#endif /* IDTOIDENUMERATOR_H_ */
