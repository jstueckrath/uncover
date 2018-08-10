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

#include "GTXLReaderXerces.h"
#include "XMLioException.h"
#include "../logging/message_logging.h"
#include <boost/lexical_cast.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
//#include <xercesc/validators/DTD/DTDValidator.hpp>
#include "XercesLoggingErrorHandler.h"
#include "xercesc_helpers.h"
#include "../basic_types/standard_operators.h"

#include <sstream>

using namespace xercesc;
using namespace std;
using namespace uncover::logging;
using namespace uncover::graphs;

namespace uncover {
	namespace io {

		GTXLReaderXerces::GTXLReaderXerces() {
			// Initialize Xerces platform
			try {
				XMLPlatformUtils::Initialize();
			}
			catch(XMLException& e) {
				throw XMLioException("Failed to initialize Xerces!" + generateXerxesErrorMessage(e));
			}

			// pre-transcode some strings to XMLCH* for later use
			xmlch_gxl = XMLString::transcode("gxl");
			xmlch_edgeids = XMLString::transcode("edgeids");
			xmlch_true = XMLString::transcode("true");
			xmlch_hypergraph = XMLString::transcode("hypergraph");
			xmlch_text = XMLString::transcode("#text");
			xmlch_id = XMLString::transcode("id");
			xmlch_rel = XMLString::transcode("rel");
			xmlch_relend = XMLString::transcode("relend");
			xmlch_target = XMLString::transcode("target");
			xmlch_role = XMLString::transcode("role");
			xmlch_startorder = XMLString::transcode("startorder");
			xmlch_attr = XMLString::transcode("attr");
			xmlch_name = XMLString::transcode("name");
			xmlch_string = XMLString::transcode("string");
			xmlch_edgemode = XMLString::transcode("edgemode");
			xmlch_node = XMLString::transcode("node");
			xmlch_edge = XMLString::transcode("edge");
			xmlch_label = XMLString::transcode("label");
			xmlch_graph = XMLString::transcode("graph");
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
			xmlch_type = XMLString::transcode("type");
			xmlch_transformationunit = XMLString::transcode("transformationunit");
			xmlch_int = XMLString::transcode("int");
			xmlch_quantification = XMLString::transcode("quantification");
			xmlch_elem = XMLString::transcode("elem");
			xmlch_ref = XMLString::transcode("ref");
		}

		GTXLReaderXerces::~GTXLReaderXerces() {
			// delete pre-transcoded strings
			XMLString::release(&xmlch_gxl);
			XMLString::release(&xmlch_edgeids);
			XMLString::release(&xmlch_true);
			XMLString::release(&xmlch_hypergraph);
			XMLString::release(&xmlch_text);
			XMLString::release(&xmlch_id);
			XMLString::release(&xmlch_rel);
			XMLString::release(&xmlch_relend);
			XMLString::release(&xmlch_target);
			XMLString::release(&xmlch_role);
			XMLString::release(&xmlch_startorder);
			XMLString::release(&xmlch_attr);
			XMLString::release(&xmlch_name);
			XMLString::release(&xmlch_string);
			XMLString::release(&xmlch_edgemode);
			XMLString::release(&xmlch_node);
			XMLString::release(&xmlch_edge);
			XMLString::release(&xmlch_label);
			XMLString::release(&xmlch_graph);
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
			XMLString::release(&xmlch_type);
			XMLString::release(&xmlch_transformationunit);
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
				throw XMLioException("Failed to terminate Xerces after use." + generateXerxesErrorMessage(e));
			}
		}

		shared_ptr<vector<Hypergraph_sp>> GTXLReaderXerces::readGXL(string filename,
				unordered_map<IDType,IDType>* oldIDs) const {

			// parse the file and log the errors
			XercesLoggingErrorHandler errorHandler;
			XercesDOMParser gxlParser;
			// NOTE: In the case of exceptions I assume the errorHandler is destroyed after gxlParser or gxlParser does not
			// call errorHandler when it is destroyed.
			gxlParser.setErrorHandler(&errorHandler);
			gxlParser.parse(filename.c_str());

			// abort the import if the document cannot be loaded or the root node is wrong
			DOMDocument* xmlDoc = gxlParser.getDocument();
			if(xmlDoc == NULL) {
				throw XMLioException("Failed to parse Document. Unknown error occurred. Import aborted.");
			}
			DOMElement* elementRoot = xmlDoc->getDocumentElement();

			if(!isEqualIgnoringCase2(xmlch_gxl, elementRoot->getNodeName())) {
				SafeXTString<char> tmpChar(XMLString::transcode(elementRoot->getNodeName()));
				throw XMLioException("The XML root element is '" + (string)tmpChar.getContent() + "' but should be 'gxl'. Import aborted.");
			}

			// check all child nodes for graph nodes
			shared_ptr<vector<Hypergraph_sp>> result = make_shared<vector<Hypergraph_sp>>();
			for(DOMElement* element = elementRoot->getFirstElementChild(); element != NULL; element = element->getNextElementSibling()) {
				if(isEqualIgnoringCase2(xmlch_graph, element->getNodeName())) {
					result->push_back(readHypergraph(element, oldIDs).first);
				} else if(showWarnings) {
					SafeXTString<char> tmpErrorChar(XMLString::transcode(element->getNodeName()));
					logger.newWarning() << "'gxl' node contains an illegal child '" << tmpErrorChar << "', which was ignored." << endLogMessage;
				} // else ignore
			}

			return result;
		}

		pair<AnonHypergraph_sp,unordered_map<string, IDType>> GTXLReaderXerces::readAnonHypergraph(
				xercesc::DOMElement* graphroot) const {
			pair<AnonHypergraph_sp,unordered_map<string, IDType>> resultPair(make_shared<AnonHypergraph>(),unordered_map<string,IDType>());
			readAnonHypergraph(graphroot,nullptr,*resultPair.first,0,resultPair.second);
			return resultPair;
		}

