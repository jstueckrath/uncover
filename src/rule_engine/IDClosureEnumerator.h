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

#ifndef IDCLOSUREENUMERATOR_H_
#define IDCLOSUREENUMERATOR_H_

#include "../basic_types/IDPartitionEnumerator.h"

namespace uncover {
	namespace rule_engine {

		/**
		 * This enumerator can be used to enumerate all equivalences on IDs, where the equivalence closure of
		 * the enumerated IDPartition and a predefined IDPartition is the IDPartition, where every ID is in
		 * the same partition.
		 * @see IDPartition
		 * @see IDPartitionEnumerator
		 * @author Jan Stückrath
		 */
		class IDClosureEnumerator {

			public:

				/**
				 * Generates a new IDClosureEnumerator for the given ID set and the given partition. All enumerated
				 * partitions will be partitions on the given ID set. Every partition on the given ID set will be
				 * enumerated, if the equivalence closure of it and the given partition relates all its elements.
				 * @param idSet the set of IDs for which partitions will be enumerated
				 * @param partition the partition with which the closure with the enumerated partition will be formed
				 */
				IDClosureEnumerator(unordered_set<IDType> const& idSet, basic_types::IDPartition const& partition);

				/**
				 * Move constructor. Moves all computations and results of the given enumerator to this new
				 * enumerator, while invalidating the given enumerator. Calling functions of the old enumerator
				 * will probably throw exceptions.
				 * @param oldEnum the enumerator from which data will be (re)moved from
				 */
				IDClosureEnumerator(IDClosureEnumerator&& oldEnum);

				/**
				 * Destroys this instance of the enumerator.
				 */
				virtual ~IDClosureEnumerator();

				/**
				 * Returns whether the enumerator is still valid or not. An IDClosureEnumerator is invalidated only
				 * by a call of the move constructor.
				 * @return true iff the enumerator is valid
				 */
				bool isValid() const;

				/**
				 * Return whether the enumerator has ended, i.e. the last IDPartition was enumerated.
				 * Throws an exception if enumerator is not valid.
				 * @return true iff the last IDPartition was enumerated
				 */
				bool ended() const;

				/**
				 * Resets the computation of this enumerator, such that it will enumerate partitions
				 * beginning with the first again. Throws an exception if enumerator is not valid.
				 */
				void reset();

				/**
				 * Computes the next valid IDPartition. Throws an exception if enumerator is not valid.
				 * @return a reference to this enumerator
				 */
				IDClosureEnumerator& operator++();

				/**
				 * Return the last computed IDPartition. Throws an exception if enumerator is not valid.
				 * @return the last computed IDPartition
				 */
				basic_types::IDPartition const& operator*() const;

				/**
				 * Prints the most important contents of the given IDClosureEnumerator.
				 * @param ost the stream to which the data will be printed
				 * @param data the enumerator which should be printed
				 * @return a reference to the used stream
				 */
				friend std::ostream& operator<< (std::ostream& ost, IDClosureEnumerator const& data);

			private:

				/**
				 * Checks if the enumerator is valid and throws an exception if it is not.
				 */
				void throwIfNotValid() const;

				/**
				 * Stores an enumerator to enumerate all possible IDPartitions.
				 */
				basic_types::IDPartitionEnumerator* enumerator;

				/**
				 * Stores the partition with which the equivalence closure will be formed.
				 */
				const basic_types::IDPartition* otherPart;

				/**
				 * Stores whether the enumerator finished its computation or not.
				 */
				bool finished;

		};

	} /* namespace rule_engine */
} /* namespace uncover */

#endif /* IDCLOSUREENUMERATOR_H_ */
