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

#ifndef STDMATCHFINDER_H_
#define STDMATCHFINDER_H_

#include "../basic_types/globals.h"
#include "../graphs/AnonHypergraph.h"
#include "../graphs/Mapping.h"

namespace uncover {
	namespace rule_engine {

		/**
		 * StdMatchFinder provides a simple algorithm which takes two arbitrary graphs and calculates all matchings from
		 * one to the other. The Matcher can calculate injective as well as non-injective matches and can search for all
		 * or just one match. The result is a collection of Mappings.
		 * @author Jan Stückrath
		 * @see graphs::Hypergraph
		 * @see graphs::Mapping
		 */
		class StdMatchFinder {

			public:

				/**
				 * Generates a new instance of StdMatchFinder. No further initialization is necessary.
				 * @param injective set to true, if only injective matches should be returned
				 * @param stopFast set to true, if the computation should be stoped after the first match is found
				 */
				StdMatchFinder(bool injective = false, bool stopFast = false);

				/**
				 * Destroys the current instance of StdMatchFinder.
				 */
				virtual ~StdMatchFinder();

				/**
				 * Sets whether this matcher should generate injective matches or not.
				 * @param injective 'true' will cause injective matches to be generated
				 */
				void setInjectivity(bool injective);

				/**
				 * Returns whether this matcher generates injective or non-injective matches.
				 * @return true, iff this matcher will generate only injective matches
				 */
				bool matchesInjectively() const;

				/**
				 * Sets whether this matcher should stop after the first matching found.
				 * @param stopFast 'true' will cause the matcher to stop after finding one match
				 */
				void setStopOnFirstFind(bool stopFast);

				/**
				 * Returns whether the matcher will stop after finding the first match.
				 * return true, iff this matcher stops after finding the first match
				 */
				bool stopsOnFirstFind() const;

				/**
				 * This method takes two graphs and calculates all matches from the source to the target graph.
				 * According to the parameter it is either searched for injective or non-injective matches.
				 * Instead of searching for all possible matches, the procedure can also terminate after the
				 * first match is found. In any case a list of the possible matches is returned.
				 * @param source the source graph, which is mapped to the target graph
				 * @param target the target graph, which is mapped from the source graph
				 * @return a collection of all matchings from the source graph to the target graph (an empty
				 * 				 collection, if none exist)
				 */
				shared_ptr<vector<graphs::Mapping>> findMatches(
						graphs::AnonHypergraph const& source,
						graphs::AnonHypergraph const& target) const;

			protected:

				/**
				 * Maps all edges of the target graph recursively to all edges of the source graph, including
				 * the induced mapping on the nodes. Either searches for one or all Mappings. These Mappings
				 * can be restricted to be injective.
				 * @param source the source graph, the Mapping maps from
				 * @param target the target graph, the Mapping maps to
				 * @param edgesToGo the number of edges of the source graph, which still have to be mapped;
				 * 				initially this has to be the number of edges of the source graph
				 * @param mappableEdges a working set of edges, which are not yet mapped, but attached to
				 * 				already mapped nodes; initially this has to be an empty set
				 * @param currentMapping the currently worked on (partial) Mapping; initially has to be an
				 * 				empty Mapping
				 * @param revCurrentMapping the reverse of currentMapping; only used if injective=true;
				 * 				initially has to be an empty Mapping
				 * @param foundMappings the steadily growing collection of found (total) Mappings; initially
				 * 				has to be an empty vector
				 */
				void mapEdges(
						graphs::AnonHypergraph const& source,
						graphs::AnonHypergraph const& target,
						size_t edgesToGo,
						unordered_set<IDType>& mappableEdges,
						graphs::Mapping& currentMapping,
						graphs::Mapping& revCurrentMapping,
						shared_ptr<vector<graphs::Mapping>> foundMappings) const;

