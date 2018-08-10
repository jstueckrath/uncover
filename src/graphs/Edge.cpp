/***************************************************************************
 *   Copyright (C) 2005 SZS                                                *
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

#include "Edge.h"
#include "../basic_types/standard_operators.h"

namespace uncover {
	namespace graphs {

		Edge::Edge(IDType id, vector<IDType> const& verts, string label) : ID(id), label(label), vertices(verts) {}

		Edge::~Edge() {}

		IDType Edge::getID() const
		{
			return ID;
		}

		void Edge::setLabel(string s)
		{
			label = s;
		}

		string Edge::getLabel() const
		{
			return label;
		}

		size_t Edge::getArity() const {
			return vertices.size();
		}

		Edge::VList& Edge::getVertices() {
			return vertices;
		}

		Edge::VList const& Edge::getVertices() const {
			return vertices;
		}

		Edge::VList::iterator Edge::beginV() {
			return vertices.begin();
		}

		Edge::VList::const_iterator Edge::beginV() const {
			return vertices.cbegin();
		}

		Edge::VList::const_iterator Edge::cbeginV() const {
			return vertices.cbegin();
		}

		Edge::VList::iterator Edge::endV() {
			return vertices.end();
		}

		Edge::VList::const_iterator Edge::endV() const {
			return vertices.cend();
		}

		Edge::VList::const_iterator Edge::cendV() const {
			return vertices.cend();
		}

		std::ostream& operator<< (std::ostream& ost, Edge const& data)
		{
			ost << "e:" << data.ID << "(" << data.label << ")" << data.vertices;
			return ost;
		}

	}
}
