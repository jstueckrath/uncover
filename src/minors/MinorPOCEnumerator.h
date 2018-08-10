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

#ifndef MINORPOCENUMERATOR_H_
#define MINORPOCENUMERATOR_H_

#include "../analysis/MinPOCEnumerator.h"
#include "../rule_engine/PartialPOCEnumerator.h"

namespace uncover {
	namespace minors {

		/**
		 * Implements the abstract MinPOCEnumerator class for the minor ordering. It will enumerate
		 * minimal pushout complement wrt. the minor ordering, such that every pushout complement is
		 * represented by at least one minimal POC.
		 * @author Jan Stückrath
		 */
		class MinorPOCEnumerator : public analysis::MinPOCEnumerator {

			public:

				/**
				 * Generates a new MinorPOCEnumerator without initialization. This enumerator must be initialized
				 * by calling resetWith(), or otherwise its functions will throw exceptions.
				 */
				MinorPOCEnumerator();

				/**
				 * Move constructor. Generates a new MinorPOCEnumerator from a given one, invalidating it at the
				 * same time. The new enumerator will be initialize iff the old one was.
				 * @param oldEnum the enumerator of which the new one will be generated
				 */
				MinorPOCEnumerator(MinorPOCEnumerator&& oldEnum);

				/**
				 * Destroys this MinorPOCEnumerator.
				 */
				virtual ~MinorPOCEnumerator();

				virtual bool ended() const override;

				virtual MinorPOCEnumerator& operator++() override;

				virtual graphs::Hypergraph_sp operator*() const override;

				virtual bool resetWith(graphs::AnonRule_csp rule, IDType ruleID, graphs::Hypergraph_csp graph) override;

				using analysis::MinPOCEnumerator::resetWith;

			private:

				/**
				 * Checks if this MinorPOCEnumerator is in a valid state, i.e. initialized, and throws an
				 * exception if it is not.
				 */
				void checkAndThrow() const;

				/**
				 * Stores an encapsulated partialEnumerator.
				 */
				rule_engine::PartialPOCEnumerator* partialEnumerator;

				/**
				 * The ID of the rule with which this enumerator was last initialized.
				 */
				IDType ruleID;

				/**
				 * The ID of the graph with which this enumerator was last initialized.
				 */
				IDType graphID;

		};

	} /* namespace minors */
} /* namespace uncover */

#endif /* MINORPOCENUMERATOR_H_ */
