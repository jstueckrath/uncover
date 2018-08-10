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

#ifndef TOTALPOCENUMERATOR_H_
#define TOTALPOCENUMERATOR_H_

#include "../graphs/AnonRule.h"
#include "../graphs/Hypergraph.h"
#include "StdMatchFinder.h"
#include "IDClosureEnumerator.h"
#include "IDtoIDEnumerator.h"

namespace uncover {
	namespace rule_engine {

		/**
		 * This enumerator computes all pushout complements for all co-matches from a total (given) rule to
		 * a given graph. The enumerator can be configured to compute only pushout complements, where the
		 * match (from L to the POC) is injective.
		 * @author Jan Stückrath
		 */
		class TotalPOCEnumerator {

			public:

				/**
				 * Initializes a new TotalPOCEnumerator with the given graph and rule as parameters. The given
				 * rule must be a total morphism. The matches can be set to injective or conflict-free.
				 * @param rule the rule which should be applied backwards
				 * @param graph the graph to which the co-matches will be computed
				 * @param injective set to true if only injective POCs are wanted
				 */
				TotalPOCEnumerator(
						graphs::AnonRule_csp rule,
						graphs::Hypergraph_csp graph,
						bool injective);

				/**
				 * Move constructor. Constructs a new TotalPOCEnumerator and moves the content of the given
				 * TotalPOCEnumerator to the new one. Thereafter the old enumerator is invalid and may cause
				 * exceptions and/or undefined behaviour if used.
				 * @param oldEnum the enumerator of which the contents will be moved
				 */
				TotalPOCEnumerator(TotalPOCEnumerator&& oldEnum);

				/**
				 * Destroys this instance of the TotalPOCEnumerator.
				 */
				virtual ~TotalPOCEnumerator();

				/**
				 * Resets this enumerator to its initial values. After this call will cause the enumeration
				 * process to start from the beginning.
				 */
				void reset();

				/**
				 * Returns true if there are no more POCs to compute. If this method returns true, call of
				 * other functions may throw exceptions.
				 * @return true if there are no more POCs to compute
				 */
				bool ended() const;

				/**
				 * Computes the next pushout complement. If the enumerator ended before calling this method,
				 * it behaves as a no-op.
				 * @return a reference to this enumerator
				 */
				TotalPOCEnumerator& operator++();

				/**
				 * Returns the last computed Mapping (from L to the POC).
				 * @return the last computed Mapping (from L to the POC)
				 */
				graphs::Mapping_sp getMapping() const;

				/**
				 * Returns the last computed pushout complement graph.
				 * @return the last computed pushout complement graph
				 */
				graphs::Hypergraph_sp getGraph() const;

				/**
				 * Returns a reference to the co-match currently used for the computation of POCs.
				 * @return a reference to the co-match currently used
				 */
				graphs::Mapping const& getCurrentCoMatch() const;

			private:

				/**
				 * Generates enumerators for the equivalences on edges and computes an initial,
				 * non-merged POC. Only used if the rule is non-injective and matchings may be
				 * non-injective. Initializes the fields: edgeEnums, templateGraphOr,
				 * templateMappOr and mappPOCtoGraph.
				 */
				void initEdgeTemplates();

				/**
				 * First performs merging of vertices and edges according to the current equivalence
				 * on the edges, and then generates the enumerators for possible equivalences on
				 * vertices. Only used if the rule is non-injective and matchings may be non-injective.
				 * Initializes the fields: vertexEnums, templateGraphME and templateMappME.
				 */
				void initVertexTemplates();

				/**
				 * Generates enumerators for attaching edges in all possible ways to split vertices in
				 * the POC and generates an initial non-merged POC. Only used if the rule is non-injective
				 * and matchings are set to be injective. Initializes the fields: vertexEnumsInj,
				 * templateGraphOr and templateMappOr.
				 */
				void initTemplatesInj();

				/**
				 * Generates an initial POC without any mergings. Only used if the rule is non-injective.
				 * The initial POC is generated in the following way:
				 * 1) Generate a copy of the left side L.
				 * 2) Add a vertex for each vertex of the matched graph G without a preimage in R.
				 * 3) Add an edge for each edge of G without a preimage in R. Attach this edge to new
				 * vertices (adding them), except the vertex was already added in the previous step.
				 */
				void generateNonMergedPOC();

