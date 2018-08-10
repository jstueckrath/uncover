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

#ifndef PARTIALPOCENUMERATOR_H_
#define PARTIALPOCENUMERATOR_H_

#include "../graphs/AnonRule.h"
#include "TotalPOCEnumerator.h"

namespace uncover {
	namespace rule_engine {

		/**
		 * The PartialPOCEnumerator computes a subset of all pushout complement of a partial rule and a total match.
		 * This is done by first splitting the rule into a total and a partial part. First the pushout complement
		 * for the total part is computed by the TotalPOCEnumerator. The result is then modified to be the pushout
		 * complement of the original rule. These modifications only add elements to the pushout complement which
		 * have a preimage in the left rule side. Note that this enumerator does only compute a finite subset of all
		 * pushout complement, but not all, since there may be infinitely many. The computed set can be used to
		 * calculate a representative set wrt. some criteria.
		 * @see TotalPOCEnumerator
		 * @author Jan Stückrath
		 */
		class PartialPOCEnumerator {

			public:

				/**
				 * Generates a new PartialPOCEnumerator with the given rule and graph. The rule may be any (partial)
				 * morphism. The matches can be set to either injective or conflict-free matches. A
				 * collection of original rules, i.e. rules from which the current rule could have been generated,
				 * can be given if inj=false. Computed pushout complements are then only kept if the match is
				 * conflict-free wrt. at least one of these rules.
				 * @param rule the rule which will be applied backwards
				 * @param graph the graph to which the rule will be applied backwards
				 * @param inj set to true iff only injective matching should be used
				 * @param orRules a collection of all original rules; this parameter is ignored if inj=true, it is null
				 *        or it is empty
				 */
				PartialPOCEnumerator(graphs::AnonRule_csp rule,
						graphs::Hypergraph_csp graph,
						bool inj,
						shared_ptr<vector<graphs::AnonRule_csp>> orRules);

				/**
				 * Move constructor. Creates a new PartialPOCEnumerator using the data of the given one. The given
				 * enumerator will be invalidated in the process and must be reset before being usable again.
				 * @param oldEnum the enumerator from which the data will be copied
				 */
				PartialPOCEnumerator(PartialPOCEnumerator&& oldEnum);

				/**
				 * Destroys this PartialPOCEnumerator.
				 */
				virtual ~PartialPOCEnumerator();

				/**
				 * Returns true, if the enumerator has ended. This is the case if the enumerator was incremented,
				 * but unable to compute any further elements to enumerate.
				 * @return true, iff there are no more elements to enumerate
				 */
				bool ended() const;

				/**
				 * Reinitializes the enumerator with the given rule and graph. Any previous computations performed
				 * by this enumerator are discarded and the first element for its new content is enumerated. A
				 * collection of original rules, i.e. rules from which the current rule could have been generated,
				 * can be given if inj=false. Computed pushout complements are then only kept if the match is
				 * conflict-free wrt. at least one of these rules.
				 * @param rule the rule which will be applied backwards
				 * @param graph the graph to which the rule will be applied backwards
				 * @param inj set to true iff only injective matching should be used
				 * @param orRules a collection of all original rules; this parameter is ignored if inj=true, it is
				 *        null or it is empty
				 */
				void reset(graphs::AnonRule_csp rule,
						graphs::Hypergraph_csp graph,
						bool inj,
						shared_ptr<vector<graphs::AnonRule_csp>> orRules);

				/**
				 * Computes the next element in the enumeration. If the last element was already computed, this
				 * call is ignored.
				 * @return a reference to this enumerator
				 */
				PartialPOCEnumerator& operator++();

				/**
				 * Returns the co-match currently used for the backwards application of the rule.
				 * @return the co-match currently used for the backwards application of the rule
				 */
				graphs::Mapping const& getCurrentCoMatch() const;

				/**
				 * Returns the last computed match from the left hand-side to the (last computed) pushout
				 * complement. If the enumerator has ended, null is returned.
				 * @return the last computed mapping or null if the enumerator has ended
				 */
				graphs::Mapping_sp getMapping() const;

				/**
				 * Returns the last computed pushout complement. If the enumerator has ended, null is returned.
				 * @return the last computed pushout complement or null if the enumerator has ended
				 */
				graphs::Hypergraph_sp getGraph() const;

