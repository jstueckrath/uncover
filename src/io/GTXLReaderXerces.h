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

#ifndef GTXLREADERXERCES_H_
#define GTXLREADERXERCES_H_

#include "GTXLReader.h"
#include <xercesc/dom/DOMElement.hpp>
#include <tuple>

namespace uncover {
	namespace io {

		/**
		 * This class imports GXL or GTXL XML files and converts them to Hypergraph and GTS objects respectively.
		 * Xerces is used as underlying Framework.
		 * @author Jan Stückrath
		 * @see graphs::Hypergraph
		 * @see graphs::GTS
		 */
		class GTXLReaderXerces: public io::GTXLReader {

			public:

				/**
				 * Creates a new GTXLReaderXerces object, which includes initialization of the Xerces framework. No additional
				 * initialization is needed.
				 */
				GTXLReaderXerces();

				/**
				 * Destroys the GTXLReaderXerces object. This also frees all resources reserved for Xerces.
				 */
				virtual ~GTXLReaderXerces();

				virtual shared_ptr<vector<graphs::Hypergraph_sp>> readGXL(string filename,
						unordered_map<IDType,IDType>* oldIDs) const override;

				virtual graphs::GTS_sp readGTXL(string filename,
						unordered_map<IDType,IDType>* oldRuleIDs = nullptr,
						unordered_map<IDType,IDType>* oldGraphIDs = nullptr) const override;

			private:

				/**
				 * Alias for a map, mapping every quantification name to the set of ID of which it consists.
				 */
				typedef unordered_map<string, unordered_set<IDType>> QuantMap;

				/**
				 * Alias for a triple consisting of a rule (first element) and two QuantMaps for the left rule side
				 * (second element) and the right rule side (third element).
				 */
				typedef std::tuple<graphs::Rule_sp, GTXLReaderXerces::QuantMap, GTXLReaderXerces::QuantMap> RuleTriple;

				/**
				 * Takes the given UQRule and generates a quantification consisting of all specified element in the
				 * left and right sides. The elements with IDs specified by 'leftQuant' and 'rightQuant' are moved from
				 * the core rule of the given UQRule into a new quantification (deleting parts of the core rule). The
				 * mapping of the quantification is implicitly given by the core rule (and deleted from it in the process).
				 * @param rule the UQRule for which the quantification should be generated
				 * @param leftQuant the set of IDs of which the left quantification side will consist
				 * @param rightQuant the set of IDs of which the right quantification side will consist
				 */
				void convertToQuant(graphs::UQRule& rule,
						unordered_set<IDType> const& leftQuant,
						unordered_set<IDType> const& rightQuant) const;

				/**
				 * This method takes a DOMElement object and tries to extract a Hypergraph out of it. The given DOMElement has
				 * to be the root element of a 'graph' node specified by the GXL definition. Additionally to a pointer to the
				 * extracted Hypergraph, a mapping of the IDs used in the XML file to the internal IDs of the Hypergraph, is returned.
				 * @param graphroot a DOMElement object containing the root element of a 'graph'
				 * @param oldIDs if given, the read graphs new ID is mapped to the old (stored) ID
				 * @return a pointer to the extracted Hypergraph and a mapping of the XML IDs to the Hypergraph IDs
				 * @throws exceptions::XMLioException if the given DOMElement is not a correct Hypergraph
				 */
				pair<graphs::Hypergraph_sp,unordered_map<string, IDType>> readHypergraph(xercesc::DOMElement* graphroot,
						unordered_map<IDType,IDType>* oldIDs = nullptr) const;

				/**
				 * This method takes a DOMElement object and tries to extract an AnonHypergraph out of it. The given DOMElement has
				 * to be the root element of a 'graph' node specified by the GXL definition. Additionally to a pointer to the
				 * extracted AnonHypergraph, a mapping of the IDs used in the XML file to the internal IDs of the Hypergraph, is returned.
				 * @param graphroot a DOMElement object containing the root element of a 'graph'
				 * @return a pointer to the extracted graph and a mapping of the XML IDs to the graph IDs
				 * @throws exceptions::XMLioException if the given DOMElement is not a correct graph
				 */
				pair<graphs::AnonHypergraph_sp,unordered_map<string, IDType>> readAnonHypergraph(xercesc::DOMElement* graphroot) const;

				/**
				 * This method serves as an implementation of both the readHypergraph(..,..) and the readAnonHypergraph(..) functions,
				 * filling all parameters given, with exception of the first, with data read from the given XML node.
				 * The given DOMElement has to be the root element of a 'graph' node specified by the GXL definition. If the oldIDs
				 * parameter in not null, the old ID stored in the 'graph' XML node is read and the graphs new ID is mapped to it.
				 * The read graph is stored into the given graph parameter. The given graphID is only used if oldIDs is not null
				 * (as key in the mapping). In the strToID parameter all XML IDs of vertices and edges are mapped to their IDs in the
				 * read graph.
				 * @param graphroot a DOMElement object containing the root element of a 'graph'
				 * @param oldIDs if given, the read graphs new ID is mapped to the old (stored) ID
				 * @param graph the graph to which the read data is stored
				 * @param graphID the ID of the given graph, if it is not an anonymous graph (ignored if oldIDs is null)
				 * @param strToID every XML ID of a vertex or edge with be mapped by this map to their ID in the read graph
				 * @throws exceptions::XMLioException if the given DOMElement is not a correct graph
				 */
				void readAnonHypergraph(xercesc::DOMElement* graphroot,
						unordered_map<IDType,IDType>* oldIDs,
						graphs::AnonHypergraph& graph,
						IDType const& graphID,
						unordered_map<string,IDType>& strToID) const;