				/**
				 * Computes the current pushout complement from the temporary values computed to far.
				 * Only used if the rule is non-injective and matchings may be non-injective.
				 */
				void computePOC();

				/**
				 * Computes the current pushout complement from the temporary values computed to far.
				 * Only used if the rule is non-injective and matchings are set to be injective.
				 */
				void computePOCInj();

				/**
				 * Computes the current pushout complement without use of any temporary computed
				 * values. Only used if the rule is injective.
				 */
				void computePOCInjRule();

				/**
				 * Searches for the next match where pushout complements exist. Discards all matches
				 * where no POCs exist.
				 * @return true iff a match was found, i.e. the enumerator did not end
				 */
				bool chooseNextMatch();

				/**
				 * Increments the given collection of enumerators such that the next POC can be
				 * generated from it. Only used if the rule is non-injective and matchings may be
				 * non-injective.
				 * @return true iff the increment was possible, i.e. not all possibilities where
				 * 				 enumerated yet
				 */
				bool incEnums(vector<IDClosureEnumerator>& enums);

				/**
				 * Increments the given collection of enumerators such that the next POC can be
				 * generated from it. Only used if the rule is non-injective and matchings are
				 * set to be injective.
				 * @return true iff the increment was possible, i.e. not all possibilities where
				 * 				 enumerated yet
				 */
				bool incEnums(vector<IDtoIDEnumerator>& enums);

				/**
				 * Deletes all temporary fields used in the POC computation (freeing their memory).
				 */
				void deleteTemporaryFields();

				/**
				 * The rule which should be applied backwards.
				 */
				graphs::AnonRule_csp rule;

				/**
				 * The graph to be matched to.
				 */
				graphs::Hypergraph_csp graph;

				/**
				 * Stores if the POCs should be injective.
				 */
				const bool injective;

				/**
				 * Stores if the rule is injective.
				 */
				const bool injectiveRule;

				/**
				 * Stores a matcher used to compute matchings from the right side to the given graph.
				 */
				StdMatchFinder* matcher;

				/**
				 * A list of all co-matches computed. The last element is used as the current match and
				 * used matches are popped.
				 */
				shared_ptr<vector<graphs::Mapping>> matchList;

				/**
				 * The last computed POC graph.
				 */
				graphs::Hypergraph_sp currGraph;

				/**
				 * The last computed match from the left side to the last computed POC.
				 */
				graphs::Mapping_sp currMapp;

				///////////////////////////////////////////////////////////////
				// fields used to enumerate all POCs of one applicable match //
				///////////////////////////////////////////////////////////////

				/**
				 * A list of enumerators for enumerating all possible equivalences on edges.
				 * Only used if rule is non-injective and matchings may be non-injective.
				 */
				vector<IDClosureEnumerator>* edgeEnums;

				/**
				 * A list of enumerators for enumerating all possible equivalences on vertices.
				 * Only used if rule is non-injective and matchings may be non-injective.
				 */
				vector<IDClosureEnumerator>* vertexEnums;

				/**
				 * A copy of an initially non-merged POC. Only used if rule is non-injective.
				 */
				graphs::Hypergraph* templateGraphOr;

				/**
				 * The mapping from L to templateGraphOr. Only used if rule is non-injective.
				 */
				graphs::Mapping* templateMappOr;

				/**
				 * A copy of the POC after performing the mergings of edges (including implied
				 * mergings on vertices. Only used if rule is non-injective and matchings may
				 * be non-injective.
				 */
				graphs::Hypergraph* templateGraphME;

				/**
				 * The mapping from L to templateGraphME. Only used if rule is non-injective
				 * and matchings may be non-injective.
				 */
				graphs::Mapping* templateMappME;

				/**
				 * The mapping from templateGraphOr to the matched graph. Only used if rule
				 * is non-injective.
				 */
				graphs::Mapping* mappPOCtoGraph;

				/**
				 * A list of enumerators for enumerating possibilities to attach edges to split
				 * vertices. Only used if rule is non-injective and matchings are set to be injective.
				 */
				vector<IDtoIDEnumerator>* vertexEnumsInj;

		};

	} /* namespace rule_engine */
} /* namespace uncover */

#endif /* TOTALPOCENUMERATOR_H_ */
