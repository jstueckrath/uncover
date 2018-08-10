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

#ifndef XMLIOEXCEPTION_H_
#define XMLIOEXCEPTION_H_

#include "../basic_types/GenericMessageException.h"

namespace uncover {
	namespace io {

		/**
		 * This exception is thrown if an error occurred while importing or exporting XML documents. An XMLioException
		 * may also encapsulate exceptions of the underlying XML framework used.
		 * @author Jan Stückrath
		 */
		class XMLioException : public basic_types::GenericMessageException {

			public:

				/**
				 * Generates a new XMLioException with the given message as body.
				 * @param messageBody the exception message
				 */
				XMLioException(string messageBody);

				/**
				 * Destroys the XMLioException instance.
				 */
				virtual ~XMLioException() throw();

		};

	} /* namespace exceptions */
} /* namespace uncover */

#endif /* XMLIOEXCEPTION_H_ */