		void GTXLReaderXerces::readAnonHypergraph(xercesc::DOMElement* graphroot,
				unordered_map<IDType,IDType>* oldIDs,
				graphs::AnonHypergraph& graph,
				IDType const& graphID,
				unordered_map<string,IDType>& strToID) const {

			XMLSize_t attrNum = 0;
			DOMAttr* oneAttr = nullptr;

			// check if the graph has edge IDs
			oneAttr = graphroot->getAttributeNode(xmlch_edgeids);
			if(oneAttr == NULL || !isEqualIgnoringCase2(xmlch_true, oneAttr->getValue())) {
				throw XMLioException("At least one given graph has no edge IDs. Aborting Import!");
			} else {
				attrNum++;
			}

			if(showWarnings) {

				// check the id of the graph
				oneAttr = graphroot->getAttributeNode(xmlch_id);
				if(oneAttr != nullptr) {
					attrNum++;
				} else {
					logger.newWarning() << "The read graph has no id (will be ignored)!" << endLogMessage;
				}

				// ignore whether graph is a hypergraph
				oneAttr = graphroot->getAttributeNode(xmlch_hypergraph);
				if(oneAttr != nullptr) {
				} else {
					attrNum++;
				}

				// ignore edgemode attribute
				oneAttr = graphroot->getAttributeNode(xmlch_edgemode);
				if(oneAttr != NULL) {
					attrNum++;
				}

				// warn if graph has too many attributes
				if(graphroot->getAttributes()->getLength() > attrNum) {
					logger.newWarning() << "Some of the graphs attributes were ignored." << endLogMessage;
				}
			}

			// maps the XML IDs to IDTypes
			unordered_map<string, IDType> nodeIDList;
			// stores the pointer to all rel and edge XML nodes, for postponed reading
			vector<DOMElement*> rels;
			vector<DOMElement*> edges;

			// traverse all child elements of the graph root
			for(DOMElement* curElem = graphroot->getFirstElementChild(); curElem != nullptr; curElem = curElem->getNextElementSibling()) {

				// the element is a node
				if(isEqualIgnoringCase2(xmlch_node, curElem->getNodeName())) {
					DOMAttr* nodeAttr = curElem->getAttributeNode(xmlch_id);
					if(nodeAttr == NULL) {
						throw XMLioException("At least one 'node' node of a given graph has no ID.");
					} else {
						IDType id = graph.addVertex();
						SafeXTString<char> tmpChar(XMLString::transcode(nodeAttr->getNodeValue()));
						if(nodeIDList.count((string)tmpChar.getContent()) != 0) {
							throw XMLioException("Two nodes of the graph have the same ID '" + (string)tmpChar.getContent() + "'.");
						}
						nodeIDList[(string)tmpChar.getContent()] = id;
						strToID[(string)tmpChar.getContent()] = id;
						if(showWarnings && (curElem->getAttributes()->getLength() != 1 || curElem->getChildElementCount() != 0)) {
							logger.newWarning() << "Some attributes and/or children of the node '" <<  tmpChar << "' where ignored." << endLogMessage;
						}
					}

				// the element is an edge (rel node) and will be stored for postponed reading
				} else if (isEqualIgnoringCase2(xmlch_rel, curElem->getNodeName())) {
					rels.push_back(curElem);

				// the element is an edge (edge node) and will be stored for postponed reading
				} else if (isEqualIgnoringCase2(xmlch_edge, curElem->getNodeName())) {
					edges.push_back(curElem);

				// the element is an attr XML node and contains the name of the graph
				} else if (isEqualIgnoringCase2(xmlch_attr, curElem->getNodeName())) {

					DOMAttr* name = curElem->getAttributeNode(xmlch_name);
					if(name != nullptr && isEqualIgnoringCase2(xmlch_name, name->getValue())) {
						DOMElement* attrChild = curElem->getFirstElementChild();
						if(attrChild && !XMLString::compareString(attrChild->getNodeName(), xmlch_string)) {
							SafeXTString<char> tmpChar(XMLString::transcode(attrChild->getTextContent()));
							graph.setName(tmpChar.getContent());
						} else if(showWarnings) {
								logger.newError() << "Tried to read name node (child of attr) from graph, "
										"but failed due to missing or badly formed attributes." << endLogMessage;
						}
					} else if (name != nullptr && isEqualIgnoringCase2(xmlch_id, name->getValue())){
						// ignore id if reading is not necessary
						if(oldIDs == nullptr) {
							continue;
						}
						DOMElement* attrChild = curElem->getFirstElementChild();
						if(attrChild && !XMLString::compareString(attrChild->getNodeName(), xmlch_int)) {
							SafeXTString<char> tmpChar(XMLString::transcode(attrChild->getTextContent()));
							try {
								(*oldIDs)[graphID] = boost::lexical_cast<IDType>(tmpChar.getContent());
							} catch( const boost::bad_lexical_cast & ) {
								logger.newError() << "Tried to read id node (child of attr) from graph, but failed due to invalid id value." << endLogMessage;
							}

						} else if(showWarnings) {
								logger.newError() << "Tried to read id node (child of attr) from graph, "
										"but failed due to missing or badly formed attributes." << endLogMessage;
						}
					} else if (showWarnings) {
						logger.newError() << "Tried to read attr node from graph, but no or unknown name given." << endLogMessage;
					}

				// all non-read elements
				} else if(showWarnings) {
					SafeXTString<char> tmpChar(XMLString::transcode(curElem->getNodeName()));
					logger.newWarning() << "An element with name '" << tmpChar.getContent() << "' of a graph was ignored." << endLogMessage;
				}

			}

			// read all 'edge' nodes (stored as hyperedges); such edges are assumed to be directed
			for(vector<DOMElement*>::iterator it = edges.begin(); it != edges.end(); ++it) {
				DOMAttr* edgeID = (*it)->getAttributeNode(xmlch_id);
				if(edgeID == nullptr) {
					throw XMLioException("At least one edge (edge node) has no ID!");
				}

				// check if edge id was already used
				SafeXTString<char> edgeXMLid(XMLString::transcode(edgeID->getNodeValue()));
				string edgeXMLidStr = (string)edgeXMLid.getContent();
				if(strToID.count(edgeXMLidStr) != 0) {
					throw XMLioException("The edge '" + edgeXMLidStr + "' has the same ID as another edge or node.");
				}

				vector<IDType> attachedNodes;

				// check and read from node
				DOMAttr* fromNode = (*it)->getAttributeNode(xmlch_from);
				if (fromNode == nullptr) {
					throw XMLioException("The edge '" + edgeXMLidStr + "' has no from attribute.");
				} else {
					SafeXTString<char> fromNodeValue(XMLString::transcode(fromNode->getNodeValue()));
					if(nodeIDList.count((string)fromNodeValue.getContent()) == 0) {
						throw XMLioException("The edge '" + edgeXMLidStr + "' is attached to a non existing node ('from' value).");
					} else {
						attachedNodes.push_back(nodeIDList[(string)fromNodeValue.getContent()]);
					}
				}

				// check and read to node
				DOMAttr* toNode = (*it)->getAttributeNode(xmlch_to);
				if (toNode == nullptr) {
					throw XMLioException("The edge '" + edgeXMLidStr + "' has no to attribute.");
				} else {
					SafeXTString<char> toNodeValue(XMLString::transcode(toNode->getNodeValue()));
					if(nodeIDList.count((string)toNodeValue.getContent()) == 0) {
						throw XMLioException("The edge '" + edgeXMLidStr + "' is attached to a non existing node ('to' value).");
					} else {
						attachedNodes.push_back(nodeIDList[(string)toNodeValue.getContent()]);
					}
				}

				// check for label
				string edgeLabel("");
				for(DOMElement* edgeAttr = (*it)->getFirstElementChild(); edgeAttr != nullptr; edgeAttr = edgeAttr->getNextElementSibling()) {
					if(isEqualIgnoringCase2(xmlch_attr, edgeAttr->getNodeName())) {
						DOMAttr* attrAttr = edgeAttr->getAttributeNode(xmlch_name);
						if(attrAttr == NULL || !isEqualIgnoringCase2(xmlch_label, attrAttr->getNodeValue())) {
							if(showWarnings) {
								logger.newWarning() << "The edge '" << edgeXMLidStr << "' has a non-label attr node, which was ignored." << endLogMessage;
							}
							continue;
						}
						int strChildCount = 0;
						for(DOMElement* strChild = edgeAttr->getFirstElementChild(); strChild != nullptr; strChild = strChild->getNextElementSibling()) {
							if(isEqualIgnoringCase2(xmlch_string, strChild->getNodeName())) {
								if(strChildCount) {
									edgeLabel = "";
									if(showWarnings) {
										logger.newWarning() << "The edge '" << edgeXMLidStr << "' has too many labels, '' assumed." << endLogMessage;
									}
								} else {
									DOMNodeList* stringChildContent = strChild->getChildNodes();
									if(stringChildContent->getLength() == 1 && stringChildContent->item(0)->getNodeType() == DOMNode::NodeType::TEXT_NODE) {
										SafeXTString<char> tmpChar(XMLString::transcode(stringChildContent->item(0)->getNodeValue()));
										edgeLabel = (string)tmpChar.getContent();
									} else if(showWarnings) {
										logger.newWarning() << "The label of edge '" << edgeXMLidStr << "' is empty or too large." << endLogMessage;
									}
									strChildCount++;
								}
							}
						}
					}
				}

				strToID[edgeXMLidStr] = graph.addEdge(edgeLabel, attachedNodes);
			}

			// read all 'rel' nodes (hyperedges)
			for(vector<DOMElement*>::iterator it = rels.begin(); it != rels.end(); ++it) {
				DOMAttr* edgeAttr = (*it)->getAttributeNode(xmlch_id);
				if(edgeAttr == NULL) {
					throw XMLioException("At least one 'rel' node of a given graph has no ID.");
				} else {
					// check if edge ID is already in use
					SafeXTString<char> edgeXMLid(XMLString::transcode(edgeAttr->getNodeValue()));
					if(strToID.count((string)edgeXMLid.getContent()) != 0) {
						throw XMLioException("The edge '" + (string)edgeXMLid.getContent() + "' has the same ID as another edge or node.");
					}

					// check if the edge has more attributes than just 'id'
					if(showWarnings && (*it)->getAttributes()->getLength() != 1) {
						logger.newWarning() <<  "Some attributes of the edge '" << edgeXMLid << "' where ignored." << endLogMessage;
					}

					string edgeLabel("");
					unordered_map<unsigned int, IDType> orderToVertex; // maps startorder to vertex ID
					for(DOMElement* child = (*it)->getFirstElementChild(); child != nullptr; child = child->getNextElementSibling()) {

						// process the tentacles of the edge
						if(isEqualIgnoringCase2(xmlch_relend, child->getNodeName())) {
							if(showWarnings && child->getFirstElementChild() != nullptr) {
								logger.newWarning() << "The children of a 'relend' of edge '" << edgeXMLid << "' where ignored." << endLogMessage;
							}
							DOMAttr* targetAttr = child->getAttributeNode(xmlch_target);
							DOMAttr* startAttr = child->getAttributeNode(xmlch_startorder);
							if(targetAttr == NULL || startAttr == NULL) {
								throw XMLioException("A 'relend' node of edge '" + (string)edgeXMLid.getContent() +  "' does not specify a target or startorder.");
							} else {
								SafeXTString<char> nodeID(XMLString::transcode(targetAttr->getNodeValue()));
								if(nodeIDList.count((string)nodeID.getContent()) == 0) {
									throw XMLioException("The edge '" + (string)edgeXMLid.getContent() + "' is connected to a non existing vertex.");
								}
								SafeXTString<char> startorderStr(XMLString::transcode(startAttr->getNodeValue()));
								try {
										unsigned int startorder = boost::lexical_cast<unsigned int>(startorderStr.getContent());
										if(orderToVertex.count(startorder) != 0) {
											throw XMLioException("Multiple vertices of edge '" + (string)edgeXMLid.getContent() + "' have the same startorder.");
										}
										orderToVertex[startorder] = nodeIDList[(string)nodeID.getContent()];
								}
								catch( const boost::bad_lexical_cast & )
								{
									throw XMLioException("A 'relend' node of edge '" + (string)edgeXMLid.getContent() +  "' has a invalid startorder.");
								}
							}
						} // check attr nodes for label
						else if(isEqualIgnoringCase2(xmlch_attr, child->getNodeName())) {
							DOMAttr* attrAttr = child->getAttributeNode(xmlch_name);
							if(attrAttr == NULL || !isEqualIgnoringCase2(xmlch_label, attrAttr->getNodeValue())) {
								if(showWarnings) {
									logger.newWarning() << "The edge '" << edgeXMLid << "' has a non-label attr node, which was ignored." << endLogMessage;
								}
								continue;
							}
							int strChildCount = 0;
							for(DOMElement* strChild = child->getFirstElementChild(); strChild != nullptr; strChild = strChild->getNextElementSibling()) {
								if(isEqualIgnoringCase2(xmlch_string, strChild->getNodeName())) {
									if(strChildCount) {
										edgeLabel = "";
										if(showWarnings) {
											logger.newWarning() << "The edge '" << edgeXMLid << "' has too many labels, '' assumed." << endLogMessage;
										}
									} else {
										DOMNodeList* stringChildContent = strChild->getChildNodes();
										if(stringChildContent->getLength() == 1 && stringChildContent->item(0)->getNodeType() == DOMNode::NodeType::TEXT_NODE) {
											SafeXTString<char> tmpChar(XMLString::transcode(stringChildContent->item(0)->getNodeValue()));
											edgeLabel = (string)tmpChar.getContent();
										} else if(showWarnings) {
											logger.newWarning() << "The label of edge '" << edgeXMLid << "' is empty or too large." << endLogMessage;
										}
										strChildCount++;
									}
								} // else ignore
							}
						} // warn if nodes were not processed
						else if(showWarnings) {
							SafeXTString<char> tmpChar(XMLString::transcode(child->getNodeName()));
							logger.newWarning() << "A child '" << tmpChar << "' of edge '" << edgeXMLid << "' was ignored." << endLogMessage;
						}
					}

					// fill the vertexlist, add the edge to the graph and store the id
					vector<IDType> attachedVertices;
					for(unsigned int i = 0; i < orderToVertex.size(); i++) {
						if(orderToVertex.count(i) == 0) {
							throw XMLioException("The edge '" + (string)edgeXMLid.getContent() + "' is missing a startorder.");
						}
						attachedVertices.push_back(orderToVertex[i]);
					}
					strToID[(string)edgeXMLid.getContent()] = graph.addEdge(edgeLabel, attachedVertices);
				}
			}

		}

