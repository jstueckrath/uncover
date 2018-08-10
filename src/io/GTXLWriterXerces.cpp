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

#include "GTXLWriterXerces.h"
#include "XMLioException.h"
#include <boost/lexical_cast.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include "xercesc_helpers.h"
#include <sstream>

#define ANONGRAPHID 0

using namespace xercesc;
using namespace uncover::graphs;
using namespace boost::filesystem;
using namespace std;

namespace uncover {
	namespace io {

		GTXLWriterXerces::GTXLWriterXerces() {

			// Initialize Xerces platform
			try {
				XMLPlatformUtils::Initialize();
			}
			catch( XMLException& e ) {
				SafeXTString<char> message(XMLString::transcode(e.getMessage()));
				stringstream ss;
				ss << "Failed to initialize Xerces: " << message;
				throw XMLioException(ss.str());
			}

			// pre-transcode some strings to XMLCH* for later use
			xmlch_gxl = XMLString::transcode("gxl");
			xmlch_graph = XMLString::transcode("graph");
			xmlch_edgeids = XMLString::transcode("edgeids");
			xmlch_true = XMLString::transcode("true");
			xmlch_hypergraph = XMLString::transcode("hypergraph");
			xmlch_undirected = XMLString::transcode("undirected");
			xmlch_id = XMLString::transcode("id");
			xmlch_edgemode = XMLString::transcode("edgemode");
			xmlch_node = XMLString::transcode("node");
			xmlch_vertex = XMLString::transcode("vertex");
			xmlch_rel = XMLString::transcode("rel");
			xmlch_relend = XMLString::transcode("relend");
			xmlch_target = XMLString::transcode("target");
			xmlch_role = XMLString::transcode("role");
			xmlch_startorder = XMLString::transcode("startorder");
			xmlch_attr = XMLString::transcode("attr");
			xmlch_name = XMLString::transcode("name");
			xmlch_string = XMLString::transcode("string");
			xmlch_label = XMLString::transcode("label");
			xmlch_gts = XMLString::transcode("gts");
			xmlch_initial = XMLString::transcode("initial");
			xmlch_rule = XMLString::transcode("rule");
			xmlch_lhs = XMLString::transcode("lhs");
			xmlch_rhs = XMLString::transcode("rhs");
			xmlch_rulegraph = XMLString::transcode("rulegraph");
			xmlch_mapping = XMLString::transcode("mapping");
			xmlch_mapelem = XMLString::transcode("mapelem");
			xmlch_from = XMLString::transcode("from");
			xmlch_to = XMLString::transcode("to");
			xmlch_int = XMLString::transcode("int");
			xmlch_quantification = XMLString::transcode("quantification");
			xmlch_elem = XMLString::transcode("elem");
			xmlch_ref = XMLString::transcode("ref");

		}

		GTXLWriterXerces::~GTXLWriterXerces() {

			// delete pre-transcoded strings
			XMLString::release(&xmlch_gxl);
			XMLString::release(&xmlch_graph);
			XMLString::release(&xmlch_edgeids);
			XMLString::release(&xmlch_true);
			XMLString::release(&xmlch_hypergraph);
			XMLString::release(&xmlch_id);
			XMLString::release(&xmlch_edgemode);
			XMLString::release(&xmlch_undirected);
			XMLString::release(&xmlch_node);
			XMLString::release(&xmlch_vertex);
			XMLString::release(&xmlch_rel);
			XMLString::release(&xmlch_relend);
			XMLString::release(&xmlch_target);
			XMLString::release(&xmlch_role);
			XMLString::release(&xmlch_startorder);
			XMLString::release(&xmlch_attr);
			XMLString::release(&xmlch_name);
			XMLString::release(&xmlch_string);
			XMLString::release(&xmlch_label);
			XMLString::release(&xmlch_gts);
			XMLString::release(&xmlch_initial);
			XMLString::release(&xmlch_rule);
			XMLString::release(&xmlch_lhs);
			XMLString::release(&xmlch_rhs);
			XMLString::release(&xmlch_rulegraph);
			XMLString::release(&xmlch_mapping);
			XMLString::release(&xmlch_mapelem);
			XMLString::release(&xmlch_from);
			XMLString::release(&xmlch_to);
			XMLString::release(&xmlch_int);
			XMLString::release(&xmlch_quantification);
			XMLString::release(&xmlch_elem);
			XMLString::release(&xmlch_ref);

			// Terminate Xerces platform
			try
			{
				XMLPlatformUtils::Terminate();
			}
			catch(XMLException& e)
			{
				SafeXTString<char> message(XMLString::transcode(e.getMessage()));
				stringstream ss;
				ss << "Failed to terminate Xerces after use: " << message;
				throw XMLioException(ss.str());
			}

		}

