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

#ifndef DELAYEDLOGMESSAGE_H_
#define DELAYEDLOGMESSAGE_H_

#include "message_logging.h"

namespace uncover {
	namespace logging {

		/**
		 * This class stores a log message and sends it at the end of the scope in which it was defined.
		 * @author Jan Stückrath
		 */
		class DelayedLogMessage
		{

			public:

				/**
				 * Creates a new DelayedLogMessage of the given type and with the given content.
				 * @param logType the type of log message
				 * @param message the content of the message
				 */
				DelayedLogMessage(LogTypes logType, string message);

				/**
				 * Destroys this DelayedLogMessage, immediately printing its stored message.
				 */
				~DelayedLogMessage();

			private:

				/**
				 * Stores the log type of this DelayedLogMessage.
				 */
				LogTypes logType;

				/**
				 * Stores the content of this DelayedLogMessage.
				 */
				string message;

		};

	} /* namespace logging */
} /* namespace uncover */

#endif /* DELAYEDLOGMESSAGE_H_ */
