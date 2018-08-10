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

#include "MinGraphList.h"
#include <iostream>
#include "../logging/message_logging.h"

using namespace uncover::basic_types;
using namespace uncover::graphs;
using namespace uncover::analysis;
using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace graphs {

		MinGraphList::MinGraphList(Order_sp order,
				BackwardResultHandler_sp resHandler)
			:	order(order),
			 	graphs(make_shared<vector<Hypergraph_sp>>()),
			 	resHandler(resHandler),
			 	idsInList() {}

		MinGraphList::~MinGraphList() {}

		shared_ptr<vector<Hypergraph_sp>> MinGraphList::getGraphs() {
			return graphs;
		}

		bool MinGraphList::addGraph(Hypergraph_sp graph, shared_ptr<vector<Hypergraph_sp>> deletedGraphs) {

			bool canBeLarger = true;
			vector<Hypergraph_sp>::iterator it = graphs->begin();

			while(it != graphs->end()) {

				// true if given graph is larger than some graph in the list
				if(canBeLarger && order->isLessOrEq(**it,*graph)) {

					if(resHandler) {
						resHandler->wasDeletedBecauseOf(graph->getID(),(*it)->getID());
					}

					return false;
				}

				// true if given graph is smaller than some graph in the list
				// if this case is true, the first case can never be true anymore!
				if(order->isLessOrEq(*graph,**it)) {

					if(deletedGraphs) {
						deletedGraphs->push_back(*it);
					}

					if(resHandler) {
						resHandler->wasDeletedBecauseOf((*it)->getID(),graph->getID());
					}

					idsInList.erase((*it)->getID());
					it = graphs->erase(it);
					canBeLarger = false;

				} else {
					++it;
				}

			}

			graphs->push_back(graph);
			idsInList.insert(graph->getID());
			return true;

		}

		size_t MinGraphList::addGraphs(vector<Hypergraph_sp> const& graphs) {

			// add all graph, minimizing immediately
			for(auto it = graphs.cbegin(); it != graphs.cend(); ++it) {
				this->addGraph(*it);
			}

			return this->graphs->size();
		}

		size_t MinGraphList::size() const {
			return graphs->size();
		}

		bool MinGraphList::contains(IDType graphID) const {
			return idsInList.count(graphID) > 0;
		}

	} /* namespace graphs */
} /* namespace uncover */