		void GTXLWriterXerces::writeGXL(vector<Hypergraph_sp> const& graphs, path& filename) const {
			DOMImplementation* domImpl =  DOMImplementation::getImplementation();

			// generate DOM Document
			DOMDocument* doc = domImpl->createDocument();
			DOMElement* rootElement = doc->createElement(xmlch_gxl);
			doc->appendChild(rootElement);
			for(auto iter = graphs.cbegin(); iter != graphs.cend(); ++iter) {
				writeHypergraph(rootElement, **iter);
			}

			// output Document to file system
			SafeXRes<DOMLSSerializer> serializer(domImpl->createLSSerializer());
			SafeXRes<DOMLSOutput> outputDesc(domImpl->createLSOutput());
	    SafeXTString<XMLCh> filenameXML(XMLString::transcode(filename.c_str()));
	    LocalFileFormatTarget formatTarget(filenameXML.getContent());
	    outputDesc->setByteStream(&formatTarget);
	    SafeXTString<XMLCh> encoding(XMLString::transcode("ISO-8859-1"));
	    outputDesc->setEncoding(encoding.getContent());
	    serializer->getDomConfig()->setParameter(XMLUni::fgDOMXMLDeclaration, true);
	    serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	    serializer->write(doc, outputDesc.getContent());
	    formatTarget.flush(); // really needed?
		}

		void GTXLWriterXerces::writeHypergraph(xercesc::DOMElement* gxlRoot, Hypergraph const& graph) const {

			DOMElement* graphroot = writeHypergraph(gxlRoot, graph, graph.getID());

			// store graph ID if desired
			if(storeGraphIDs) {
				DOMDocument* doc = gxlRoot->getOwnerDocument();

				DOMElement* idChild = doc->createElement(xmlch_attr);
				idChild->setAttribute(xmlch_name,xmlch_id);
				graphroot->appendChild(idChild);

				DOMElement* idChild2 = doc->createElement(xmlch_int);
				idChild->appendChild(idChild2);
				SafeXTString<XMLCh> graphID(XMLString::transcode(boost::lexical_cast<string>(graph.getID()).c_str()));

				DOMText* idChildText = doc->createTextNode(graphID.getContent());
				idChild2->appendChild(idChildText);
			}

		}

		void GTXLWriterXerces::writeHypergraph(xercesc::DOMElement* gxlRoot, AnonHypergraph const& graph) const {
			writeHypergraph(gxlRoot,graph,ANONGRAPHID);
		}

