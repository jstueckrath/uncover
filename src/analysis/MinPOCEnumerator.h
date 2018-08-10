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

#ifndef MINPOCENUMERATOR_H_
#define MINPOCENUMERATOR_H_

#include "../graphs/AnonRule.h"
#include "../graphs/Hypergraph.h"
#include "BackwardResultHandler.h"
#include "PreparedGTS.h"

namespace uncover {
	namespace analysis {

		/**
		 * This abstract class serves as a schema of an enumerator taking a rule and a graph and enumerating
		 * all minimal pushout complements wrt. some order. Since we need to know the original rule from which a
		 * rule was prepared, a reference to the prepared GTS is stored to provide this information. This class
		 * has to be concretized for each order to be compatible with the backward search.
		 * @author Jan Stückrath
		 */
		class MinPOCEnumerator {

			public:

				/**
				 * Destroys this instance of the enumerator.
				 */
				virtual ~MinPOCEnumerator() {}

				/**
				 * Returns true, iff the last element was already enumerated, i.e. no new graph could be computed,
				 * when operator++() was called the last time. If this function returns true, a call of operator*()
				 * will return null.
				 * @return true, iff the last element was already enumerated
				 */
				virtual bool ended() const = 0;

				/**
				 * Computes the next minimal pushout complement if any.
				 * @return a reference to this enumerator
				 */
				virtual MinPOCEnumerator& operator++() = 0;

				/**
				 * Returns the last computed minimal pushout complement. If this enumerator has ended, this function
				 * will return null.
				 * @return the last computed minimal POC, or null if enumerator has ended
				 */
				virtual graphs::Hypergraph_sp operator*() const = 0;

				/**
				 * Initializes this enumerator to compute all minimal pushout complements for the given rule and graph.
				 * When this function is called, any previous computation by this enumerator will be discarded.
				 * @param rule the rule used for the pushout complement computation
				 * @param ruleID the rule ID which will be displayed in the logs
				 * @param graph the graph (pushout object) used for the POC computation
				 * @return true iff the reset operation was successful
				 */
				virtual bool resetWith(graphs::AnonRule_csp rule, IDType ruleID, graphs::Hypergraph_csp graph) = 0;

				/**
				 * Initializes this enumerator to compute all minimal pushout complements for the given rule and graph.
				 * When this function is called, any previous computation by this enumerator will be discarded.
				 * @param rule the rule used for pushout complement computation
				 * @param graph the graph used for pushout complement computation
				 * @return true iff the reset operation was successful
				 */
				bool resetWith(graphs::Rule_csp rule, graphs::Hypergraph_csp graph) {
					if(rule != nullptr && graph != nullptr) {
						return resetWith(rule,rule->getID(),graph);
					} else {
						return false;
					}
				}

				/**
				 * Set the BackwardResultHandler used by this enumerator to the given one.
				 * @param resHandler the BackwardResultHandler to be used
				 */
				void setResultHandler(BackwardResultHandler_sp resHandler) { resultHandler = resHandler; }

				/**
				 * Set the PreparedGTS used by this enumerator to the given one.
				 * @param gts the new PreparedGTS to be used
				 */
				void setPreparedGTS(PreparedGTS_csp gts) { prepGTS = gts; }

			protected:

				/**
				 * Initializes this enumerator.
				 */
				MinPOCEnumerator() : resultHandler(nullptr), prepGTS(nullptr) {}

				/**
				 * Stores the BackwardResultHandler used by this enumerator.
				 */
				BackwardResultHandler_sp resultHandler;

				/**
				 * Stores a pointer to the PrepareGTS from which the rules will be taken, which are used to initialize
				 * this enumerator.
				 */
				PreparedGTS_csp prepGTS;

		};

		/**
		 * Alias of a shared pointer to an MinPOCEnumerator.
		 */
		typedef shared_ptr<MinPOCEnumerator> MinPOCEnumerator_sp;

	}
}

#endif /* MINPOCENUMERATOR_H_ */
