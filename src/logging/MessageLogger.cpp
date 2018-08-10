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

#include "MessageLogger.h"
#include <time.h>
#include <iomanip>

using namespace std;

namespace uncover {
	namespace logging {

		MessageLogger::MessageLogger(std::ostream& ostr) : devnull(nullptr),
				mainstream(ostr),
				currLevel(LogTypes::LT_None),
				messageStreaming(false) {}

		MessageLogger::~MessageLogger() {}

		ostream& MessageLogger::newMsg(LogTypes logType) {
			if(messageStreaming) {
				endMsg();
			}
			currLevel = logType;
			messageStreaming = true;
			if(currLevel <= gl_LogLevel) {
				streamHeader(mainstream,currLevel);
				return mainstream;
			} else {
				return devnull;
			}
		}

		ostream& MessageLogger::contMsg() {
			if(!messageStreaming) {
				newMsg(LogTypes::LT_None);
			}
			if(currLevel <= gl_LogLevel) {
				return mainstream;
			} else {
				return devnull;
			}
		}

		ostream& MessageLogger::newUserInfo() {
			return newMsg(LogTypes::LT_UserInfo);
		}

		ostream& MessageLogger::newDebug() {
			return newMsg(LogTypes::LT_Debug);
		}

		ostream& MessageLogger::newWarning() {
			return newMsg(LogTypes::LT_Warning);
		}

		ostream& MessageLogger::newError() {
			return newMsg(LogTypes::LT_Error);
		}

		ostream& MessageLogger::newCriticalError() {
			return newMsg(LogTypes::LT_CriticalError);
		}

		void MessageLogger::endMsg() {
			if(currLevel <= gl_LogLevel) {
				mainstream << endl;
			}
			currLevel = LogTypes::LT_None;
			messageStreaming = false;
		}

		void MessageLogger::streamCurrentTime(ostream& ost) {
			time_t rawtime;
			time(&rawtime);
			struct tm * timeinfo = localtime(&rawtime);
			ost << setfill('0') << setw(2) << timeinfo->tm_mday << "." << setw(2) << (timeinfo->tm_mon+1)
					<< "." << setw(4) << (timeinfo->tm_year+1900) << " " << setw(2) << timeinfo->tm_hour
					<< ":" << setw(2) << timeinfo->tm_min << ":" << setw(2) << timeinfo->tm_sec;
		}

		ostream& MessageLogger::streamHeader(ostream& ost, LogTypes logType) {
			ost << "[";
			streamCurrentTime(ost);
			ost << " " << gl_logTypeToString(logType) << "] ";
			return ost;
		}

	} /* namespace logging */
} /* namespace uncover */
