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

#ifndef MESSAGELOGGER_H_
#define MESSAGELOGGER_H_

#include <ostream>
#include "../basic_types/globals.h"
#include "../basic_types/standard_operators.h"

namespace uncover {
	namespace logging {

		/**
		 * This class is used to stream log messages. Log messages of different types can be created by calling the appropriate functions,
		 * which will return a reference to an output stream. This stream can then be used to stream the message. If the message is finished,
		 * the endMsg() function needs to be called. If the message is still unfinished, contMsg() can be used to retrieve the current
		 * output stream.
		 * @author Jan Stückrath
		 */
		class MessageLogger {

			public:

				/**
				 * Creates a new MessageLogger which will use the given output stream to stream messages.
				 */
				MessageLogger(std::ostream& ostr);

				/**
				 * Destroys the current MessageLogger instance.
				 */
				virtual ~MessageLogger();

				/**
				 * Creates a new log message of the given type. This will create a header consisting of the current date, time and log type.
				 * The returned output stream can be used to write the messages content. The message must be ended via endMsg().
				 * @see endMsg()
				 * @param logType the log type of which the message should be
				 * @return an output stream reference to which the message can be written
				 */
				std::ostream& newMsg(LogTypes logType);

				/**
				 * Return the output stream for the currently streamed log message. If no message is streamed currently, then a new one is
				 * created using a default header.
				 * @return the output stream reference to which the message can be written
				 */
				std::ostream& contMsg();

				/**
				 * Creates a new log message for user information. This will create a header consisting of the current date, time and log type.
				 * The returned output stream can be used to write the messages content. The message must be ended via endMsg().
				 * @see endMsg()
				 * @return an output stream reference to which the message can be written
				 */
				std::ostream& newUserInfo();

				/**
				 * Creates a new log message for debug information. This will create a header consisting of the current date, time and log type.
				 * The returned output stream can be used to write the messages content. The message must be ended via endMsg().
				 * @see endMsg()
				 * @return an output stream reference to which the message can be written
				 */
				std::ostream& newDebug();

				/**
				 * Creates a new log message for warnings. This will create a header consisting of the current date, time and log type.
				 * The returned output stream can be used to write the messages content. The message must be ended via endMsg().
				 * @see endMsg()
				 * @return an output stream reference to which the message can be written
				 */
				std::ostream& newWarning();

				/**
				 * Creates a new log message for errors. This will create a header consisting of the current date, time and log type.
				 * The returned output stream can be used to write the messages content. The message must be ended via endMsg().
				 * @see endMsg()
				 * @return an output stream reference to which the message can be written
				 */
				std::ostream& newError();

				/**
				 * Creates a new log message for critical errors. This will create a header consisting of the current date, time and log type.
				 * The returned output stream can be used to write the messages content. The message must be ended via endMsg().
				 * @see endMsg()
				 * @return an output stream reference to which the message can be written
				 */
				std::ostream& newCriticalError();

				/**
				 * Ends the currently stream message.
				 */
				void endMsg();

			private:

				/**
				 * Streams the current time stamp in the format 'day.month.year hour:minute:second'.
				 * @param ost the stream to which the current time stamp will be printed
				 */
				void streamCurrentTime(std::ostream& ost);

				/**
				 * Streams a header for a log message of the given type to the given stream. This includes a time stamp
				 * and an abbreviation of the messages log type.
				 * @param ost the stream to which the header will be printed
				 * @param logType the type of the log message for which the header will be generated
				 */
				std::ostream& streamHeader(std::ostream& ost, LogTypes logType);

				/**
				 * This output stream discards all information sent to it.
				 */
				std::ostream devnull;

				/**
				 * Stores the output stream given this MessageLogger while instantiation. Will be used for streaming the log messages.
				 */
				std::ostream& mainstream;

				/**
				 * Stores the log level of the message currently streamed.
				 */
				LogTypes currLevel;

				/**
				 * Stores whether there is a message currently streamed.
				 */
				bool messageStreaming;

		};

	} /* namespace logging */
} /* namespace uncover */

#endif /* MESSAGELOGGER_H_ */