		pair<Hypergraph_sp, unordered_map<string, IDType>> GTXLReaderXerces::readHypergraph(DOMElement* graphroot,
				unordered_map<IDType,IDType>* oldIDs) const {
			pair<Hypergraph_sp,unordered_map<string, IDType>> resultPair(make_shared<Hypergraph>(),unordered_map<string,IDType>());
			readAnonHypergraph(graphroot,oldIDs,*resultPair.first,resultPair.first->getID(),resultPair.second);
			return resultPair;
		}

		GTS_sp GTXLReaderXerces::readGTXL(string filename,
				unordered_map<IDType,IDType>* oldRuleIDs,
				unordered_map<IDType,IDType>* oldGraphIDs) const {

			// parse the file and log the errors
			XercesLoggingErrorHandler errorHandler;
			XercesDOMParser gxlParser;

			// NOTE: In the case of exceptions I assume the errorHandler is destroyed after gxlParser or gxlParser does not
			// call errorHandler when it is destroyed.
			gxlParser.setErrorHandler(&errorHandler);
			gxlParser.parse(filename.c_str());

			// abort the import if the document could not be loaded
			DOMDocument* xmlDoc = gxlParser.getDocument();
			if(xmlDoc == nullptr) {
				throw XMLioException("Failed to parse Document. Import aborted.");
			}

			// abort the import if the root element could not be loaded
			DOMElement* elementRoot = xmlDoc->getDocumentElement();
			if(elementRoot == nullptr) {
				throw XMLioException("XML root element non-existent or unreadable.");
			}

			// abort if root element is wrong
			if(!isEqualIgnoringCase2(xmlch_gts, elementRoot->getNodeName())) {
				SafeXTString<char> tmpChar(XMLString::transcode(elementRoot->getNodeName()));
				throw XMLioException("The XML root element is '" + (string)tmpChar.getContent() + "' but should be 'gts'. Import aborted.");
			}

			GTS_sp resultGTS;

			// load the id (name) of the GTS
			string name("");
			DOMAttr* gtsID = elementRoot->getAttributeNode(xmlch_id);
			if(gtsID != nullptr) {
				SafeXTString<char> tempChar(XMLString::transcode(gtsID->getNodeValue()));
				resultGTS = make_shared<GTS>((string)tempChar.getContent());
			} else if(showWarnings) {
				logger.newWarning() << "The 'gts' has no 'id' attribute, '' used." << endLogMessage;
				resultGTS = make_shared<GTS>();
			}

			int initialCount(0);
			for(DOMElement* child = elementRoot->getFirstElementChild(); child != nullptr; child = child->getNextElementSibling()) {
				// there should be exactly one child containing the initial graphs
				if(isEqualIgnoringCase2(xmlch_initial, child->getNodeName())) {
					initialCount++;

					for(DOMElement* graphChild = child->getFirstElementChild(); graphChild != nullptr; graphChild = graphChild->getNextElementSibling()) {
						if(isEqualIgnoringCase2(xmlch_graph, graphChild->getNodeName())) {
							resultGTS->addInitialGraph(readHypergraph(graphChild,oldGraphIDs).first);
						} else if(showWarnings) {
							logger.newWarning() << "An initial node contained a non-graph child." << endLogMessage;
						}
					}
				// child is a rule
				} else if(isEqualIgnoringCase2(xmlch_rule, child->getNodeName())) {

					GTXLReaderXerces::RuleTriple triple = readRule(child,oldRuleIDs);
					GTXLReaderXerces::QuantMap& leftQuant = get<1>(triple);
					GTXLReaderXerces::QuantMap& rightQuant = get<2>(triple);

					if(leftQuant.size() == 0 && rightQuant.size() == 0) {

						// a rule without quantifications is added as standard rule
						resultGTS->addStdRule(get<0>(triple));

					} else {

						// a rule with at least one quantification is added ad universally quantified rule
						checkConsistency(*get<0>(triple), leftQuant, rightQuant);
						UQRule_sp newRule = make_shared<UQRule>(*get<0>(triple));
						for(GTXLReaderXerces::QuantMap::const_iterator iter = leftQuant.cbegin(); iter != leftQuant.cend(); ++iter) {
							convertToQuant(*newRule, iter->second, rightQuant[iter->first]);
						}
						resultGTS->addUQRule(newRule);

					}

				} else if(showWarnings) {
					SafeXTString<char> tmpChar(XMLString::transcode(child->getNodeName()));
					logger.newWarning() << "The 'gts' node contained a child '" << tmpChar.getContent() << "' which was ignored." << endLogMessage;
				}
			}

			return resultGTS;
		}

