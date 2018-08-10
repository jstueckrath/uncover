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

#include "Hypergraph.h"
#include <string.h>
#include <sstream>
#include "../basic_types/standard_operators.h"
#include "../logging/message_logging.h"

using namespace uncover::basic_types;
using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace graphs {

		Hypergraph::Hypergraph(string name) :
				AnonHypergraph(name),
				id(gl_getNewStateID()) {}

		Hypergraph::Hypergraph(const AnonHypergraph& graph) :
				AnonHypergraph(graph),
				id(gl_getNewStateID()) {}

		Hypergraph::Hypergraph(const Hypergraph& graph) :
				AnonHypergraph(graph),
				id(gl_getNewStateID()) {}

		Hypergraph::~Hypergraph()	{}

		IDType Hypergraph::getID() const
		{
			return this->id;
		}

		void Hypergraph::streamTo(std::ostream& ost) const {
			ost << "Hypergraph[name=" << name << ", id=" << id << ", vertices=[";

			// stream out the nodes
			auto vit = vertices.cbegin();
			if(vit != vertices.cend()) {
				ost << vit->first;
				vit++;
			}
			for (; vit != vertices.cend(); vit++)
			{
				ost << "," << vit->first;
			}
			ost << "], edges=[";

			// stream out the edges
			auto eit = edges.cbegin();
			if(eit != edges.cend()) {
				ost << (eit->second);
				++eit;
			}
			for(;eit != edges.cend(); ++eit) {
				ost << ", " << (eit->second);
			}
			ost << "]]";
		}

	}
}
