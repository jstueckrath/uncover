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

#include "ExpEnumerator.h"

using namespace std;

namespace uncover {
	namespace rule_engine {

		ExpEnumerator::ExpEnumerator(size_t base, size_t exponent) :
				dataSize(exponent),
				data(new size_t[exponent]),
				base(base-1) {
			this->initialize();
		}

		ExpEnumerator::~ExpEnumerator() {
			if(data) delete[] data;
		}

		void ExpEnumerator::initialize() {
			for(size_t i = 0; i < dataSize; ++i) {
				data[i] = 0;
			}
		}

		bool ExpEnumerator::canCountUp() {
			for(size_t i = 0; i < dataSize; ++i) {
				if(data[i] < base) {
					return true;
				}
			}
			return false;
		}

		void ExpEnumerator::operator++() {
			for(size_t i = 0; i < dataSize; ++i) {
				if(data[i] < base) {
					data[i]++;
					return;
				} else { // data[i] = base
					data[i] = 0;
				}
			}
		}

		size_t ExpEnumerator::size() {
			return dataSize;
		}

		size_t& ExpEnumerator::operator[](size_t& index) {
			return data[index];
		}

	} /* namespace rule_engine */
} /* namespace uncover */
