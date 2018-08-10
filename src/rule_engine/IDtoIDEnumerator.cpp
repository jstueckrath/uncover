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

#include "IDtoIDEnumerator.h"

using namespace uncover::basic_types;

namespace uncover {
	namespace rule_engine {

		// this enumerator cannot/should not be called
		IDtoIDEnumerator::IDtoIDEnumerator() :
			source(0),
			targets(nullptr),
			iter() {}

		IDtoIDEnumerator::IDtoIDEnumerator(IDType sourceID, unordered_set<IDType> const& targetIDs) :
			source(sourceID),
			targets(new unordered_set<IDType>(targetIDs)),
			iter(targets->cbegin()) {}

		IDtoIDEnumerator::IDtoIDEnumerator(IDtoIDEnumerator&& oldEnum)
			:	source(oldEnum.source),
			 	targets(oldEnum.targets),
			 	iter(oldEnum.iter) {
			oldEnum.iter = targets->cend();
			oldEnum.targets = nullptr;
		}

		IDtoIDEnumerator::~IDtoIDEnumerator() {
			if(targets) delete targets;
		}

		IDtoIDEnumerator& IDtoIDEnumerator::operator ++() {
			if(iter != targets->cend()) ++iter;
			return *this;
		}

		void IDtoIDEnumerator::reset() {
			iter = targets->cbegin();
		}

		bool IDtoIDEnumerator::ended() const {
			return iter == targets->cend();
		}

		const IDType& IDtoIDEnumerator::getSource() const {
			return source;
		}

		const IDType& IDtoIDEnumerator::getTarget() const {
			if(iter == targets->cend()) {
				throw InvalidStateException("IDtoIDEnumerator cannot return target ID, because there is none left.");
			}
			return *iter;
		}

	} /* namespace rule_engine */
} /* namespace uncover */
