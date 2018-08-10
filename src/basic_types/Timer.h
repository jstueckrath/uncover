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

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>
#include "InvalidStateException.h"

namespace uncover {
	namespace basic_types {

		/**
		 * Provides a timer, which can be started and used to measure elapsed time. Additionally a timeout can be set and
		 * function is provided for checking if the timeout was reached.
		 * @author Jan Stückrath
		 */
		class Timer {

			public:

				/**
				 * Generates a new Timer with the given timeout. If no timeout is given, 0 is used (no timeout).
				 * @param timeout the timeout to be stored (a value of 0 deactivates timeout check)
				 */
				Timer(unsigned int timeout = 0);

				/**
				 * Destroys this instance of a Timer.
				 */
				virtual ~Timer();

				/**
				 * Returns the timeout stored in this Timer.
				 * @return the stored timeout
				 */
				unsigned int getTimeout() const;

				/**
				 * Sets the timeout for this Timer. A given value of 0 deactivates the timeout check.
				 * This function does not affect the timers running state.
				 * @param timeout the new timeout
				 */
				void setTimeout(unsigned int timeout);

				/**
				 * Returns true iff the timeout has a positive value.
				 * @return true iff the timeout is not 0
				 */
				bool isTimeoutSet() const;

				/**
				 * Returns the time when this Timer was started.
				 * @return the time when this Timer was started
				 * @exception TimerStateException if the Timer was never started
				 */
				time_t getStartTime() const;

				/**
				 * Returns the time elapsed since the Timer was started (in seconds).
				 * @return the time elapsed since the Timer was started (in seconds)
				 * @exception TimerStateException if the Timer was never started
				 */
				double getElapsedTime() const;

				/**
				 * Starts the timer. This enables checking for elapsed time, etc. Calling this method multiple
				 * times will reset the Timers starting time.
				 */
				void start();

				/**
				 * Returns true, iff the elapsed time is larger than the set timeout.
				 * @return true, iff the elapsed time is larger than the set timeout
				 * @exception TimerStateException if the Timer was never started
				 */
				bool outOfTime() const;

				/**
				 * Returns true iff the Timers start-method was called.
				 * @return true iff the Timers start-method was called
				 */
				bool isRunning() const;

			private:

				/**
				 * Stores the timeout for "out of time"-checks.
				 */
				unsigned int timeout;

				/**
				 * Stores the starting time of this Timer.
				 */
				time_t startTime;

				/**
				 * Stores whether this timer was started at least once.
				 */
				bool running;
		};

		/**
		 * Alias for a shared pointer to a Timer.
		 */
		typedef shared_ptr<Timer> Timer_sp;

	}
}
#endif /* TIMER_H_ */
