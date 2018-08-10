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

#ifndef STANDARD_OPERATORS_H_
#define STANDARD_OPERATORS_H_

// WARNING this is not a nice fix, but has to be done because of problems with ADL lookup
namespace std {

	/**
	 * Defines the stream operator for unordered maps.
	 */
	template <typename K, typename V> ostream& operator<<(ostream& ost, unordered_map<K,V> const& mapping) {
		typename unordered_map<K,V>::const_iterator iter = mapping.cbegin();
		ost << "(";
		while(iter != mapping.cend()) {
			ost << "[" << iter->first << "," << iter->second << "]";
			++iter;
		}
		ost << ")";
		return ost;
	}

	/**
	 * Defines the stream operator for parameterized vectors.
	 */
	template <typename T> ostream& operator<<(ostream& ost, vector<T> const& vec) {
		typename vector<T>::const_iterator iter = vec.cbegin();
		ost << "(";
		if(iter != vec.cend()) {
			ost << *iter;
			++iter;
		}
		for (; iter != vec.cend(); iter++)
		{
			ost << "," << *iter;
		}
		ost << ")";
		return ost;
	}

	/**
	 * Defines the stream operator for parameterized vectors.
	 */
	template <typename T> ostream& operator<<(ostream& ost, unordered_set<T> const& vec) {
		typename unordered_set<T>::const_iterator iter = vec.cbegin();
		ost << "(";
		if(iter != vec.cend()) {
			ost << *iter;
			++iter;
		}
		for (; iter != vec.cend(); iter++)
		{
			ost << "," << *iter;
		}
		ost << ")";
		return ost;
	}

	/**
	 * Defines the stream operator for pairs.
	 */
	template <typename T1, typename T2> ostream& operator<<(ostream& ost, pair<T1,T2> const& pair) {
		ost << "(" << pair.first << "," << pair.second << ")";
		return ost;
	}

}

#endif /* STANDARD_OPERATORS_H_ */
