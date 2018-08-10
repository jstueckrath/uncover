/***************************************************************************
 *   Copyright (C) 2005 by SZS                                             *
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

#include "Mapping.h"
#include <iostream>

using namespace uncover::basic_types;

namespace uncover {
	namespace graphs {

		Mapping::Mapping() : match() {}

		Mapping::~Mapping() {}

		unordered_set<IDType> Mapping::getInverseMatch(IDType const& tar) const {
			unordered_set<IDType> targetIDs;
			for(auto it = match.cbegin(); it != match.cend(); ++it) {
				if(tar == it->second) {
					targetIDs.insert(it->first);
				}
			}
			return targetIDs;
		}

		Mapping::Inversion Mapping::getInverseMatch() const {
			Mapping::Inversion resultmap;
			getInverseMatch(resultmap);
			return resultmap;
		}

		void Mapping::getInverseMatch(Mapping::Inversion& invmatch) const {
			invmatch.clear();
			for(Mapping::const_iterator it = match.cbegin(); it != match.cend(); ++it) {
				invmatch[it->second].insert(it->first);
			}
		}

		IDType const& Mapping::getTar(IDType const& source) const {
			return match.at(source);
		}

		bool Mapping::delSrc(IDType const& source)
		{
			return match.erase(source) > 0;
		}

		size_t Mapping::delSrc(unordered_set<IDType> const& sources) {
			size_t res = 0;
			for(unordered_set<IDType>::const_iterator iter = sources.cbegin(); iter != sources.cend(); ++iter) {
				res += this->match.erase(*iter);
			}
			return res;
		}

		size_t Mapping::delTar(IDType const& target)
		{
			vector<IDType> toDelete;
			for (auto it = match.cbegin(); it != match.cend(); ++it)
			{
				if (it->second == target)
				{
					toDelete.push_back(it->first);
				}
			}

			for (vector<IDType>::iterator	it = toDelete.begin(); it != toDelete.end(); ++it)
			{
				delSrc(*it);
			}

			return toDelete.size();
		}

		size_t Mapping::delTar(unordered_set<IDType> const& targets)
		{
			vector<IDType> toDelete;
			for (auto it = match.cbegin(); it != match.cend(); ++it)
			{
				if(targets.count(it->second))
				{
					toDelete.push_back(it->first);
				}
			}

			for (vector<IDType>::iterator	it = toDelete.begin(); it != toDelete.end(); ++it)
			{
				delSrc(*it);
			}

			return toDelete.size();
		}

		bool Mapping::hasSrc(IDType const& source) const
		{
			return (this->match.count(source) > 0);
		}

		bool Mapping::hasTar(IDType const& target) const {
			for(auto iter = match.cbegin(); iter != match.cend(); ++iter) {
				if(iter->second == target) {
					return true;
				}
			}
			return false;
		}

		void Mapping::concat(Mapping const& map) {

			vector<IDType> toDelete;

			for(Mapping::const_iterator iter = match.begin(); iter != match.end(); ++iter) {

				if(map.hasSrc(iter->second)) {
					match[iter->first] = map.getTar(iter->second);
				} else {
					toDelete.push_back(iter->first);
				}

			}

			for(vector<IDType>::iterator iter = toDelete.begin(); iter != toDelete.end(); ++iter) {
				match.erase(*iter);
			}

		}

		Mapping::iterator Mapping::begin() noexcept {
			return match.begin();
		}

		Mapping::const_iterator Mapping::cbegin() const noexcept {
			return match.cbegin();
		}

		Mapping::const_iterator Mapping::begin() const noexcept {
			return match.cbegin();
		}

		Mapping::iterator Mapping::end() noexcept {
			return match.end();
		}

		Mapping::const_iterator Mapping::end() const noexcept {
			return match.cend();
		}

		Mapping::const_iterator Mapping::cend() const noexcept {
			return match.cend();
		}

		IDType& Mapping::operator[](IDType const& src) {
			return match[src];
		}

		bool Mapping::isInjective() const {

			unordered_set<IDType> idsFound;
			for(Mapping::const_iterator it = match.begin(); it != match.end(); ++it) {
				if(idsFound.count(it->second)) {
					return false;
				} else {
					idsFound.insert(it->second);
				}
			}
			return true;
		}

		void Mapping::clear() {
			match.clear();
		}

		bool operator== ( const Mapping & m1, const Mapping & m2 )
		{
			if(m1.match.size() != m2.match.size())
				return false;

			for(auto it = m1.match.cbegin(); it != m1.match.cend(); it++)
			{
				Mapping::const_iterator it2 = m2.match.find(it->first);
				if(it2 == m2.match.end() || it->second != it2->second)
					return false;
			}

			return true;
		}

		std::ostream& operator<<(std::ostream& ost, Mapping const& map)
		{
			ost << "Mapping{";
			for (Mapping::const_iterator matchit = map.match.begin(); matchit != map.match.end(); ++matchit) {
				ost << '(' << matchit->first << ',' << matchit->second << ')';
			}
			ost << "}";
			return ost;
		}

	}
}
