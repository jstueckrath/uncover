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

#include "GenericMessageException.h"
#include <sstream>

namespace uncover {
	namespace basic_types {

		GenericMessageException::GenericMessageException(std::string exceptionName, std::string messageBody)
			: exceptionMessage(exceptionName + ": " + messageBody) {}

		GenericMessageException::~GenericMessageException() throw() {}

		const char* GenericMessageException::what() const throw() {
			return exceptionMessage.c_str();
		}

		std::ostream& operator<< (std::ostream& ost, GenericMessageException& data)
		{
			ost << data.exceptionMessage;
			return ost;
		}

	} /* namespace exceptions */
} /* namespace uncover */
