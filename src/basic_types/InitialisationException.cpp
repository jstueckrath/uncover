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

#include "InitialisationException.h"
#include <sstream>

namespace uncover {
	namespace basic_types {

		// This constructor cannot/should not be called
		InitialisationException::InitialisationException() :
				GenericMessageException("InitialisationException", "!!! no exception text specified !!!") {}

		InitialisationException::InitialisationException(std::string messageBody) :
				GenericMessageException("InitialisationException", messageBody) {}

		InitialisationException::~InitialisationException() throw() {}

	}
}
