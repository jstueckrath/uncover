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

#ifndef OUTOFIDSEXCEPTION_H_
#define OUTOFIDSEXCEPTION_H_

#include "GenericMessageException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This exception is thrown if an object was about to be created, but the programm has run out of unique IDs.
		 * @author Jan Stückrath
		 */
		class OutOfIDsException : public GenericMessageException {

			public:

				/**
				 * Generates a new OutOfIDsException with the given message as body.
				 * @param messageBody the exception message
				 */
				OutOfIDsException(string messageBody);

				/**
				 * Destroys the OutOfIDsException instance.
				 */
				virtual ~OutOfIDsException() throw();

		};

	} /* namespace exceptions */
} /* namespace uncover */
#endif /* OUTOFIDSEXCEPTION_H_ */
