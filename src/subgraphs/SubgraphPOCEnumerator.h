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

#ifndef SUBGRAPHPOCENUMERATOR_H_
#define SUBGRAPHPOCENUMERATOR_H_

#include "../analysis/MinPOCEnumerator.h"
#include "../rule_engine/PartialPOCEnumerator.h"

namespace uncover {
	namespace subgraphs {

		/**
		 * This enumerator can be used to enumerate all pushout complements of a (partial) rule and a graph.
		 * Only the pushout complements minimal wrt the subgraph ordering are enumerated. Depending on the
		 * initialization parameters, the matches (to the pushout complement) are either guaranteed to be
		 * injective or conflict-free. Furthermore, a path bound can be given, which will cause every pushout
		 * complement exceeding this bound to be dropped.
		 * @author Jan Stückrath
		 * @see SubgraphOrder
		 * @see analysis::MinPOCEnumerator
		 */
		class SubgraphPOCEnumerator : public analysis::MinPOCEnumerator {

			public:

				/**
				 * Creates a new enumerator with the given parameters. If the first parameter is true, only injective
				 * matches will be used, otherwise matches may be conflict-free. The second parameter sets a path bound,
				 * causing all pushout complements to be dropped containing a longer path. A negative path bound
				 * disables this check.
				 * @param inj use true for injective and false for conflict-free matches
				 * @param bound sets the longest path length allowed in a pushout complement; a negative number disables check
				 */
				SubgraphPOCEnumerator(bool inj, int bound);

				/**
				 * Move constructor. All data of the old enumerator will be moved to the new one, invalidating the old
				 * enumerator. Use the old enumerator after this call (apart from deletion) will throw exceptions.
				 * @param oldEnum the old enumerator of which the data will be moved
				 */
				SubgraphPOCEnumerator(SubgraphPOCEnumerator&& oldEnum);

				/**
				 * Destroys this enumerator.
				 */
				virtual ~SubgraphPOCEnumerator();

				virtual bool ended() const override;

				virtual SubgraphPOCEnumerator& operator++() override;

				virtual graphs::Hypergraph_sp operator*() const override;

				virtual bool resetWith(graphs::AnonRule_csp rule, IDType ruleID, graphs::Hypergraph_csp graph) override;

				using analysis::MinPOCEnumerator::resetWith;

			private:

				/**
				 * Checks if this enumerator is valid, i.e. its data was not moved away, and throws an exception if it
				 * is not.
				 */
				void checkAndThrow() const;

				/**
				 * Checks if the last graph computed by the stored PartialPOCEnumerator exceeds the path bound. If this
				 * check fails, or if this check is disabled, false is returned.
				 * @return true, iff the path bound is positive and the current graph exceeds it.
				 */
				bool exceedsBound() const;

				/**
				 * Stores the path bound used by this enumerator. May be negative.
				 */
				int pathbound;

				/**
				 * Stores whether injective (true) or conflict-free (false) matches should be used.
				 */
				bool injective;

				/**
				 * Stores a PartialPOCEnumerator used for the pushout complement computation.
				 */
				rule_engine::PartialPOCEnumerator* partialEnumerator;

				/**
				 * Stores the ID of the rule applied backwards, for logging.
				 */
				IDType ruleID;

				/**
				 * Stores the ID of the graph, for logging.
				 */
				IDType graphID;

		};

	} /* namespace subgraphs */
} /* namespace uncover */

#endif /* SUBGRAPHPOCENUMERATOR_H_ */