				/**
				 * This method takes a DOMElement object and tries to extract a Rule out of it. The given DOMElement has
				 * to be the root element of a 'rule' node specified by the GTXL definition.
				 * @param ruleroot a DOMElement object containing the root element of a 'rule'
				 * @param oldIDs if given, the read rules new ID is mapped to the old (stored) ID
				 * @return a pointer to the extracted Rule
				 * @throws exceptions::XMLioException if the given DOMElement is not a correct Rule
				 */
				GTXLReaderXerces::RuleTriple readRule(xercesc::DOMElement* ruleroot,
						unordered_map<IDType,IDType>* oldIDs = nullptr) const;

				/**
				 * Takes a 'quantification' XML element and extracts the QuantMap from it.
				 * @param root the 'quantification' XML element
				 * @param quantMap the QuantMap in which the read entries will be stored
				 * @param idMap a mapping from the XML IDs to IDs of the graph on which the quantification is defined
				 */
				void readQuantMap(xercesc::DOMElement* root,
						GTXLReaderXerces::QuantMap& quantMap,
						unordered_map<string,IDType>& idMap) const;

				/**
				 * Checks if the input parameters form a correct and consistent Rule. Existence of mapped IDs is checked as well as
				 * the morphism property. An exception is thrown if the check is not successful.
				 * @param mapping a reference to the mapping to be checked
				 * @param left a pointer to the left graph of the Match
				 * @param right a pointer to the right graph of the Match
				 * @throws XMLioException if the given parameters form no correct Rule object
				 */
				void checkConsistency(const graphs::Mapping& match,
						const graphs::Hypergraph& left,
						const graphs::Hypergraph& right) const;

				/**
				 * Checks if a consistent UQRule can be generated from the given Rule and QuantMaps. An exception is thrown if
				 * the check is not successful.
				 * @param rule the rule for which consistency should be checked
				 * @param leftQuantMap a mapping of all quantifications to the IDs of which they consist in the left rule side
				 * @param rightQuantMap a mapping of all quantifications to the IDs of which they consist in the right rule side
				 */
				void checkConsistency(graphs::Rule const& rule,
						GTXLReaderXerces::QuantMap& leftQuantMap,
						GTXLReaderXerces::QuantMap& rightQuantMap) const;

				/**
				 * Checks if the given partition of the given graph into quantifications is consistent. An exception is thrown
				 * if the check is not successful.
				 * @param graph the graph to be checked
				 * @param quantMap a mapping of all quantifications to the IDs of which they consist
				 */
				void checkConsistency(graphs::AnonHypergraph const& graph, GTXLReaderXerces::QuantMap const& quantMap) const;

				/**
				 * Checks if the two strings are equal. The first string is assumed to be lower case, while upper case symbols
				 * of the second string will be treated as if they where lower case.
				 * @param lowerCase a string containing only lower case symbols
				 * @param notLowerCase a string containing lower and/or upper case symbols
				 * @return true, iff the strings are equal, ignoring cases of the second string
				 */
				inline bool isEqualIgnoringCase2(const XMLCh* lowerCase, const XMLCh* notLowerCase) const;

				///////////////////////////////////////////////////////////////////////////////////////
				// This XML strings will be pre-transcoded and later used in the GXL and GTXL imports.
				///////////////////////////////////////////////////////////////////////////////////////
				XMLCh* xmlch_gxl;
				XMLCh* xmlch_edgeids;
				XMLCh* xmlch_true;
				XMLCh* xmlch_hypergraph;
				XMLCh* xmlch_text;
				XMLCh* xmlch_id;
				XMLCh* xmlch_rel;
				XMLCh* xmlch_relend;
				XMLCh* xmlch_target;
				XMLCh* xmlch_role;
				XMLCh* xmlch_startorder;
				XMLCh* xmlch_attr;
				XMLCh* xmlch_name;
				XMLCh* xmlch_string;
				XMLCh* xmlch_edgemode;
				XMLCh* xmlch_node;
				XMLCh* xmlch_edge;
				XMLCh* xmlch_label;
				XMLCh* xmlch_graph;
				XMLCh* xmlch_int;

				////////////////////////////////////////////////////////////////////////////////
				// This XML strings will be pre-transcoded and later used in the GTXL imports.
				////////////////////////////////////////////////////////////////////////////////
				XMLCh* xmlch_gts;
				XMLCh* xmlch_initial;
				XMLCh* xmlch_rule;
				XMLCh* xmlch_lhs;
				XMLCh* xmlch_rhs;
				XMLCh* xmlch_rulegraph;
				XMLCh* xmlch_mapping;
				XMLCh* xmlch_mapelem;
				XMLCh* xmlch_from;
				XMLCh* xmlch_to;
				XMLCh* xmlch_type;
				XMLCh* xmlch_transformationunit;
				XMLCh* xmlch_quantification;
				XMLCh* xmlch_elem;
				XMLCh* xmlch_ref;

		};

	} /* namespace io */
} /* namespace uncover */
#endif
