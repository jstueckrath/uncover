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

#ifndef COLLECTIONCALLBACK_H_
#define COLLECTIONCALLBACK_H_

namespace uncover {
	namespace basic_types {

		/**
		 * This template provides a possibility to add elements to a collection without having a reference to
		 * the collection of knowing its type. It can be used by functions and objects which simply fill up
		 * a collection. An implementation of this template has to specify how the insertion of new elements
		 * should be performed.
		 * @author Jan Stückrath
		 */
		template<typename E>
		class CollectionCallback {

			public:

				/**
				 * Destroys the current instance of this object.
				 */
				virtual ~CollectionCallback() {}

				/**
				 * Adds an element to the collection hidden by this callback object. Exact handling of the element
				 * depends on the derived class.
				 * @param element the element which should be added.
				 */
				virtual void insert(E element) = 0;

			protected:

				/**
				 * Creates a new instance of this object. Can only be called by subclasses.
				 */
				CollectionCallback() {}

		};

	} /* namespace analysis */
} /* namespace uncover */

#endif /* COLLECTIONCALLBACK_H_ */