		DOMElement* GTXLWriterXerces::writeHypergraph(xercesc::DOMElement* gxlRoot,
				AnonHypergraph const& graph,
				IDType const& graphID) const {

			DOMDocument* doc = gxlRoot->getOwnerDocument();

			// create graph root and fill attributes
			DOMElement* graphroot = doc->createElement(xmlch_graph);
			gxlRoot->appendChild(graphroot);
			string graphIDstr = generateGraphID(graphID);
			SafeXTString<XMLCh> graphIDXML(XMLString::transcode(graphIDstr.c_str()));
			graphroot->setAttribute(xmlch_id, graphIDXML.getContent());
			graphroot->setAttribute(xmlch_edgeids, xmlch_true);
			graphroot->setAttribute(xmlch_hypergraph, xmlch_true);
			graphroot->setAttribute(xmlch_edgemode, xmlch_undirected);

			// store name if existent
			if(graph.getName().compare("")) {
				DOMElement* nameChild = doc->createElement(xmlch_attr);
				nameChild->setAttribute(xmlch_name,xmlch_name);
				graphroot->appendChild(nameChild);
				DOMElement* nameChild2 = doc->createElement(xmlch_string);
				nameChild->appendChild(nameChild2);
				SafeXTString<XMLCh> graphName(XMLString::transcode(graph.getName().c_str()));
				DOMText* nameChildText = doc->createTextNode(graphName.getContent());
				nameChild2->appendChild(nameChildText);
			}

			// add a 'node' for each vertex
			for(auto iter = graph.cbeginVertices(); iter != graph.cendVertices(); ++iter) {
				DOMElement* vertex = doc->createElement(xmlch_node);
				graphroot->appendChild(vertex);
				string vertexID = generateVertexID(iter->first, graphID);
				SafeXTString<XMLCh> vertexIDXML(XMLString::transcode(vertexID.c_str()));
				vertex->setAttribute(xmlch_id,vertexIDXML.getContent());
			}

			// add a 'rel' for each edge
			for(auto iter = graph.cbeginEdges(); iter != graph.cendEdges(); ++iter) {
				DOMElement* edge = doc->createElement(xmlch_rel);
				graphroot->appendChild(edge);

				// write attributes
				string edgeID = generateEdgeID(iter->first, graphID);
				SafeXTString<XMLCh> edgeIDXML(XMLString::transcode(edgeID.c_str()));
				edge->setAttribute(xmlch_id,edgeIDXML.getContent());

				// write label
				DOMElement* edgeLabel = doc->createElement(xmlch_attr);
				edge->appendChild(edgeLabel);
				edgeLabel->setAttribute(xmlch_name,xmlch_label);
				DOMElement* edgeLabelStr = doc->createElement(xmlch_string);
				edgeLabel->appendChild(edgeLabelStr);
				SafeXTString<XMLCh> edgeLabelXML(XMLString::transcode(iter->second.getLabel().c_str()));
				DOMText* edgeLabelText = doc->createTextNode(edgeLabelXML.getContent());
				edgeLabelStr->appendChild(edgeLabelText);

				// write tentacles
				int startOrderCounter = 0;
				for(auto vIter = graph.getVerticesOfEdge(iter->first)->cbegin();
						vIter != graph.getVerticesOfEdge(iter->first)->cend(); ++vIter, ++startOrderCounter) {
					DOMElement* relend = doc->createElement(xmlch_relend);
					edge->appendChild(relend);
					string targetID = generateVertexID(*vIter, graphID);
					SafeXTString<XMLCh> targetIDXML(XMLString::transcode(targetID.c_str()));
					relend->setAttribute(xmlch_target,targetIDXML.getContent());
					relend->setAttribute(xmlch_role, xmlch_vertex);
					string startOrderStr = boost::lexical_cast<string>(startOrderCounter);
					SafeXTString<XMLCh> startOrderStrXML(XMLString::transcode(startOrderStr.c_str()));
					relend->setAttribute(xmlch_startorder,startOrderStrXML.getContent());
				}
			}

			return graphroot;
		}

		void GTXLWriterXerces::writeStdRule(xercesc::DOMElement* gtxlRoot, graphs::Rule const& rule) const {
			writeRule(gtxlRoot,rule,rule.getID(),nullptr,nullptr);
		}

		void GTXLWriterXerces::writeUQRule(xercesc::DOMElement* gtxlRoot, graphs::UQRule const& rule) const {
			vector<unordered_set<IDType>> leftUQids;
			vector<unordered_set<IDType>> rightUQids;
			UQRuleInstance_csp instance = rule.createOneInstance(&leftUQids,&rightUQids);
			writeRule(gtxlRoot,*instance,rule.getID(),&leftUQids,&rightUQids);
		}