		void GTXLReaderXerces::convertToQuant(UQRule& rule,
				unordered_set<IDType> const& leftQuant,
				unordered_set<IDType> const& rightQuant) const {

			AnonHypergraph& rRight = *rule.getMainRule().getRight();
			AnonHypergraph& rLeft = *rule.getMainRule().getLeft();
			Mapping& rMap = rule.getMainRule().getMapping();

			AnonRule qRule;
			AnonHypergraph& qRight = *qRule.getRight();
			AnonHypergraph& qLeft = *qRule.getLeft();
			Mapping& qMap = qRule.getMapping();
			Mapping quant2rule;

			// fill right quantification graph and shrink right rule graph
			Mapping r2qRightMap;
			{
				// stores edge ids for handling later
				vector<IDType> storedEdges;
				// vertices cannot be deleted before edges where deleted
				vector<IDType> deletedVertices;

				// add a vertex for each quantified vertex and store edge ids
				for(unordered_set<IDType>::const_iterator iter = rightQuant.begin(); iter != rightQuant.end(); ++iter) {
					if(rRight.hasEdge(*iter)) {
						storedEdges.push_back(*iter);
					} else if(rRight.hasVertex(*iter)) {
						r2qRightMap[*iter] = qRight.addVertex();
						deletedVertices.push_back(*iter);
					} else {
						// cannot occur
						throw XMLioException("An element of a quantification was neither an edge nor a vertex of the graph.");
					}
				}

				// add an edge for each quantified edge, possibly adding new vertices (which belong to no quantification)
				for(vector<IDType>::const_iterator iter = storedEdges.cbegin(); iter != storedEdges.cend(); ++iter) {
					vector<IDType> newAttVertices;
					vector<IDType> const* attVertices = rRight.getVerticesOfEdge(*iter);
					for(vector<IDType>::const_iterator attIt = attVertices->begin(); attIt != attVertices->end(); ++attIt) {
						// attached vertex belongs to no quantification
						if(!r2qRightMap.hasSrc(*attIt)) {
							r2qRightMap[*attIt] = qRight.addVertex();
						}
						newAttVertices.push_back(r2qRightMap[*attIt]);
					}
					r2qRightMap[*iter] = qRight.addEdge(rRight.getEdge(*iter)->getLabel(), newAttVertices);
					rRight.deleteEdge(*iter);
				}

				// delete quantified vertices from main rule
				for(vector<IDType>::const_iterator iter = deletedVertices.cbegin(); iter != deletedVertices.cend(); ++iter) {
					rRight.deleteVertex(*iter);
				}

			}

			// fill left quantification graph and shrink left rule graph
			// also: fill quantification mapping and shrink main rule Mapping
			Mapping r2qLeftMap;
			{
				// stores edge ids for handling later
				vector<IDType> storedEdges;
				// vertices cannot be deleted before edges where deleted
				vector<IDType> deletedVertices;

				// add a vertex for each quantified vertex and store edge ids
				for(unordered_set<IDType>::const_iterator iter = leftQuant.begin(); iter != leftQuant.end(); ++iter) {
					if(rLeft.hasEdge(*iter)) {
						storedEdges.push_back(*iter);
					} else if(rLeft.hasVertex(*iter)) {
						r2qLeftMap[*iter] = qLeft.addVertex();
						// update quantification mapping
						if(rMap.hasSrc(*iter)) {
							qMap[r2qLeftMap[*iter]] = r2qRightMap.getTar(rMap.getTar(*iter));
						} // image is undefined
						deletedVertices.push_back(*iter);
					} else {
						// cannot occur
						throw XMLioException("An element of a quantification was neither an edge nor a vertex of the graph.");
					}
				}

				// add an edge for each quantified edge, possibly adding new vertices (which belong to no quantification)
				for(vector<IDType>::const_iterator iter = storedEdges.cbegin(); iter != storedEdges.cend(); ++iter) {
					vector<IDType> newAttVertices;
					vector<IDType> const* attVertices = rLeft.getVerticesOfEdge(*iter);
					for(vector<IDType>::const_iterator attIt = attVertices->begin(); attIt != attVertices->end(); ++attIt) {
						// attached vertex belongs to no quantification
						if(!r2qLeftMap.hasSrc(*attIt)) {
							r2qLeftMap[*attIt] = qLeft.addVertex();
							quant2rule[r2qLeftMap[*attIt]] = *attIt;
							if(rMap.hasSrc(*attIt)) {
								qMap[r2qLeftMap[*attIt]] = r2qRightMap.getTar(rMap.getTar(*attIt));
							}
						}
						newAttVertices.push_back(r2qLeftMap[*attIt]);
					}
					r2qLeftMap[*iter] = qLeft.addEdge(rLeft.getEdge(*iter)->getLabel(), newAttVertices);
					if(rMap.hasSrc(*iter)) {
						qMap[r2qLeftMap[*iter]] = r2qRightMap.getTar(rMap.getTar(*iter));
					} // image is undefined
					rLeft.deleteEdge(*iter);
					rMap.delSrc(*iter);
				}

				// delete quantified vertices from main rule
				for(vector<IDType>::const_iterator iter = deletedVertices.cbegin(); iter != deletedVertices.cend(); ++iter) {
					rLeft.deleteVertex(*iter);
					rMap.delSrc(*iter);
				}

			}

			rule.addQuantification(qRule, quant2rule);
		}

