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

#include "MultiStreamBuffer.h"

namespace uncover {
	namespace logging {

		MultiStreamBuffer::MultiStreamBuffer() : buffers() {}

		MultiStreamBuffer::~MultiStreamBuffer() {}

		int MultiStreamBuffer::overflow(int c) {
      if (c == EOF)
      {
          return EOF;
      }
      else
      {
      	bool success = true;
      	for(auto it = buffers.cbegin(); it != buffers.cend(); ++it) {
      		success = ((*it)->sputc(c) != EOF) && success;
      	}
      	if(success) {
      		return c;
      	} else {
      		return EOF;
      	}
      }
		}

		int MultiStreamBuffer::sync() {
			bool success = true;
			for(auto it = buffers.cbegin(); it != buffers.cend(); ++it) {
				success = ((*it)->pubsync() == 0) && success;
			}
			if(success) {
				return 0;
			} else {
				return -1;
			}
		}

    void MultiStreamBuffer::addBuffer(std::streambuf* buffer) {
    	buffers.push_back(buffer);
    }

    size_t MultiStreamBuffer::removeBuffer(std::streambuf* buffer) {
			size_t del = 0;
			auto it = buffers.begin();
			while(it != buffers.end()) {
				if(*it == buffer) {
					it = buffers.erase(it);
					++del;
				} else {
					++it;
				}
			}
			return del;
    }

	} /* namespace logging */
} /* namespace uncover */