		void GTXLWriterXerces::writeRule(xercesc::DOMElement* gtxlRoot,
				AnonRule const& rule,
				IDType id,
				vector<unordered_set<IDType>> const* leftUQids,
				vector<unordered_set<IDType>> const* rightUQids) const {

			DOMDocument* doc = gtxlRoot->getOwnerDocument();

			// generate rule and add attributes; generate a new ruleID only if the old was an empty string
			DOMElement* ruleXML = doc->createElement(xmlch_rule);
			gtxlRoot->appendChild(ruleXML);
			string ruleID = generateRuleID(id);
			SafeXTString<XMLCh> ruleIDXML(XMLString::transcode(ruleID.c_str()));
			ruleXML->setAttribute(xmlch_id,ruleIDXML.getContent());

			// store rule ID if desired
			if(storeRuleIDs) {
				DOMElement* idChild = doc->createElement(xmlch_attr);
				idChild->setAttribute(xmlch_name,xmlch_id);
				ruleXML->appendChild(idChild);
				DOMElement* idChild2 = doc->createElement(xmlch_int);
				idChild->appendChild(idChild2);
				SafeXTString<XMLCh> graphID(XMLString::transcode(boost::lexical_cast<string>(id).c_str()));
				DOMText* idChildText = doc->createTextNode(graphID.getContent());
				idChild2->appendChild(idChildText);
			}

			// store name if existent
			if(rule.getName().compare("")) {
				DOMElement* nameChild = doc->createElement(xmlch_attr);
				nameChild->setAttribute(xmlch_name,xmlch_name);
				ruleXML->appendChild(nameChild);
				DOMElement* nameChild2 = doc->createElement(xmlch_string);
				nameChild->appendChild(nameChild2);
				SafeXTString<XMLCh> ruleName(XMLString::transcode(rule.getName().c_str()));
				DOMText* nameChildText = doc->createTextNode(ruleName.getContent());
				nameChild2->appendChild(nameChildText);
			}

			// add left rule side
			DOMElement* lhs = doc->createElement(xmlch_lhs);
			ruleXML->appendChild(lhs);
			DOMElement* rulegraphL = doc->createElement(xmlch_rulegraph);
			lhs->appendChild(rulegraphL);
			string rulegraphLID = generateLeftRulegraphID(id);
			SafeXTString<XMLCh> rulegraphLIDXML(XMLString::transcode(rulegraphLID.c_str()));
			rulegraphL->setAttribute(xmlch_id,rulegraphLIDXML.getContent());
			writeHypergraph(rulegraphL,*rule.getLeft());

			if(leftUQids) {
				for(size_t i = 0; i < leftUQids->size(); ++i) {
					DOMElement* leftQuant = doc->createElement(xmlch_quantification);
					SafeXTString<XMLCh> quantID(XMLString::transcode(boost::lexical_cast<string>(i).c_str()));
					leftQuant->setAttribute(xmlch_id,quantID.getContent());
					for(auto elemID : leftUQids->at(i)) {
						DOMElement* elemNode = doc->createElement(xmlch_elem);
						if(rule.getLeft()->hasVertex(elemID)) {
							SafeXTString<XMLCh> elemIDcon(XMLString::transcode(generateVertexID(elemID,ANONGRAPHID).c_str()));
							elemNode->setAttribute(xmlch_ref,elemIDcon.getContent());
						} else {
							SafeXTString<XMLCh> elemIDcon(XMLString::transcode(generateEdgeID(elemID,ANONGRAPHID).c_str()));
							elemNode->setAttribute(xmlch_ref,elemIDcon.getContent());
						}
						leftQuant->appendChild(elemNode);
					}
					rulegraphL->appendChild(leftQuant);
				}
			}

			// add right rule side
			DOMElement* rhs = doc->createElement(xmlch_rhs);
			ruleXML->appendChild(rhs);
			DOMElement* rulegraphR = doc->createElement(xmlch_rulegraph);
			rhs->appendChild(rulegraphR);
			string rulegraphRID = generateRightRulegraphID(id);
			SafeXTString<XMLCh> rulegraphRIDXML(XMLString::transcode(rulegraphRID.c_str()));
			rulegraphR->setAttribute(xmlch_id,rulegraphRIDXML.getContent());
			writeHypergraph(rulegraphR,*rule.getRight());

			if(rightUQids) {
				for(size_t i = 0; i < rightUQids->size(); ++i) {
					DOMElement* rightQuant = doc->createElement(xmlch_quantification);
					SafeXTString<XMLCh> quantID(XMLString::transcode(boost::lexical_cast<string>(i).c_str()));
					rightQuant->setAttribute(xmlch_id,quantID.getContent());
					for(auto elemID : rightUQids->at(i)) {
						DOMElement* elemNode = doc->createElement(xmlch_elem);
						if(rule.getRight()->hasVertex(elemID)) {
							SafeXTString<XMLCh> elemIDcon(XMLString::transcode(generateVertexID(elemID,ANONGRAPHID).c_str()));
							elemNode->setAttribute(xmlch_ref,elemIDcon.getContent());
						} else {
							SafeXTString<XMLCh> elemIDcon(XMLString::transcode(generateEdgeID(elemID,ANONGRAPHID).c_str()));
							elemNode->setAttribute(xmlch_ref,elemIDcon.getContent());
						}
						rightQuant->appendChild(elemNode);
					}
					rulegraphR->appendChild(rightQuant);
				}
			}

			// add mapping
			DOMElement* mapping = doc->createElement(xmlch_mapping);
			ruleXML->appendChild(mapping);
			string mappingID = ruleID + (string)"map";
			SafeXTString<XMLCh> mappingIDXML(XMLString::transcode(mappingID.c_str()));
			mapping->setAttribute(xmlch_id, mappingIDXML.getContent());
			Mapping const& match = rule.getMapping();
			for(auto mIter = match.cbegin(); mIter != match.cend(); ++mIter) {
				DOMElement* mapelement = doc->createElement(xmlch_mapelem);
				mapping->appendChild(mapelement);
				string fromID;
				string toID;
				// if mapping maps nodes
				if(rule.getLeft()->hasVertex(mIter->first)) {
					fromID = generateVertexID(mIter->first,ANONGRAPHID);
					toID = generateVertexID(mIter->second,ANONGRAPHID);
				} else { //if mapping maps edges
					fromID = generateEdgeID(mIter->first,ANONGRAPHID);
					toID = generateEdgeID(mIter->second,ANONGRAPHID);
				}
				SafeXTString<XMLCh> fromIDXML(XMLString::transcode(fromID.c_str()));
				mapelement->setAttribute(xmlch_from,fromIDXML.getContent());
				SafeXTString<XMLCh> toIDXML(XMLString::transcode(toID.c_str()));
				mapelement->setAttribute(xmlch_to,toIDXML.getContent());
			}

		}

