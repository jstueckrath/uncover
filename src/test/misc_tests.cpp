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

#ifdef COMPILE_UNIT_TESTS

#include <boost/test/unit_test.hpp>
#include "../logging/message_logging.h"

using namespace uncover::logging;

namespace uncover {
	namespace test {

		BOOST_AUTO_TEST_SUITE(misc)

		BOOST_AUTO_TEST_CASE(logging_test)
		{
			LogTypes store = gl_LogLevel;
			for(int i = -1; i <= LogTypes::LT_Debug; ++i) {
				gl_LogLevel = static_cast<LogTypes>(i);
				BOOST_TEST_MESSAGE("Now testing gl_LogLevel " << gl_logTypeToString(gl_LogLevel));
				logger.newUserInfo() << "This is a user info." << endLogMessage;
				logger.newCriticalError() << "This is a critical error." << endLogMessage;
				logger.newError() << "This is an error." << endLogMessage;
				logger.newWarning() << "This is a warning." << endLogMessage;
				logger.newDebug() << "This is a debug message." << endLogMessage;
			}

			logger.contMsg() << "This is a broken message.";
			logger.newUserInfo() << "This is another one." << endLogMessage;

			gl_LogLevel = store;
		}

		BOOST_AUTO_TEST_SUITE_END()

	}
}

#endif
