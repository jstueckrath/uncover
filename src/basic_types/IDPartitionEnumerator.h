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

#ifndef IDPARTITIONENUMERATOR_H_
#define IDPARTITIONENUMERATOR_H_

#include "IDPartition.h"
#include "InvalidStateException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * A hasher for pointers to IDPartions.
		 */
		struct IDPartitionPointerHasher {

			/**
			 * Takes a pointer to an IDPartition and returns a hash value. The hash value is computed
			 * from the contents of the IDPartition and not based on the pointer.
			 * @param s the pointer to the IDPartition to be hashed
			 * @return the hash value
			 */
	    inline size_t operator()(const IDPartition* s) const;

	  };

		/**
		 * An equality checker for pointers to IDPartitions.
		 */
		struct IDPartitionPointerEquality {

			/**
			 * Checks if the given IDPartitions pointed to are equal. The check is based on the contents
			 * of the IDPartitions and not the pointers itself.
			 * @param x a pointer to the first IDPartition
			 * @param y a pointer to the second IDPartition
			 * @return true, iff the IDPartitions pointed to are equal
			 */
		  bool operator() (const IDPartition* x, const IDPartition* y) const;

		};

		/**
		 * Provides a possibility to enumerate all partitions on a given set of IDs. Initialized with the
		 * ID set, it behaves like an iterator independent of any collection (although not satisfying all
		 * necessary properties of such). This enumerator can be moved, but cannot be duplicated!
		 * @author Jan Stückrath
		 */
		class IDPartitionEnumerator {

			public:

				/**
				 * Creates a new IDPartitionEnumerator with the given IDs. Duplicate IDs in the given collection
				 * are ignored.
				 * @param idSet a collection of IDs
				 */
				IDPartitionEnumerator(vector<IDType> const& idSet);

				/**
				 * Creates a new IDPartitionEnumerator with the given IDs.
				 * @param idSet a collection of IDs
				 */
				IDPartitionEnumerator(unordered_set<IDType> const& idSet);

				/**
				 * Moves the data of the given enumerator to a new IDPartitionEnumerator. The new IDPartitionEnumerator
				 * has the same state as the old enumerator and the old enumerator is invalidated. The old
				 * enumerator may now longer be used!
				 * @param oldEnum an rvalue reference to an IDPartitionEnumerator
				 */
				IDPartitionEnumerator(IDPartitionEnumerator&& oldEnum);

				/**
				 * Destroy this instance of the enumerator.
				 */
				virtual ~IDPartitionEnumerator();

				/**
				 * Return whether this IDPartitionEnumerator is in a valid state or not.
				 * @return true, iff this IDPartitionEnumerator is in a valid state
				 */
				bool isValid() const;

				/**
				 * Returns true, if all possible IDPartions were enumerated. This will be the case as soon
				 * as the ++-operator was called and unable to create any further IDPartitions.
				 * @return true, iff all possible IDPartions were enumerated
				 */
				bool ended() const;

				/**
				 * Resets this enumerator to start the enumeration process from the first element.
				 */
				void reset();

				/**
				 * Returns the size of the set of element on which partitions are enumerated.
				 * @return the size of the set of element on which partitions are enumerated
				 */
				size_t baseSetSize() const;

				/**
				 * Computes the next IDPartition in the enumeration. If there are no more IDPartitions, this
				 * enumerator is marked to have ended, but does not report an error. If this enumerator has
				 * ended, this operator does nothing (including reporting an error!).
				 * @return a reference to itself (after computing the next IDPartition)
				 */
				IDPartitionEnumerator& operator++();

				/**
				 * Returns the last IDPartition which was computed. If the enumerator has ended, this operator
				 * will not report an error, but return the (very) last IDPartition computed.
				 * @return the last IDPartition computed
				 */
        IDPartition const& operator*() const;

			private:

        /**
         * Generates an enumerator with no IDs. This constructor can/should only be called internally.
         */
				IDPartitionEnumerator();

				/**
				 * Checks if this IDPartitionEnumerator is still valid and throws an InvalidStateException if
				 * it is not.
				 */
				void throwIfNotValid() const;

				/**
				 * Stores at which index of genPartitionsVec the current IDPartition is stored.
				 */
				size_t current;

				/**
				 * Stores the index of the first element of genPartitionsVec which was not already used to
				 * create more coarse IDPartitions.
				 */
				size_t lastNew;

				/**
				 * Stores all enumerated IDPartitions, where the order of the elements is the sequence in which
				 * the IDPartitions were computed.
				 */
				vector<IDPartition*>* genPartitionsVec;

				/**
				 * Stores all enumerated IDPartitions to avoid enumerating the same IDPartition multiple times.
				 */
				unordered_set<IDPartition*, IDPartitionPointerHasher, IDPartitionPointerEquality>* genPartitionsSet;

				/**
				 * Stores whether this IDPartitionEnumerator is still valid.
				 */
				bool valid;

		};

	} /* namespace basic_types */
} /* namespace uncover */

#endif /* IDPARTITIONENUMERATOR_H_ */
