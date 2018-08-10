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

#include "DelayedLogMessage.h"

namespace uncover {
	namespace logging {

		DelayedLogMessage::DelayedLogMessage(LogTypes logType, string message) : logType(logType), message(message) {}

		DelayedLogMessage::~DelayedLogMessage() {

			switch (logType) {
				case LogTypes::LT_UserInfo:
					logger.newUserInfo() << message << endLogMessage;
					return;
				case LogTypes::LT_Debug:
					logger.newDebug() << message << endLogMessage;
					return;
				case LogTypes::LT_Warning:
					logger.newWarning() << message << endLogMessage;
					return;
				case LogTypes::LT_Error:
					logger.newError() << message << endLogMessage;
					return;
				case LogTypes::LT_CriticalError:
					logger.newCriticalError() << message << endLogMessage;
					return;
				default:
					logger.newMsg(LogTypes::LT_None) << message << endLogMessage;
					return;
			}

		}

	} /* namespace logging */
} /* namespace uncover */
