/***************************************************************************
 *   Copyright (C) 2015 by Jan Stückrath <jan.stueckrath@uni-due.de>       *
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

#include "MultiStream.h"
#include <iostream>

using namespace std;

namespace uncover {
	namespace logging {

		MultiStream::MultiStream() : ostream(&buf) {
			buf.addBuffer(cout.rdbuf());
		}

		MultiStream::~MultiStream() {}

		void MultiStream::addOStream(shared_ptr<std::ostream> str) {
			streams.push_back(str);
			buf.addBuffer(str->rdbuf());
		}

		size_t MultiStream::removeOStream(shared_ptr<std::ostream> str) {
			size_t del = 0;
			auto it = streams.begin();
			while(it != streams.end()) {
				if(*it == str) {
					it = streams.erase(it);
					++del;
				} else {
					++it;
				}
			}
			buf.removeBuffer(str->rdbuf());
			return del;
		}

	} /* namespace logging */
} /* namespace uncover */