				/**
				 * Checks if a given edge of the source graph can be mapped to a given edge of the target graph. This includes
				 * checking for label equality and if the result is still a Mapping (and not a relation). If necessary, the
				 * injectivity of the Mapping is also checked.
				 * @param source the source graph
				 * @param srcEdge the source edge ID to check (must be part of the source graph)
				 * @param target the target graph
				 * @param tarEdge the target edge ID to check (must be part of the target graph)
				 * @param currentMapping the currently worked on (partial) Mapping
				 * @param revCurrentMapping the reverse of currentMapping; only used if injective=true
				 */
				bool isMapPossible(
						graphs::AnonHypergraph const& source,
						IDType srcEdge,
						graphs::AnonHypergraph const& target,
						IDType tarEdge,
						graphs::Mapping& currentMapping,
						graphs::Mapping& revCurrentMapping) const;

				/**
				 * Searched for an unmapped edge in the source graph according to a simple optimization criterion. This criterion is:
				 * has the maximal arity.
				 * @param source the graph to search in
				 * @param currentMapping the currently worked on (partial) Mapping
				 * @return the ID of the "best" unmapped edge
				 */
				IDType getNewEntryPoint(
						graphs::AnonHypergraph const& source,
						graphs::Mapping const& currentMapping) const;

				/**
				 * Maps all isolated notes of the source graph to the target graph. According to the parameters, either one or all
				 * Mappings are searched for and these Mappings must be injective or not. The result is returned as a vector of
				 * Mappings
				 * @param source the source graph, mapped from
				 * @param target the target graph, mapped to
				 * @param foundMappings the collection of Mappings found so far, where the isolated nodes still need to be mapped
				 * @return a vector containing all Mappings including mapped isolated nodes
				 */
				shared_ptr<vector<graphs::Mapping>> mapIsolatedNodes(
						graphs::AnonHypergraph const& source,
						graphs::AnonHypergraph const& target,
						shared_ptr<vector<graphs::Mapping>> foundMappings) const;

				/**
				 * This method recursively enumerates through all possibilities to map the isolated nodes injectively to the
				 * target graph. For each such possibility a new Mapping is added to the collection of all Mappings.
				 * @param isolatedNodes a vector containing all isolated nodes of the source graph
				 * @param isolatedNodesPos the current position in the list of isolated nodes, a map is searched for;
				 * 				initially has to be 0
				 * @param targets a collection of all nodes of the target graph, where the isolated nodes can be mapped to;
				 * 				has to be all nodes of the target graph not already mapped to
				 * @param noTargets a set of all targets already used for isolated node mapping; initially has to be an
				 * 				empty set
				 * @param currentMapping the currently worked on (partial) Mapping
				 * @param allMappings a collection, where all resulting Mappings will be stored; should be empty initially
				 */
				void enumAllIsoNodesInj(
						vector<IDType>& isolatedNodes,
						size_t isolatedNodesPos,
						vector<IDType>& targets,
						unordered_set<IDType>& noTargets,
						graphs::Mapping& currentMapping,
						shared_ptr<vector<graphs::Mapping>> allMappings) const;

				/**
				 * Enumerates through all possibilities to map the isolated nodes non-injectively to the target graph. For each such
				 * possibility a new Mapping is added to the collection of all Mappings.
				 * @param isolatedNodes a vector containing all isolated nodes of the source graph
				 * @param targets a collection of all nodes of the target graph, where the isolated nodes can be mapped to;
				 * 				has to be all nodes of the target graph
				 * @param currentMapping the currently worked on (partial) Mapping
				 * @param allMappings a collection, where all resulting Mappings will be stored; should be empty initially
				 */
				void enumAllIsoNodesNonInj(vector<IDType>& isolatedNodes,
						vector<IDType>& targets,
						graphs::Mapping& currentMapping,
						shared_ptr<vector<graphs::Mapping>> allMappings) const;

			private:

				/**
				 * Stores whether the used matches should injective or not.
				 */
				bool matchInjective;

				/**
				 * Stores whether the matcher should stop after finding at least one match.
				 */
				bool stopOnFirstFind;

		};

	} /* namespace rule_engine */
} /* namespace uncover */
#endif /* MATCHFINDER_H_ */
