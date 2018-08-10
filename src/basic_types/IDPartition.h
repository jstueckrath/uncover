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

#ifndef IDPARTITION_H_
#define IDPARTITION_H_

#include "globals.h"
#include "hash_functions.h"

namespace uncover {
	namespace basic_types {

		/**
		 * A SubIDPartition is a set of IDs used by IDPartition.
		 */
		typedef unordered_set<IDType> SubIDPartition;

		/**
		 * An IDPartition is a set of sets of IDs (which is effectively a set of equivalence classes).
		 */
		typedef unordered_set<SubIDPartition> IDPartition;

		/**
		 * Checks if two sets of IDs intersect, i.e. if there is an ID which is in both given sets.
		 * @param part1 the first set of IDs
		 * @param part2 the second set of IDs
		 * @return true, iff there is an ID which is in both given sets
		 */
		extern bool idpart_intersect(SubIDPartition const& part1, SubIDPartition const& part2);

		/**
		 * Builds the transitive closure of two given IDPartitions. The result is the finest partition
		 * on the set of IDs of the two given IDPartitions, where two IDs are in the same partition, if they
		 * are in the same partition of at least one of the given IDPartitions.
		 * The given partitions need not be partitions on the same base set, but IDs in both partitions are
		 * identified by equality.
		 * @param part1 the first IDPartition (remains unchanged)
		 * @param part2 the second IDPartition (remains unchanged)
		 * @return the transitive closure of the given IDPartitions
		 */
		extern IDPartition idpart_transitive_closure(IDPartition const& part1, IDPartition const& part2);

		/**
		 * Builds the transitive closure of two given IDPartitions. The first given IDPartition is
		 * changed to be the transitive closure.
		 * @see #idpart_transitive_closure
		 * @param part1 the first IDPartition, which will be changed to the trantivie closure
		 * @param part2 the second IDPartition (remains unchanged)
		 */
		extern void idpart_transitive_closure_into1(IDPartition& part1, IDPartition const& part2);

	}
}

#endif /* IDPARTITION_H_ */
