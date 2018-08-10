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

#ifndef MESSAGE_LOGGING_H_
#define MESSAGE_LOGGING_H_

#include <ostream>
#include "MessageLogger.h"

namespace uncover {
	namespace logging {

		/**
		 * The main logger for all log messages. Has to be initialized by the main program.
		 */
		extern MessageLogger logger;

		/**
		 * Causes the current MessageLogger instance to end the current Message. If this function is streamed (or simply
		 * called), the current MessageLogger instance will be informed that the current log message ended.
		 * @param ost an output stream
		 * @return the given output stream
		 */
		std::ostream& endLogMessage(std::ostream& ost);

	}
}

#endif /* MESSAGE_LOGGING_H_ */
