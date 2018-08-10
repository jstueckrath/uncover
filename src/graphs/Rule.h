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

#ifndef DATA_STRUCTURESRULE_H
#define DATA_STRUCTURESRULE_H

#include "AnonRule.h"
#include "Mapping.h"

namespace uncover {
	namespace graphs {

		/**
		 * Represents the data structure for a rule including a unique identifier. The rule consists of a name,
		 * a left rulegraph, a right rulegraph and a mapping form the left to the right graph. This class slightly
		 * extends the AnonRule class.
		 * @see AnonHypergraph
		 * @see Mapping
		 * @see AnonRule
		 * @author Jan Stückrath
		 */
		class Rule : public AnonRule {

			public:

				/**
				 * Generates a new rule with the given name, left graph, right graph and mapping. If these
				 * parameters are not given, an empty string is used as name, empty graphs are used as left and
				 * right sides and an empty mapping is used as mapping. This rule is also assigned a new
				 * unique identifier. Caution, the left and right graphs are stores and not copied by this
				 * constructor.
				 * @param name the name of the rule (default: empty string)
				 * @param left the left graph of the rule (default: empty graph)
				 * @param right the right graph of the rule (default: empty graph)
				 * @param match the mapping of the left to the right graph (default: empty mapping)
				 */
				Rule(string name = "",
						AnonHypergraph_sp left = make_shared<AnonHypergraph>(),
						AnonHypergraph_sp right = make_shared<AnonHypergraph>(),
						Mapping const& match = Mapping());

				/**
				 * Generates a new rule for the given rule, copying its content. The new rule is an independent
				 * copy of the old rule and is assigned a new unique identifier.
				 * @param oldRule the old rule from which the data will be copied
				 */
				Rule(Rule const& oldRule);

				/**
				 * Generates a new rule for the given rule, copying its content. The new rule is an independent
				 * copy of the old rule and is assigned a new unique identifier.
				 * @param oldRule the old rule from which the data will be copied
				 */
				Rule(AnonRule const& oldRule);

				/**
				 * Generates a new rule using the given graph. A copy of the given graph is generated as left and
				 * as right graph and the mapping is initialized to be the identity. Furthermore, the new rule
				 * is assigned a new unique identifier.
				 * @param graph the graph which will be used as left and right graph
				 */
				Rule(AnonHypergraph const& graph);

				/**
				 * Destroys this Rule object.
				 */
				virtual ~Rule();

				/**
				 * Returns the unique identifier of this Rule object.
				 * @return the unique identifier of this rule
				 */
				IDType getID() const;

				virtual void streamTo(std::ostream& ost) const override;

			protected:

				/**
				 * Stores the ID of this Rule.
				 */
				IDType ID;

		};

		/**
		 * Alias for a shared pointer to a Rule.
		 */
		typedef shared_ptr<Rule> Rule_sp;

		/**
		 * Alias for a shared pointer to a constant Rule.
		 */
		typedef shared_ptr<Rule const> Rule_csp;

	}
}

#endif
