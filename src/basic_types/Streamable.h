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

#ifndef STREAMABLE_H_
#define STREAMABLE_H_

namespace uncover {
	namespace basic_types {

		/**
		 * Streamable provides a streaming function which must be implemented by any deriving class. It also defines
		 * the streaming operator, such that every class derived from Streamable can be streamed by any ostream.
		 * @author Jan Stückrath
		 */
		class Streamable {

			public:

				/**
				 * Streams as string representation of this Streamable object to the given ostream.
				 * @param ost the stream to which the string representation will be written
				 */
				virtual void streamTo(std::ostream& ost) const = 0;

			protected:

				/**
				 * Creates a new Streamable object. This constructor renders this class uninstantiable except
				 * from subclasses.
				 */
				Streamable() {}

				/**
				 * Destroys this Streamable object.
				 */
				virtual ~Streamable() {}

				/**
				 * Prints a string representation of this Streamable object to the given stream.
				 * @param ost the stream to which the Streamable object will be printed
				 * @param data the Streamable object to be streamed
				 * @return a reference to the given stream
				 */
				friend std::ostream& operator<< (std::ostream& ost, uncover::basic_types::Streamable const& data) {
					data.streamTo(ost);
					return ost;
				}

		};

	}
}

#endif /* STREAMABLE_H_ */