		tuple<Rule_sp, GTXLReaderXerces::QuantMap, GTXLReaderXerces::QuantMap> GTXLReaderXerces::readRule(DOMElement* ruleroot,
				unordered_map<IDType,IDType>* oldIDs) const{

			// define variables for all parameters read in the following
			string ruleName("");
			pair<AnonHypergraph_sp, unordered_map<string,IDType>> leftSideGraph;
			GTXLReaderXerces::QuantMap leftQuantMap;
			pair<AnonHypergraph_sp, unordered_map<string,IDType>> rightSideGraph;
			GTXLReaderXerces::QuantMap rightQuantMap;
			Mapping mapping;
			IDType oldID = 0;

			// process all child elements of the ruleroot
			for(DOMElement* curElem = ruleroot->getFirstElementChild(); curElem != nullptr; curElem = curElem->getNextElementSibling()) {

				// the element is an attr XML node and contains the name of the rule
				if(isEqualIgnoringCase2(xmlch_attr, curElem->getNodeName())) {

					DOMAttr* name = curElem->getAttributeNode(xmlch_name);
					if(name != nullptr && isEqualIgnoringCase2(xmlch_name, name->getValue())) {
						DOMElement* attrChild = curElem->getFirstElementChild();
						if(attrChild && !XMLString::compareString(attrChild->getNodeName(), xmlch_string)) {
							SafeXTString<char> tmpChar(XMLString::transcode(attrChild->getTextContent()));
							ruleName = tmpChar.getContent();
						} else if(showWarnings) {
								logger.newError() << "Tried to read name node (child of attr) from rule, "
										"but failed due to missing or badly formed attributes." << endLogMessage;
						}
					} else if (name != nullptr && isEqualIgnoringCase2(xmlch_id, name->getValue())){

						// ignore id if reading is not necessary
						if(oldIDs == nullptr) {
							continue;
						}
						DOMElement* attrChild = curElem->getFirstElementChild();
						if(attrChild && !XMLString::compareString(attrChild->getNodeName(), xmlch_int)) {
							SafeXTString<char> tmpChar(XMLString::transcode(attrChild->getTextContent()));
							try {
								oldID = boost::lexical_cast<IDType>(tmpChar.getContent());
							} catch( const boost::bad_lexical_cast & ) {
								logger.newError() << "Tried to read id node (child of attr) from graph, but failed due to invalid id value." << endLogMessage;
							}

						} else if(showWarnings) {
								logger.newError() << "Tried to read id node (child of attr) from graph, "
										"but failed due to missing or badly formed attributes." << endLogMessage;
						}
					}	else if (showWarnings) {
						logger.newError() << "Tried to read attr node from rule, but no or unknown name given." << endLogMessage;
					}

				// the element is the left rule side
				} else if (isEqualIgnoringCase2(xmlch_lhs, curElem->getNodeName())) {
					for(DOMElement* ruleGraph = curElem->getFirstElementChild(); ruleGraph != nullptr; ruleGraph = ruleGraph->getNextElementSibling()) {
						if(isEqualIgnoringCase2(xmlch_rulegraph, ruleGraph->getNodeName())) {
							for(DOMElement* graphInRuleGraph = ruleGraph->getFirstElementChild(); graphInRuleGraph != nullptr; graphInRuleGraph = graphInRuleGraph->getNextElementSibling()) {
								if(isEqualIgnoringCase2(xmlch_graph, graphInRuleGraph->getNodeName())) {
									leftSideGraph = readAnonHypergraph(graphInRuleGraph);
								} else if(isEqualIgnoringCase2(xmlch_quantification, graphInRuleGraph->getNodeName())) {
									readQuantMap(graphInRuleGraph, leftQuantMap, leftSideGraph.second);
								}
							}
						}
					}

				// the element is the right rule side
				} else if (isEqualIgnoringCase2(xmlch_rhs, curElem->getNodeName())) {
					for(DOMElement* ruleGraph = curElem->getFirstElementChild(); ruleGraph != nullptr; ruleGraph = ruleGraph->getNextElementSibling()) {
						if(isEqualIgnoringCase2(xmlch_rulegraph, ruleGraph->getNodeName())) {
							for(DOMElement* graphInRuleGraph = ruleGraph->getFirstElementChild(); graphInRuleGraph != nullptr; graphInRuleGraph = graphInRuleGraph->getNextElementSibling()) {
								if(isEqualIgnoringCase2(xmlch_graph, graphInRuleGraph->getNodeName())) {
									rightSideGraph = readAnonHypergraph(graphInRuleGraph);
								} else if(isEqualIgnoringCase2(xmlch_quantification, graphInRuleGraph->getNodeName())) {
									readQuantMap(graphInRuleGraph, rightQuantMap, rightSideGraph.second);
								}
							}
						}
					}

				// the element is the mapping of the rule
				} else if (isEqualIgnoringCase2(xmlch_mapping, curElem->getNodeName())) {
					// Note that the ID of the mapping is neither read not stored here
					for(DOMElement* mapElem = curElem->getFirstElementChild(); mapElem != nullptr; mapElem = mapElem->getNextElementSibling()) {
						if(isEqualIgnoringCase2(xmlch_mapelem, mapElem->getNodeName())) {
							DOMAttr* fromAttr = mapElem->getAttributeNode(xmlch_from);
							DOMAttr* toAttr = mapElem->getAttributeNode(xmlch_to);
							if(fromAttr == NULL || toAttr == NULL) {
								throw XMLioException("A 'from' or 'to' attribute is missing in a 'mapelem' node of a rule.");
							}
							// if the graphs contain the correct ID and store the mapping
							SafeXTString<char> fromID(XMLString::transcode(fromAttr->getNodeValue()));
							if(leftSideGraph.second.count((string)fromID.getContent()) == 0) {
								stringstream ss;
								ss << "A 'from' attribute of a 'mapelem' node maps to a non-existing node '" << fromID.getContent() << "'.";
								throw XMLioException(ss.str());
							}
							SafeXTString<char> toID(XMLString::transcode(toAttr->getNodeValue()));
							if(rightSideGraph.second.count((string)toID.getContent()) == 0) {
								stringstream ss;
								ss << "A 'to' attribute of a 'mapelem' node maps to a non-existing node '" << toID.getContent() << "'.";
								throw XMLioException(ss.str());
							}
							mapping[leftSideGraph.second[(string)fromID.getContent()]] = rightSideGraph.second[(string)toID.getContent()];
						}
					}

				}

			}

			checkConsistency(mapping, *leftSideGraph.first, *rightSideGraph.first);

			Rule_sp newRule = make_shared<Rule>(ruleName, leftSideGraph.first, rightSideGraph.first, mapping);

			if(oldID) {
				(*oldIDs)[newRule->getID()] = oldID;
			}

			return make_tuple(newRule, leftQuantMap, rightQuantMap);
		}

