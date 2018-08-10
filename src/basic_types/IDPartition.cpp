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

#include "IDPartition.h"

using namespace std;

namespace uncover {
	namespace basic_types {

		bool idpart_intersect(SubIDPartition const& part1, SubIDPartition const& part2) {
			for(auto p1It = part1.cbegin(); p1It != part1.cend(); ++p1It) {
				if(part2.count(*p1It)) {
					return true;
				}
			}
			return false;
		}

		IDPartition idpart_transitive_closure(IDPartition const& part1, IDPartition const& part2) {
			IDPartition result(part1);
			idpart_transitive_closure_into1(result,part2);
			return result;
		}

		void idpart_transitive_closure_into1(IDPartition& part1, IDPartition const& part2) {
			for(IDPartition::const_iterator p2It = part2.cbegin(); p2It != part2.cend(); ++p2It) {

				unordered_set<IDType> newClass = *p2It;
				vector<IDPartition::const_iterator> toDelete;
				for(IDPartition::const_iterator resIt = part1.begin(); resIt != part1.cend(); ++resIt) {
					if(idpart_intersect(*p2It,*resIt)) {
						newClass.insert(resIt->begin(), resIt->end());
						toDelete.push_back(resIt);
					}
				}

				for(vector<IDPartition::const_iterator>::iterator delIt = toDelete.begin(); delIt != toDelete.end(); ++delIt) {
					part1.erase(*delIt);
				}

				part1.insert(newClass);
			}
		}

	}
}
