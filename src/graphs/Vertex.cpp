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

#include "Vertex.h"

using namespace std;

namespace uncover {
	namespace graphs {

		Vertex::Vertex(IDType id) : id(id) {}

	  Vertex::~Vertex() {}

	 	IDType Vertex::getID() const
	 	{
	 		return id;
	 	}

		std::ostream& operator<< (std::ostream& ost, Vertex const& data)
		{
			ost << "v:" << (data.id);
			return ost;
		}

	}
}
