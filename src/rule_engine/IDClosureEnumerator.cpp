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

#include "IDClosureEnumerator.h"
#include "../logging/message_logging.h"

using namespace uncover::basic_types;
using namespace uncover::logging;

namespace uncover {
	namespace rule_engine {

		IDClosureEnumerator::IDClosureEnumerator(
				unordered_set<IDType> const& idSet,
				basic_types::IDPartition const& partition)
			: enumerator(new IDPartitionEnumerator(idSet)),
			  otherPart(new IDPartition(partition)),
			  finished(false) {

			if(enumerator->ended()) {
				enumerator = nullptr;
				otherPart = nullptr;
				logger.newWarning() << "An IDClosureEnumerator was initialized, but its encapsulated IDPartitionEnumerator "
						"failed to create at least one IDPartition. The IDClosureEnumerator was invalidated." << endLogMessage;
				return;
			}

			this->reset();

		}

		IDClosureEnumerator::IDClosureEnumerator(IDClosureEnumerator&& oldEnum)
			: enumerator(oldEnum.enumerator),
			  otherPart(oldEnum.otherPart),
			  finished(oldEnum.finished){
			oldEnum.enumerator = nullptr;
			oldEnum.otherPart = nullptr;
		}

		IDClosureEnumerator::~IDClosureEnumerator() {
			if(enumerator) delete enumerator;
			if(otherPart) delete otherPart;
		}

		bool IDClosureEnumerator::isValid() const {
			return enumerator != nullptr && otherPart != nullptr;
		}

		bool IDClosureEnumerator::ended() const {
			throwIfNotValid();
			return finished;
		}

		void IDClosureEnumerator::throwIfNotValid() const {
			if(enumerator == nullptr || otherPart == nullptr) {
				throw InvalidStateException("This IDClosureEnumerator was invalidated by a move constructor call or "
						"initialization failed. No further methods of this IDClosureEnumerator may be called.");
			}
		}

		void IDClosureEnumerator::reset() {
			throwIfNotValid();
			finished = false;
			enumerator->reset();

			if(idpart_transitive_closure(**enumerator,*otherPart).size() != 1) {
				++(*this);
			}
		}

		IDClosureEnumerator& IDClosureEnumerator::operator++() {

			throwIfNotValid();
			if(!finished) {

				do {

					++(*enumerator);
					if(enumerator->ended()) {
						finished = true;
						return *this;
					}

				} while(idpart_transitive_closure(**enumerator,*otherPart).size() != 1);

			}

			return *this;
		}

		const IDPartition& IDClosureEnumerator::operator*() const {
			throwIfNotValid();
			if(finished) {
				throw InvalidStateException("An IDClosureEnumerator was dereferences after it has ended.");
			}
			return **enumerator;
		}

		std::ostream& operator<< (std::ostream& ost, IDClosureEnumerator const& data) {

			ost << "IDClosureEnumerator[stored partition=";
			if(data.otherPart) {
				ost << (*data.otherPart);
			} else {
				ost << "nullptr";
			}
			ost << ", current partition=";
			if(data.finished) {
				ost << "ended]";
			} else {
				if(data.enumerator) {
					ost << (**data.enumerator);
				} else {
					ost << "nullptr";
				}
				ost << "]";
			}
			return ost;
		}

	} /* namespace rule_engine */
} /* namespace uncover */
