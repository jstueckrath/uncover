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

#ifndef MULTISTREAMBUFFER_H_
#define MULTISTREAMBUFFER_H_

#include <streambuf>
#include <vector>

// TODO comment this class

namespace uncover {
	namespace logging {

		class MultiStreamBuffer : public std::streambuf {

			friend class MultiStream;

			public:

				MultiStreamBuffer();

				virtual ~MultiStreamBuffer();

			protected:

		    virtual int overflow(int c) override;

		    virtual int sync() override;

		    void addBuffer(std::streambuf* buffer);

		    size_t removeBuffer(std::streambuf* buffer);

			private:

		    std::vector<std::streambuf*> buffers;

		};


	} /* namespace logging */
} /* namespace uncover */

#endif /* MULTISTREAMBUFFER_H_ */
