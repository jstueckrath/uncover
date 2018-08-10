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

#ifndef HASH_FUNCTIONS_H_
#define HASH_FUNCTIONS_H_

// WARNING this is not a nice fix, but has to be done because of problems with ADL lookup
namespace std
{

	/**
	 * Defines the hash of an unordered_set as the xor of all elements of the set.
	 */
  template<typename T> struct hash<unordered_set<T>> {

		/**
		 * This operator serves as an implementation of its encapsulating struct.
		 */
    inline size_t operator()(const unordered_set<T> & s) const
    {
    	std::hash<T> hasher;
      size_t seed = 0;
      for(typename unordered_set<T>::const_iterator it = s.cbegin(); it != s.cend(); ++it) {
				seed ^= hasher(*it);
      }
      return (seed << s.size()) + (seed >> s.size());
    }

  };

}

#endif /* HASH_FUNCTIONS_H_ */
