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

#ifndef GENERICMESSAGEEXCEPTION_H_
#define GENERICMESSAGEEXCEPTION_H_

#include <exception>
#include "../basic_types/globals.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This class serves as a superclass of all Exceptions. It just provides a simple storage of a message.
		 * @author Jan Stückrath
		 */
		class GenericMessageException : public std::exception {

			public:

				/**
				 * Destroys the GenericMessageException instance.
				 */
				virtual ~GenericMessageException() throw();

				/**
				 * Returns a string representation of the exception. This contains the exceptions name and the actual message.
				 * @return a string representation (as const char*) of the exception
				 */
				virtual const char* what() const throw() override;

				/**
				 * Streams the string representation of the exception using the given ostream.
				 * @param ost an arbitrary ostream
				 * @param data the exception to be streamed
				 * @return the given ostream ost
				 */
				friend std::ostream& operator<< (std::ostream& ost, GenericMessageException& data);

			protected:

				/**
				 * Creates a new GenericMessageException form the input text.
				 * @param exceptionName the name of the (derived) exception
				 * @param messageBody the actual error message
				 */
				GenericMessageException(string exceptionName, string messageBody);

				/**
				 * Stores the message of the exception.
				 */
				string exceptionMessage;

		};

	} /* namespace exceptions */
} /* namespace uncover */

#endif /* GENERICMESSAGEEXCEPTION_H_ */
