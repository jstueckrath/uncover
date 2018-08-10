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

#ifndef GTXLWRITERXERCES_H_
#define GTXLWRITERXERCES_H_

#include "GTXLWriter.h"
#include <xercesc/dom/DOMElement.hpp>

namespace uncover {
	namespace io {

		/**
		 * This class exports GXL or GTXL XML files by converting them from Hypergraph and GTS objects respectively.
		 * Xerces is used as underlying framework.
		 * @author Jan Stückrath
		 */
		class GTXLWriterXerces : public GTXLWriter {

			public:

				/**
				 * Creates a new GTXLWriterXerces object, which includes initialization of the Xerces framework. No additional
				 * initialization is needed.
				 */
				GTXLWriterXerces();

				/**
				 * Destroys the GTXLWriterXerces object. This also frees all resources reserved for Xerces.
				 */
				virtual ~GTXLWriterXerces();

				virtual void writeGXL(vector<graphs::Hypergraph_sp> const& graphs, boost::filesystem::path& filename) const override;

				virtual void writeGTXL(graphs::GTS const& gts, boost::filesystem::path& filename) const override;

			private:

				/**
				 * Takes a Hypergraph, converts it to a DOMElement and adds the resulting object as a child of
				 * the given DOMElement.
				 * @param gxlRoot the DOMElement where the Hypergraph should be added as child
				 * @param graph the Hypergraph to be converted
				 */
				void writeHypergraph(xercesc::DOMElement* gxlRoot, graphs::Hypergraph const& graph) const;

				/**
				 * Takes an AnonHypergraph, converts it to a DOMElement and adds the resulting object as a child of
				 * the given DOMElement. This function is the same as the variant for Hypergraphs, but the graphs
				 * ID (which it does not have) will never be stored even if the option is set. Additionally elements
				 * of this graph will not have a unique (XML) ID outside the graph itself.
				 * @param gxlRoot the DOMElement where the AnonHypergraph should be added as child
				 * @param graph the AnonHypergraph to be converted
				 */
				void writeHypergraph(xercesc::DOMElement* gxlRoot, graphs::AnonHypergraph const& graph) const;

				/**
				 * Implements the writeHypergraph functions for Hypergraph and AnonHypergraph.
				 * @param gxlRoot the DOMElement where the graph should be added as child
				 * @param graph the graph which should be added
				 * @param graphID the ID of the given graph; this can be any value, but should be the Hypergraphs ID
				 * 				or 0 if the graph is an real AnonHypergraph
				 * @return the DOMElement generated for the given graph (already added to the document tree)
				 */
				xercesc::DOMElement* writeHypergraph(xercesc::DOMElement* gxlRoot,
						graphs::AnonHypergraph const& graph,
						IDType const& graphID) const;

				/**
				 * Takes a Rule, converts it to a DOMElement and adds the resulting object as a child of the given
				 * DOMElement.
				 * @param gtxlRoot the DOMElement where the Rule should be added as child
				 * @param rule the Rule to be converted
				 */
				void writeStdRule(xercesc::DOMElement* gtxlRoot, graphs::Rule const& rule) const;

				/**
				 * Takes a UQRule, converts it to a DOMElement and adds the resulting object as a child of the given
				 * DOMElement.
				 * @param gtxlRoot the DOMElement where the UQRule should be added as child
				 * @param rule the UQRule to be converted
				 */
				void writeUQRule(xercesc::DOMElement* gtxlRoot, graphs::UQRule const& rule) const;