		void GTXLReaderXerces::readQuantMap(DOMElement* root,
				GTXLReaderXerces::QuantMap& quantMap,
				unordered_map<string,IDType>& idMap) const {

			string quantID;
			DOMAttr* quantIDattr = root->getAttributeNode(xmlch_id);
			if(quantIDattr != nullptr) {
				SafeXTString<char> tempChar(XMLString::transcode(quantIDattr->getNodeValue()));
				quantID = (string)tempChar.getContent();
			} else if(showWarnings) {
				throw XMLioException("There is a quantification defined without id.");
			}

			for(DOMElement* curElem = root->getFirstElementChild(); curElem != nullptr; curElem = curElem->getNextElementSibling()) {

				// only 'elem' nodes are accepted
				if(isEqualIgnoringCase2(xmlch_elem, curElem->getNodeName())) {

					DOMAttr* ref = curElem->getAttributeNode(xmlch_ref);
					if(ref != nullptr) {

						SafeXTString<char> tmpChar(XMLString::transcode(ref->getNodeValue()));
						string tmpString(tmpChar.getContent());
						if(idMap.count(tmpString)) {
							quantMap[quantID].insert(idMap[tmpString]);
						} else {
							throw XMLioException("The quantification '" + quantID + "' references the non-existant element '" + tmpString + "'.");
						}

					} else if(showWarnings) {
						logger.newWarning() << "An 'elem' child of a 'quantification' node has no 'ref' attribute.";
						logger.contMsg() << " This child will be ignored." << endLogMessage;
					}

				} else if(showWarnings) {
					logger.newWarning() << "A 'quantification' node contained an non-elem child." << endLogMessage;
				}

			}
		}

