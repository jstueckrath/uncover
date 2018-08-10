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

#ifndef INITIALISATIONEXCEPTION_H_
#define INITIALISATIONEXCEPTION_H_

#include "GenericMessageException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This exception is thrown if an object could not be initialized successfully. It is mostly used in conjunction with Scenarios.
		 * @author Jan Stückrath
		 * @see basic_types::Scenario
		 */
		class InitialisationException : public GenericMessageException {

			public:

				/**
				 * Generates a new InitialisationException with the given message as body.
				 * @param messageBody the exception message
				 */
				InitialisationException(std::string messageBody);

				/**
				 * Destroys the InitialisationException instance.
				 */
				virtual ~InitialisationException() throw();

			private:

				/**
				 * Parameterless constructor is hidden.
				 */
				InitialisationException();

		};

	}
}

#endif /* INITIALISATIONEXCEPTION_H_ */
