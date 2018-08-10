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

#ifndef VECTORCALLBACK_H_
#define VECTORCALLBACK_H_

#include "CollectionCallback.h"

namespace uncover {
	namespace basic_types {

		/**
		 * This template class implements a CollectionCallback to a vector. It is initialized by a
		 * reference to a vector, to which any object added to the callback object will be relayed.
		 * @author Jan Stückrath
		 */
		template<typename E>
		class VectorCallback : public CollectionCallback<E> {

			public:

				/**
				 * Generates a new callback with the given vector reference.
				 * @param vec a reference to a vector
				 */
				VectorCallback(vector<E>& vec) : vec(vec) {}

				/**
				 * Destroys this callback object. This does not affect the referenced vector!
				 */
				virtual ~VectorCallback() {}

				/**
				 * Adds an object to the vector encapsulated by the callback object.
				 */
				virtual void insert(E element) override { vec.push_back(element); }

			private:

				/**
				 * Stores a reference to a vector.
				 */
				vector<E>& vec;
		};

	}
}

#endif /* VECTORCALLBACK_H_ */
