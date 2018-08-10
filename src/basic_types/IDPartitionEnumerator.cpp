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

#include "IDPartitionEnumerator.h"

namespace uncover {
	namespace basic_types {

		/////////////////////////////////////////////////////////////////////////////////////////////
		// IDPartitionPointerHasher implementation
		/////////////////////////////////////////////////////////////////////////////////////////////

    inline size_t IDPartitionPointerHasher::operator()(const IDPartition* s) const
    {
    	if(s == nullptr) {
    		return 0;
    	}
    	std::hash<unordered_set<IDType>> hasher;
      size_t seed = 0;
      for(typename IDPartition::const_iterator it = s->cbegin(); it != s->cend(); ++it) {
				seed ^= hasher(*it);
      }
      return (seed << s->size()) + (seed >> s->size());
    }

		/////////////////////////////////////////////////////////////////////////////////////////////
		// IDPartitionPointerEquality implementation
		/////////////////////////////////////////////////////////////////////////////////////////////

	  bool IDPartitionPointerEquality::operator() (const IDPartition* x, const IDPartition* y) const {
	  	if(x == nullptr && y == nullptr) {
	  		return true;
	  	} else if(x == nullptr || y == nullptr) {
	  		return false;
	  	} else {
	  		return *x==*y;
	  	}
	  }

		/////////////////////////////////////////////////////////////////////////////////////////////
		// IDPartitionEnumerator implementation
		/////////////////////////////////////////////////////////////////////////////////////////////

		IDPartitionEnumerator::IDPartitionEnumerator() :
				current(0),
				lastNew(0),
				genPartitionsVec(new vector<IDPartition*>),
				genPartitionsSet(new unordered_set<IDPartition*, IDPartitionPointerHasher, IDPartitionPointerEquality>),
				valid(true) {}

		IDPartitionEnumerator::IDPartitionEnumerator(IDPartitionEnumerator&& oldEnum) :
			current(oldEnum.current),
			lastNew(oldEnum.lastNew),
			genPartitionsVec(oldEnum.genPartitionsVec),
			genPartitionsSet(oldEnum.genPartitionsSet),
			valid(true)
		{
			oldEnum.valid = false;
		}

		IDPartitionEnumerator::IDPartitionEnumerator(const vector<IDType>& idSet) : IDPartitionEnumerator()
		{

			// generate the first partition (all elements in different classes)
			unordered_set<IDType> IDchecklist;
			IDPartition* first = new IDPartition();
			for(vector<IDType>::const_iterator it = idSet.cbegin(); it != idSet.cend(); ++it) {
				if(IDchecklist.count(*it) == 0) {
					IDchecklist.insert(*it);
					first->insert(unordered_set<IDType>({*it}));
				} // else ignore ID, since it was already added

			}

			genPartitionsVec->push_back(first);
			genPartitionsSet->insert(first);

		}

		IDPartitionEnumerator::IDPartitionEnumerator(unordered_set<IDType> const& idSet) : IDPartitionEnumerator() {

			// generate the first partition (all elements in different classes)
			IDPartition* first = new IDPartition();
			for(unordered_set<IDType>::const_iterator it = idSet.cbegin(); it != idSet.cend(); ++it) {
				first->insert(unordered_set<IDType>({*it}));
			}

			genPartitionsVec->push_back(first);
			genPartitionsSet->insert(first);

		}

		IDPartitionEnumerator::~IDPartitionEnumerator() {

			// if this enumerator is invalid, another enumerator now owns is pointers, and nothing may be deleted!
			if(!valid) {
				return;
			}

			delete genPartitionsSet;

			for(vector<IDPartition*>::iterator vecIt = genPartitionsVec->begin(); vecIt != genPartitionsVec->end(); ++vecIt) {
				delete *vecIt;
			}

			delete genPartitionsVec;

		}

		bool IDPartitionEnumerator::isValid() const {
			return valid;
		}

		size_t IDPartitionEnumerator::baseSetSize() const {
			return genPartitionsVec->front()->size();
		}

		void IDPartitionEnumerator::throwIfNotValid() const {
			if(!valid) {
				throw InvalidStateException("This IDPartitionEnumerator was invalidated by a move constructor call. "
						"No further methods of this IDPartitionEnumerator may be called.");
			}
		}

		bool IDPartitionEnumerator::ended() const {
			throwIfNotValid();
			return current >= genPartitionsVec->size();
		}

		void IDPartitionEnumerator::reset() {
			throwIfNotValid();
			current = 0;
		}

		IDPartitionEnumerator& IDPartitionEnumerator::operator++() {

			throwIfNotValid();

			if(this->ended()) {
				// do nothing
			} else if(current < lastNew) {
				++current;
			} else if(genPartitionsVec->at(current)->size() <= 2 && genPartitionsVec->back()->size() == 1) {
				++current;
				++lastNew;
			} else {

				IDPartition* currentPart = genPartitionsVec->at(current);

				// merge all possible pairs of equivalence classes
				for(IDPartition::const_iterator outerIt = currentPart->cbegin(); outerIt != currentPart->cend(); ++outerIt) {
					for(IDPartition::const_iterator innerIt = currentPart->cbegin(); innerIt != currentPart->cend(); ++innerIt) {

						if(*outerIt != *innerIt) {
							IDPartition* newPart = new IDPartition(*currentPart);
							newPart->erase(*innerIt);
							newPart->erase(*outerIt);
							unordered_set<IDType> newPartElem = *outerIt;
							newPartElem.insert(innerIt->cbegin(), innerIt->cend());
							newPart->insert(newPartElem);

							if(genPartitionsSet->insert(newPart).second) {
								genPartitionsVec->push_back(newPart);
							} else {
								delete newPart;
							}
						}
					}
				}

				++current;
				++lastNew;
			}

			return *this;
		}

		IDPartition const& IDPartitionEnumerator::operator*() const {

			throwIfNotValid();

			if(this->ended()) {
				return *genPartitionsVec->back();
			} else {
				return *genPartitionsVec->at(current);
			}

		}

	} /* namespace basic_types */
} /* namespace uncover */
