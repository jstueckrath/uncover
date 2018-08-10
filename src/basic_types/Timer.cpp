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

#include "Timer.h"
//#include <sys/time.h>

namespace uncover {
	namespace basic_types {

		Timer::Timer(unsigned int timeout) : timeout(timeout), startTime(), running(false) {}

		Timer::~Timer() {}

		unsigned int Timer::getTimeout() const {
			return timeout;
		}

		void Timer::setTimeout(unsigned int timeout) {
			this->timeout = timeout;
		}

		bool Timer::isTimeoutSet() const {
			return timeout != 0;
		}

		time_t Timer::getStartTime() const {
			if(running) {
				return startTime;
			} else {
				throw InvalidStateException("The timer cannot return start time, since timer is not running.");
			}
		}

		double Timer::getElapsedTime() const {
			if(running) {
				time_t curTime;
				time(&curTime);
				return difftime(curTime,startTime);
			} else {
				throw InvalidStateException("The timer cannot check elapsed time since timer is not running.");
			}
		}

		void Timer::start() {
			time(&startTime);
			running = true;
		}

		bool Timer::outOfTime() const {
			if(timeout != 0) {
				return getElapsedTime() > timeout;
			}
			return false;
		}

		bool Timer::isRunning() const {
			return running;
		}

	} /* namespace analysis */
} /* namespace uncover */
