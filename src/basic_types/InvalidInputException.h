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

#ifndef INVALIDINPUTEXCEPTION_H_
#define INVALIDINPUTEXCEPTION_H_

#include "GenericMessageException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This exception is thrown whenever a function is called with invalid inputs. This exception
		 * is only used if the invalid input cannot be ignored, e.g. by returning null.
		 * @author Jan Stückrath
		 */
		class InvalidInputException : public GenericMessageException {

			public:

				/**
				 * Generates a new exception with the given error text.
				 */
				InvalidInputException(string message);

				/**
				 * Destroys this instance of InvalidInputException.
				 */
				virtual ~InvalidInputException() throw();

		};

	} /* namespace basic_types */
} /* namespace uncover */

#endif /* INVALIDINPUTEXCEPTION_H_ */