		void GTXLReaderXerces::checkConsistency(const Mapping& mapping, const Hypergraph& left,
				const Hypergraph& right) const {

			for(auto iter = mapping.cbegin(); iter != mapping.cend(); iter++) {
				// if the ID is a Vertex on the left side, it also has to be on the right side
				if(left.hasVertex(iter->first)) {
					if(!right.hasVertex(iter->second)) {
						throw XMLioException("A vertex of the left graph is mapped to a non-vertex of the right graph.");
					}
				} else { // if the ID is an Edge on the left side, it also has to be on the right side
					// check existence of the edges
					auto leftVertices = left.getVerticesOfEdge(iter->first);
					auto rightVertices = right.getVerticesOfEdge(iter->second);
					if(leftVertices == nullptr || rightVertices == nullptr) {
						throw XMLioException("An edge of the left graph is mapped to a non-edge of the right graph or vice versa.");
					}

					// check if label is invariant
					if(left.getEdge(iter->first)->getLabel().compare(right.getEdge(iter->second)->getLabel())) {
						throw XMLioException("An edge of the left graph is mapped to an edge of the right, but the label is different.");
					}

					// check morphism property
					if(leftVertices->size() != rightVertices->size()) {
						throw XMLioException("An edge is mapped to an edge with different arity.");
					}
					for(auto leftIter = leftVertices->begin(), rightIter = rightVertices->begin();
							leftIter != leftVertices->end(); leftIter++, rightIter++) {
						// we know here that every node of the left edge has an entry in the mapping, else an
						// exception would have been thrown in readRule()
						if(mapping.getTar(*leftIter) != *rightIter) {
							throw XMLioException("An edge does not satisfy the morphism property.");
						}
					}
				}
			}
		}