				/**
				 * This function is a generalized implementation of writeStdRule(...) and writeUQRule(...). It
				 * creates a DOMElement from for the given rule, using the given ID and marks parts of the rule
				 * as belonging to quantifications. Which elements are marked this ways is defined in the two
				 * given vectors. The created DOMElement is added as a child of the given DOMElement.
				 * @param gtxlRoot the DOMElement where the converted rule will be added as a child
				 * @param rule the rule to be converted in a DOMElement
				 * @param id the ID of the rule
				 * @param leftUQids a vector of sets of IDs of the left-hand side of the rule; the i-th set of the
				 * 				vector contains all elements belonging to the i-th Quantification of the rule
				 * @param rightUQids a vector of sets of IDs of the right-hand side of the rule; the i-th set of
				 * 				the vector contains all elements belonging to the i-th Quantification of the rule
				 */
				void writeRule(xercesc::DOMElement* gtxlRoot,
						graphs::AnonRule const& rule,
						IDType id,
						vector<unordered_set<IDType>> const* leftUQids,
						vector<unordered_set<IDType>> const* rightUQids) const;

				/**
				 * Generates a unique XML ID for a Hypergraph.
				 * @param graphID the internal ID of the represented Hypergraph
				 * @return a unique XML ID for a Hypergraph
				 */
				string generateGraphID(const IDType& graphID) const;

				/**
				 * Generates a unique XML ID for a vertex of a graph. If the given graphID is not unique, the generated
				 * ID will only be unique within the graph.
				 * @param nodeID the internal ID of the vertex
				 * @param graphID the internal ID of the graph containing the vertex
				 * @return a unique XML ID for a vertex
				 */
				string generateVertexID(const IDType& nodeID, const IDType& graphID) const;

				/**
				 * Generates a unique XML ID for an edge of a graph. If the given graphID is not unique, the generated
				 * ID will only be unique within the graph.
				 * @param edgeID the internal ID of the edge
				 * @param graphID the internal ID of the graph containing the edge
				 * @return a unique XML ID for a edge
				 */
				string generateEdgeID(const IDType& edgeID, const IDType& graphID) const;

				/**
				 * Generates a unique XML ID for a Rule.
				 * @param ruleID the internal ID of the represented Rule
				 * @return a unique XML ID for a Rule
				 */
				string generateRuleID(const IDType& ruleID) const;

				/**
				 * Generates a unique XML ID for the left graph of a Rule.
				 * @param ruleID the internal ID of the represented Rule
				 * @return a unique XML ID for the left graph of a Rule
				 */
				string generateLeftRulegraphID(const IDType& ruleID) const;

				/**
				 * Generates a unique XML ID for the right graph of a Rule.
				 * @param ruleID the internal ID of the represented Rule
				 * @return a unique XML ID for the right graph of a Rule
				 */
				string generateRightRulegraphID(const IDType& ruleID) const;

				///////////////////////////////////////////////////////////////////////////////////////
				// This XML strings will be pre-transcoded and later used in the GXL and GTXL imports.
				///////////////////////////////////////////////////////////////////////////////////////
				XMLCh* xmlch_gxl;
				XMLCh* xmlch_graph;
				XMLCh* xmlch_edgeids;
				XMLCh* xmlch_true;
				XMLCh* xmlch_hypergraph;
				XMLCh* xmlch_id;
				XMLCh* xmlch_edgemode;
				XMLCh* xmlch_undirected;
				XMLCh* xmlch_node;
				XMLCh* xmlch_vertex;
				XMLCh* xmlch_rel;
				XMLCh* xmlch_attr;
				XMLCh* xmlch_name;
				XMLCh* xmlch_string;
				XMLCh* xmlch_label;
				XMLCh* xmlch_relend;
				XMLCh* xmlch_target;
				XMLCh* xmlch_role;
				XMLCh* xmlch_startorder;
				XMLCh* xmlch_int;

				//////////////////////////////////////////////////////////////////////////////////
				// This XML strings will be pre-transcoded and later used in the GTXL imports.
				//////////////////////////////////////////////////////////////////////////////////
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
				XMLCh* xmlch_quantification;
				XMLCh* xmlch_elem;
				XMLCh* xmlch_ref;

		};

	} /* namespace io */
} /* namespace uncover */

#endif /* GTXLWRITERXERCES_H_ */