			private:

				/**
				 * Uses the current graph and mapping returned by the TotalPOCEnumerator and initializes the template
				 * fields with them. These include templateGraph, templateMapp and additionally vertexMergings,
				 * mergableEdges, edgeMergings if matches may be non-injective.
				 */
				void genTemplates();

				/**
				 * Generates the current graph and mapping from the template graph and mapping. If matches may be
				 * non-injective, vertex and edge mergings are also performed according to the current values of
				 * vertexMergings and edgeMergings.
				 */
				void computeCurrent();

				/**
				 * Sets the current graph and mapping to a copy of the template graph and mapping. Then merges
				 * the vertices according to the current content of vertexMergings.
				 */
				void mergeNodes();

				/**
				 * Calls mergeNodes() to merge vertices and then initializes edgeMergings to enumerate all
				 * possible mergings of parallel edges.
				 */
				void prepEdgeMergings();

				/**
				 * Merges the edges of the current graph (adjusting also the mapping) according to the content
				 * of edgeMergings.
				 */
				void mergeEdges();

				/**
				 * Increments the edgeMergings vector to return the next merging on parallel edges. Returns
				 * false if the increment was not possible, i.e. all possibilities where enumerated.
				 * @retun false if all edge mergings where enumerated
				 */
				bool incEdgeMergings();

				/**
				 * Implements the core functionality of the increment operator in the following way:
				 * <ol>
				 * <li>If conflict-free matches are used, compute the next partial pushout complement from
				 * the current total pushout complement.</li>
				 * <li>If this is not possible or injective matches are used, compute the next total pushout
				 * complement. Construct the first partial pushout complement and return it (there is only
				 * one in the injective case).</li>
				 * <li>If there are no more total pushout complements, this enumerator ends</li>
				 * </ol>
				 * @return true, iff a pushout complement could computed, i.e. the enumerator did not end
				 */
				bool getNext();

				/**
				 * Checks whether there exists an original rule (from which the current rule was generated)
				 * such that the current match is conflict-free wrt. this rule.
				 * @return true, iff at least one such original rule exists
				 */
				inline bool cfWrtOriginalRule() const;

				/**
				 * Stores the partial part of the rule. This morphism is injective, surjective, but partial.
				 */
				graphs::AnonRule_csp partialRule;

				/**
				 * Stores the collection of original rules. (only used for non-injective matches)
				 */
				shared_ptr<vector<graphs::AnonRule_csp>> originalRules;

				/**
				 * Stores whether injective or conflict-free matches should be used.
				 */
				bool injective;

				/**
				 * Stores a TotalPOCEnumerator for computing pushout complement for the total part
				 * of the rule.
				 */
				TotalPOCEnumerator* totalPOCs;

				/**
				 * Stores the last computed (partial) pushout complement.
				 */
				graphs::Hypergraph_sp currentGraph;

				/**
				 * Stores the last computed mapping from the left hand-side to the (partial) pushout
				 * complement.
				 */
				graphs::Mapping_sp currentMapp;

				////////////////////////////////////////////////////////////////////////////////////////
				// Fields used as temporary storage and basis for creating the current graph and mapping
				////////////////////////////////////////////////////////////////////////////////////////

				/**
				 * Will contain the total poc with added nodes and edges.
				 */
				graphs::Hypergraph_sp templateGraph;

				/**
				 * Will contain the mapping from the left side to the template graph.
				 */
				graphs::Mapping_sp templateMapp;

				/**
				 * Stores an IDPartitionEnumerator to enumerate all possible node mergings.
				 */
				basic_types::IDPartitionEnumerator* vertexMergings;

				/**
				 * Stores the subset of edges which can (potentially) be merged.
				 */
				vector<IDType>* mergableEdges;

				/**
				 * Stores a sequence of IDPartitionEnumerators for enumerating all possible mergings
				 * of parallel edges.
				 */
				vector<basic_types::IDPartitionEnumerator>* edgeMergings;

				/**
				 * Stores whether the nodes need to be merged or not when computing the current graph.
				 */
				bool nodesAlreadyMerged;

		};

	} /* namespace rule_engine */
} /* namespace uncover */

#endif /* PNIPOCENUMERATOR_H_ */
