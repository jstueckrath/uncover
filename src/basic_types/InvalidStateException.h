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

#ifndef INVALIDSTATEEXCEPTION_H_
#define INVALIDSTATEEXCEPTION_H_

#include "GenericMessageException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This exception is thrown if a function of an object was called, but the object was not in a
		 * valid state to call this function. This can happen for instance if a Timer object was not started
		 * before calling a the method to retrieve its elapsed time.
		 * @see Timer
		 * @author Jan Stückrath
		 */
		class InvalidStateException: public GenericMessageException {

			public:

				/**
				 * Creates a new InvalidStateException with the specified string as message.
				 * @param messageBody the message of the exception
				 */
				InvalidStateException(string messageBody);

				/**
				 * Destroys the current instance of this InvalidStateException.
				 */
				virtual ~InvalidStateException() throw();

		};

	} /* namespace exceptions */
} /* namespace uncover */
#endif /* TIMERSTATEEXCEPTION_H_ */
