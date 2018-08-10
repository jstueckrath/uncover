/***************************************************************************
 *   Copyright (C) 2005 by SZS                                             *
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

#include "AnonRule.h"
#include "../logging/message_logging.h"

using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace graphs {

		AnonRule::AnonRule(string name, AnonHypergraph_sp left, AnonHypergraph_sp right, Mapping const& match) :
				name(name), left(left), right(right), interface(match) {}

		AnonRule::AnonRule(AnonRule const& oldAnonRule) :
				name(oldAnonRule.name),
				left(make_shared<AnonHypergraph>(*oldAnonRule.left)),
				right(make_shared<AnonHypergraph>(*oldAnonRule.right)),
				interface(oldAnonRule.interface) {}

		AnonRule::AnonRule(AnonHypergraph const& graph) :
				name(),
				left(make_shared<AnonHypergraph>(graph)),
				right(make_shared<AnonHypergraph>(graph)),
				interface() {

			for(auto it = left->cbeginVertices(); it != left->cendVertices(); ++it) {
				interface[it->first] = it->first;
			}

			for(auto it = left->cbeginEdges(); it != left->cendEdges(); ++it) {
				interface[it->first] = it->first;
			}

		}

		AnonRule::~AnonRule()	{}

		string AnonRule::getName() const
		{
			return this->name;
		}

		void AnonRule::setName ( string name )
		{
			this->name = name;
		}

		AnonHypergraph_sp AnonRule::getLeft()
		{
			return left;
		}

		AnonHypergraph_sp AnonRule::getRight()
		{
			return right;
		}

		AnonHypergraph_csp AnonRule::getLeft() const {
			return left;
		}

		AnonHypergraph_csp AnonRule::getRight() const {
			return right;
		}

		void AnonRule::setLeft(AnonHypergraph_sp graph)
		{
			left = graph;
		}

		void AnonRule::setRight(AnonHypergraph_sp graph)
		{
			right = graph;
		}

		Mapping& AnonRule::getMapping() {
			return interface;
		}

		Mapping const& AnonRule::getMapping() const {
			return interface;
		}

		bool AnonRule::isBackApplicable(AnonHypergraph const& graph, Mapping const& match) const {

			// store all elements of the right side with preimage in the left side
			unordered_set<IDType> RwithPre;
			for (Mapping::const_iterator it = interface.cbegin(); it != interface.cend(); ++it)
			{
				RwithPre.insert(it->second);
			}

			// store all elements of the matched graph G with preimage in the right side
			unordered_set<IDType> GwithPre;
			for (Mapping::const_iterator it = match.cbegin(); it != match.cend(); ++it)
			{
				GwithPre.insert(it->second);
			}

			// ************************** check dangling condition **************************

			for(auto it = right->cbeginVertices(); it != right->cendVertices(); ++it) {

				if(!RwithPre.count(it->first)) {

					auto conEdges = graph.getConnectedEdges(match.getTar(it->first));
					if(conEdges != nullptr) {

						for(auto edgeIt = conEdges->cbegin(); edgeIt != conEdges->cend(); ++edgeIt) {
							if(!GwithPre.count(*edgeIt)) {
								return false;
							}
						}

					} // else: no connected edges, no problem

				} // else vertex can be ignored, since it is not created by the AnonRule

			}

			// ************************** check identification condition **************************
			Mapping::Inversion invMatch;
			match.getInverseMatch(invMatch);

			for(Mapping::Inversion::const_iterator it = invMatch.cbegin(); it != invMatch.cend(); ++it) {

				if(it->second.size() > 1) {

					for(unordered_set<IDType>::const_iterator invMatchIt = it->second.cbegin(); invMatchIt != it->second.cend(); ++invMatchIt) {
						if(!RwithPre.count(*invMatchIt)) {
							return false;
						}
					}

				} // else: no identification, no problem

			}

			// if no check failed, the pushout complement does exist
			return true;
		}

		void AnonRule::setMapping(Mapping const& map)
		{
			this->interface = map;
		}

		void AnonRule::fillSplitRule(pair<AnonRule_sp, AnonRule_sp>& rulePair) const {
			rulePair.second = make_shared<AnonRule>(*this);
			rulePair.first = make_shared<AnonRule>(*this->getLeft());
			adjustSplitRule(*rulePair.first, *rulePair.second);
		}

		void AnonRule::adjustSplitRule(AnonRule& partial, AnonRule& total) const {

			auto oldLeft = this->getLeft();
			Mapping const& oldMap = this->getMapping();

			unordered_set<IDType> deletedElements;

			// delete unmapped edges in right side or partial and left side of total morphism
			for(auto it = oldLeft->beginEdges(); it != oldLeft->endEdges(); ++it)
			{
				if(!oldMap.hasSrc(it->first)) {
					deletedElements.insert(it->first);
					partial.getRight()->deleteEdge(it->first);
					total.getLeft()->deleteEdge(it->first);
				}
			}

			// delete unmapped vertices in right side or partial and left side of total morphism
			for(auto it = oldLeft->beginVertices(); it != oldLeft->endVertices(); ++it)
			{
				if(!oldMap.hasSrc(it->first)) {
					deletedElements.insert(it->first);
					partial.getRight()->deleteVertex(it->first);
					total.getLeft()->deleteVertex(it->first);
				}
			}

			partial.getMapping().delTar(deletedElements);
		}

		bool AnonRule::isInjective() const {
			return interface.isInjective();
		}

		bool AnonRule::isConflictFree(Mapping const& match) const {

			Mapping::Inversion invMatch;
			match.getInverseMatch(invMatch);

			for(auto it = invMatch.cbegin(); it != invMatch.cend(); ++it) {
				// ignore all injective parts of the match
				if(it->second.size() > 1) {
					auto innerIt = it->second.cbegin();
					bool defined = interface.hasSrc(*innerIt);
					++innerIt;
					for(; innerIt != it->second.cend(); ++innerIt) {
						if(defined != interface.hasSrc(*innerIt)) {
							return false;
						}
					}
				}
			}

			return true;
		}

		void AnonRule::streamTo(std::ostream& ost) const {
			ost << "AnonRule(name=" << name << ",\n";
			ost << "LHS=" << (*left) << ",\nRHS=" << (*right) << "\n";
			ost << "mapping=" << interface << ")";
		}

	}
}