		void GTXLReaderXerces::checkConsistency(Rule const& rule,
				GTXLReaderXerces::QuantMap& leftQuantMap,
				GTXLReaderXerces::QuantMap& rightQuantMap) const {

			// check consistency of left and right quantifications
			checkConsistency(*rule.getLeft(), leftQuantMap);
			checkConsistency(*rule.getRight(), rightQuantMap);

			Mapping const& mapping = rule.getMapping();
			Mapping::Inversion invMapping;
			mapping.getInverseMatch(invMapping);

			// check consistency of mapping
			for(GTXLReaderXerces::QuantMap::const_iterator outIt = leftQuantMap.begin(); outIt != leftQuantMap.end(); ++outIt) {

				// this will add an empty set for outIt->first if none was defined before
				// effectively the quantification on the right is assumed to be empty if not given
				unordered_set<IDType>& outRight = rightQuantMap[outIt->first];

				// throw an exception if the mapping of a left quantification element does not belong to the same quantification
				for(unordered_set<IDType>::const_iterator innerIt = outIt->second.begin(); innerIt != outIt->second.end(); ++innerIt) {
					if(mapping.hasSrc(*innerIt)) {
						if(!outRight.count(mapping.getTar(*innerIt))) {
							throw XMLioException("An element of the left quantification '" + outIt->first +
									"' is mapped to an element not belonging to the same quantification.");
						}
					} // else ignore
				}

				// throw an exception if a right quantification element has a preimage not in the same quantification
				for(unordered_set<IDType>::const_iterator invIt1 = outRight.cbegin(); invIt1 != outRight.cend(); ++invIt1) {
					if(invMapping.count(*invIt1)) {
						unordered_set<IDType> const& mappedElements = invMapping.at(*invIt1);
						for(unordered_set<IDType>::const_iterator invIt2 = mappedElements.end(); invIt2 != mappedElements.end(); ++invIt2) {
							if(!outIt->second.count(*invIt2)) {
								throw XMLioException("An element of the right quantification '" + outIt->first +
										"' has a preimage not belonging to the same quantification.");
							}
						}
					}
				}

			}

			if(leftQuantMap.size() != rightQuantMap.size()) {
				throw XMLioException("There is a rule where a quantification on a right side has no binding on the left side.");
			}

		}

		void GTXLReaderXerces::checkConsistency(AnonHypergraph const& graph,
				GTXLReaderXerces::QuantMap const& quantMap) const {

			// check 1: all nodes and edges belong to at most one quantification
			unordered_set<IDType> foundIDs;
			for(auto quantIt = quantMap.cbegin(); quantIt != quantMap.cend(); ++quantIt) {
				for(auto quantIDIt = quantIt->second.cbegin(); quantIDIt != quantIt->second.cend(); ++quantIDIt) {
					if(foundIDs.count(*quantIDIt)) {
						throw XMLioException("An element of one rule side belongs to two different quantifications.");
					} else {
						foundIDs.insert(*quantIDIt);
					}
				}
			}

			// foundIDs is now the union if all sets in the QuantMap and will be used as such

			for(auto quantIt = quantMap.cbegin(); quantIt != quantMap.cend(); ++quantIt) {

				unordered_set<IDType> const& curIDset = quantIt->second;

				for(auto quantIDIt = curIDset.cbegin(); quantIDIt != curIDset.cend(); ++quantIDIt) {

					// check 2: if a node belongs to a quantification, all attached edges also belong to the quantification
					if(graph.hasVertex(*quantIDIt)) {

						Hypergraph::ESet const* attEdges = graph.getConnectedEdges(*quantIDIt);
						for(Hypergraph::ESet::const_iterator attIt = attEdges->begin(); attIt != attEdges->end(); ++attIt) {
							if(!curIDset.count(*attIt)) {
								throw XMLioException("There is a quantified node which is attached to a non- or wrongly quantified edge.");
							}
						}

					} else if (graph.hasEdge(*quantIDIt)) {

						// no need to check anything; if a edge is attached to a node of a different quantification, this will already
						// be found in check 2

					} else {
						throw XMLioException("The quantified element with ID '" + boost::lexical_cast<string>(*quantIDIt) +
								"' does not exist in the graph.");
					}
				}
			}

		}

		inline bool GTXLReaderXerces::isEqualIgnoringCase2(const XMLCh* lowerCase, const XMLCh* notLowerCase) const {
			SafeXTString<XMLCh> nowLowerCase(XMLString::replicate(notLowerCase));
			XMLString::lowerCase(nowLowerCase.getContent());
			return XMLString::equals(lowerCase,nowLowerCase.getContent());
		}

	} /* namespace io */
} /* namespace uncover */
