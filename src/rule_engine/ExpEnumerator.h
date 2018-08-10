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

#ifndef EXPENUMERATOR_H_
#define EXPENUMERATOR_H_

#include <cstddef>

namespace uncover {
	namespace rule_engine {

		/**
		 * This class represents a number over a variable base with a fixed length. It is initialized with a
		 * base and an exponent, such that its values can range from 0 to base^exponent - 1. Methods to
		 * increment the number and check if increment is possible, are also provided.
		 * @author Jan Stückrath
		 */
		class ExpEnumerator {

			public:

				/**
				 * Generates an new ExpEnumerator with the given base and exponent. No further initialization is
				 * necessary.
				 * @param base the base of the internal number
				 * @param exponent the maximal length of the internal number
				 */
				ExpEnumerator(std::size_t base = 0, std::size_t exponent = 0);

				/**
				 * Destroys the current instance of ExpEnumerator.
				 */
				virtual ~ExpEnumerator();

				/**
				 * Sets the internal number to 0 (the minimal value).
				 */
				void initialize();

				/**
				 * Returns true, iff the internal number has not reached its maximal value.
				 * @return true, iff the internal number is not equal to base^exponent - 1
				 */
				bool canCountUp();

				/**
				 * Returns the maximal length of the internal number.
				 * @return the maximal length of the internal number
				 */
				std::size_t size();

				/**
				 * Increments the internal number by one. If the internal number has reached its maximal value,
				 * it is set to 0 instead.
				 */
				void operator++();

				/**
				 * Returns the value of the number at the given position. Least significant bit has the index 0.
				 * @return the value of the number at the given position
				 */
				std::size_t& operator[](std::size_t& index);

			private:

				/**
				 * The length of the array storing the internal number.
				 */
				std::size_t dataSize;

				/**
				 * An array storing the internal number.
				 */
				std::size_t* data;

				/**
				 * The base of the internal number.
				 */
				std::size_t base;

		};

	} /* namespace rule_engine */
} /* namespace uncover */

#endif /* EXPENUMERATOR_H_ */
