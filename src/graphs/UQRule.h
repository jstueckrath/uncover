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

#ifndef UQRULE_H_
#define UQRULE_H_

#include "UQRuleInstance.h"
#include "Rule.h"

namespace uncover {
	namespace graphs {

		/**
		 * This class serves as data structure of a universally quantified rule. These rules consist of a main rule and
		 * a (finite) set of universal quantifications. Before applying such a rule, we first need to instantiate the
		 * rule by generating UQRuleInstances. The theory of universally quantified rules is described in
		 * "Parameterized verification of graph transformation systems with whole neighbourhood operations" by
		 * Delzanno and Stückrath.
		 * @see UQRuleInstance
		 * @author Jan Stückrath
		 */
		class UQRule : public basic_types::Streamable {

			public:

				/**
				 * A Quantification consists of a rule and a Mapping from the main rules left-hand side to the
				 * Quantifications left-hand side.
				 */
				typedef pair<AnonRule,Mapping> Quantification;

				/**
				 * Generates a UQRule without any Quantifications using the given rule as the main rule.
				 * @param mainRule the Rule to be used as main rule (the rule will be copied)
				 */
				UQRule(Rule const& mainRule);

				/**
				 * Generates a new UQRule with the given name, left graph, right graph and Mapping. This
				 * constructor does not generate any Quantifications.
				 * @param name the name of the new UQRule
				 * @param left the left graph of the main rule
				 * @param right the right graph of the main rule
				 * @param match a Mapping from the left to the right graph, used for the main rule
				 */
				UQRule(string name = "",
						AnonHypergraph_sp left = make_shared<AnonHypergraph>(),
						AnonHypergraph_sp right = make_shared<AnonHypergraph>(),
						Mapping const& match = Mapping());

				/**
				 * Destroys this UQRule. This also destroys the encapsulated main rule.
				 */
				virtual ~UQRule();

				/**
				 * Returns the ID of this UQRule.
				 * @return the ID of this UQRule
				 */
				IDType getID() const;

				/**
				 * Returns the name of this UQRule.
				 * @return the name of this UQRule
				 */
				string getName() const;

				/**
				 * Returns a reference to the main rule encapsulated by this UQRule.
				 * @return a reference to the main rule
				 */
				Rule& getMainRule();

				/**
				 * Returns a reference to the main rule encapsulated by this UQRule.
				 * @return a reference to the main rule
				 */
				Rule const& getMainRule() const;

				/**
				 * Adds a Quantification to this UQRule consisting of the given AnonRule and Mapping. The AnonRule
				 * may be any rule and the Mapping has to be a total injective Mapping from the left-hand side of
				 * the main rule to the left-hand side of the given rule. However, the composition of the given
				 * mapping and rule must be total and injective, and the given rule may only merge elements which
				 * have no preimage in the main rules left-hand side. The validity of the parameters is assumed
				 * and not checked!
				 * @param rule the rule part of the quantification
				 * @param match a Mapping from the main rules left-hand side to the given rules left-hand side
				 */
				void addQuantification(AnonRule const& rule, Mapping const& match);

				/**
				 * Adds the given Quantification to this UQRule. The validity of the given Quantification is assumed
				 * and not checked!
				 * @see addQuantification(AnonRule const&, Mapping const&)
				 * @param quantification the Quantification to be added
				 */
				void addQuantification(Quantification const& quantification);

				/**
				 * Returns the number of Quantifications of this UQRule.
				 * @return the number of Quantifications of this UQRule
				 */
				size_t getQuantCount() const;

				/**
				 * Returns a reference to the vector storing the Quantifications of this UQRule.
				 * @return a reference to the vector storing the Quantifications of this UQRule
				 */
				vector<Quantification>& getQuantifications();

				/**
				 * Returns a reference to the vector storing the Quantifications of this UQRule.
				 * @return a reference to the vector storing the Quantifications of this UQRule
				 */
				vector<Quantification> const& getQuantifications() const;

				/**
				 * Generates and returns the instantiation of this UQRule where each Quantification
				 * is added as often specified by the given vector of quantities. The i-th value
				 * in the vector specifies how often the i-th Quantification is used. If desired,
				 * the set of IDs generated by this function in the left and right graphs can be
				 * stored.
				 * @param quantities specifies how often each Quantification is used for this
				 * 				instantiation
				 * @param genLeft if non-null, all IDs of elements generated in the left-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @param genRight if non-null, all IDs of elements generated in the right-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @return the generated instantiation; the returned pointed object may be freely changed
				 * 				 without affecting this UQRule
				 */
				UQRuleInstance_sp createInstance(
						vector<unsigned int> const& quantities,
						vector<unordered_set<IDType>>* genLeft = nullptr,
						vector<unordered_set<IDType>>* genRight = nullptr) const;