		string GTXLWriterXerces::generateGraphID(const IDType& graphID) const {
			return (string)"hg" + boost::lexical_cast<string>(graphID);
		}

		string GTXLWriterXerces::generateVertexID(const IDType& nodeID, const IDType& graphID) const {
			return (string)"hg" + boost::lexical_cast<string>(graphID) + (string)"n" + boost::lexical_cast<string>(nodeID);
		}

		string GTXLWriterXerces::generateEdgeID(const IDType& edgeID, const IDType& graphID) const {
			return (string)"hg" + boost::lexical_cast<string>(graphID) + (string)"e" + boost::lexical_cast<string>(edgeID);
		}

		string GTXLWriterXerces::generateRuleID(const IDType& ruleID) const {
			return (string)"r" + boost::lexical_cast<string>(ruleID);
		}

		string GTXLWriterXerces::generateLeftRulegraphID(const IDType& ruleID) const {
			return (string)"lhshg" + boost::lexical_cast<string>(ruleID);
		}

		string GTXLWriterXerces::generateRightRulegraphID(const IDType& ruleID) const {
			return (string)"rhshg" + boost::lexical_cast<string>(ruleID);
		}

		void GTXLWriterXerces::writeGTXL(GTS const& gts, path& filename) const {
			DOMImplementation* domImpl =  DOMImplementation::getImplementation();

			// generate document together with the root element
			DOMDocument* doc = domImpl->createDocument();
			DOMElement* rootElement = doc->createElement(xmlch_gts);
			doc->appendChild(rootElement);
			SafeXTString<XMLCh> gtsIDXML(XMLString::transcode(gts.getName().c_str()));
			rootElement->setAttribute(xmlch_id,gtsIDXML.getContent());

			// add all initial graphs
			DOMElement* initial = doc->createElement(xmlch_initial);
			rootElement->appendChild(initial);
			for(auto iter = gts.getInitialGraphs().cbegin(); iter != gts.getInitialGraphs().cend(); ++iter) {
				writeHypergraph(initial, **iter);
			}

			// add all standard rules
			for(auto iter = gts.getStdRules().cbegin(); iter != gts.getStdRules().cend(); ++iter) {
				writeStdRule(rootElement,*(iter->second));
			}

			// add all UQ rules
			for(auto iter = gts.getUQRules().cbegin(); iter != gts.getUQRules().cend(); ++iter) {
				writeUQRule(rootElement,*(iter->second));
			}

			// output Document to file system
			SafeXRes<DOMLSSerializer> serializer(domImpl->createLSSerializer());
			SafeXRes<DOMLSOutput> outputDesc(domImpl->createLSOutput());
	    SafeXTString<XMLCh> filenameXML(XMLString::transcode(filename.c_str()));
	    LocalFileFormatTarget formatTarget(filenameXML.getContent());
	    outputDesc->setByteStream(&formatTarget);
	    SafeXTString<XMLCh> encoding(XMLString::transcode("ISO-8859-1"));
	    outputDesc->setEncoding(encoding.getContent());
	    serializer->getDomConfig()->setParameter(XMLUni::fgDOMXMLDeclaration, true);
	    serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	    serializer->write(doc, outputDesc.getContent());
	    formatTarget.flush(); // really needed?
		}

	} /* namespace io */
} /* namespace uncover */
