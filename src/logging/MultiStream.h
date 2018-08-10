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

#ifndef MULTISTREAM_H_
#define MULTISTREAM_H_

#include <ostream>
#include "../basic_types/globals.h"
#include "MultiStreamBuffer.h"

// TODO comment this class

namespace uncover {
	namespace logging {

		class MultiStream : public std::ostream {

			public:

				MultiStream();

				virtual ~MultiStream();

				void addOStream(shared_ptr<std::ostream> str);

				size_t removeOStream(shared_ptr<std::ostream> str);

			private:

				MultiStreamBuffer buf;

				vector<shared_ptr<std::ostream>> streams;
		};

	} /* namespace logging */
} /* namespace uncover */

#endif