				/**
				 * Generates and returns the instantiation of this UQRule where no Quantification is added.
				 * @return the zero instance of this UQRule; the returned pointed object may be freely changed
				 * 				 without affecting this UQRule
				 */
				UQRuleInstance_sp createZeroInstance() const;

				/**
				 * Generates and returns the instantiation of this UQRule where each Quantification
				 * is added exactly once. If desired, the set of IDs generated by this function in
				 * the left and right graphs can be stored.
				 * @param genLeft if non-null, all IDs of elements generated in the left-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @param genRight if non-null, all IDs of elements generated in the right-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @return the generated instantiation; the returned pointed object may be freely changed
				 * 				 without affecting this UQRule
				 */
				UQRuleInstance_sp createOneInstance(
						vector<unordered_set<IDType>>* genLeft = nullptr,
						vector<unordered_set<IDType>>* genRight = nullptr) const;

				/**
				 * Generates and returns a new instantiation of this UQRule by copying the given instance and
				 * adding the Quantification with the given index. If desired, the set of IDs generated by
				 * this function in the left and right graphs can be stored.
				 * @param rule the instance to be copied
				 * @param quantID the ID/index of the Quantification to be added
				 * @param genLeft if non-null, all IDs of elements generated in the left-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @param genRight if non-null, all IDs of elements generated in the right-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @return the generated instantiation; the returned pointed object may be freely changed
				 * 				 without affecting this UQRule of the given instantiation
				 */
				UQRuleInstance_sp extendInstance(
						UQRuleInstance_csp rule,
						size_t quantID,
						vector<unordered_set<IDType>>* genLeft = nullptr,
						vector<unordered_set<IDType>>* genRight = nullptr) const;

				virtual void streamTo(std::ostream& ost) const override;

			private:

				/**
				 * Generates a new instantiation of this UQRule by adding the Quantification with the given
				 * index to the given instance. If desired, the set of IDs generated by this function in the
				 * left and right graphs can be stored.
				 * @param rule the instantiation to which the Quantification will be added
				 * @param quantID the ID/index of the Quantification to be added
				 * @param genLeft if non-null, all IDs of elements generated in the left-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 * @param genRight if non-null, all IDs of elements generated in the right-hand side
				 * 				for this instantiation will be stored in this vector, sorted by Quantification
				 */
				void extendRuleByUQ(
						UQRuleInstance& rule,
						size_t quantID,
						vector<unordered_set<IDType>>* genLeft,
						vector<unordered_set<IDType>>* genRight) const;

				/**
				 * Checks if all vertices incident to the given edge (given as edge ID) of the given graph have
				 * an image in the given Mapping. If this is the case, the images of all incident vertices are
				 * stores in the given vector, maintaining their order.
				 * @param graph the graph to which the edgeID belongs
				 * @param edgeID the ID of the edge of which the incident vertices will be checked
				 * @param vmap the Mapping used for checking the vertex IDs
				 * @param vertices a vector in which the images of incident vertices will be stored
				 * @return true iff all incident vertices have an image under the given Mapping
				 */
				bool checkVertices(
						AnonHypergraph_csp graph,
						IDType const& edgeID,
						Mapping const& vmap,
						vector<IDType>& vertices) const;

				/**
				 * Stores the main rule of this UQRule. Also contains this UQRules ID and name.
				 */
				Rule mainRule;

				/**
				 * Stores the Quantifications of this UQRule, which will later be indexed by their position
				 * in this vector.
				 */
				vector<Quantification> quantifications;

				/**
				 * Every universally quantified vertex in the main rules left-hand side is stored in this set.
				 * A vertex is universally quantified if its image in the left-hand side of some Quantification
				 * is incident to an edge which has no preimage in the main rules left-hand side.
				 */
				unordered_set<IDType> leftUQVertices;

				/**
				 * For all universally quantified vertices this set stores their image in the main rules right-hand
				 * side.
				 */
				unordered_set<IDType> rightUQVertices;

		};

		/**
		 * Alias for a shared pointer to a UQRule.
		 */
		typedef shared_ptr<UQRule> UQRule_sp;

		/**
		 * Alias for a shared pointer to a constant UQRule.
		 */
		typedef shared_ptr<UQRule const> UQRule_csp;

	} /* namespace graphs */
} /* namespace uncover */

#endif /* UQRULE_H_ */
