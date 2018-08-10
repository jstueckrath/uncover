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

#include "LatexExporter.h"
#include "GraphDrawer.h"
#include "io_helpers.h"
#include <sstream>
#include "../logging/message_logging.h"
#include <boost/filesystem.hpp>

using namespace uncover::graphs;
using namespace uncover::logging;
using namespace boost::filesystem;
using namespace boost::system;
using std::stringstream;
using std::ofstream;

namespace uncover {
	namespace io {

		LatexExporter::LatexExporter() : scale(0.5), compile(true) {}

		LatexExporter::~LatexExporter() {}

		void LatexExporter::setScale(double scale) {
			this->scale = scale;
		}

		double LatexExporter::getScale() const {
			return scale;
		}

		void LatexExporter::setCompilation(bool compile) {
			this->compile = compile;
		}

		bool LatexExporter::willCompile() const {
			return compile;
		}

		void LatexExporter::writeToLatex(GTS const& gts,
				string filename,
				LatexExporter::TargetType target,
				unordered_map<IDType, IDType> const* ruleIDSubs,
				unordered_map<IDType, IDType> const* graphIDSubs) const {
			writeToLatex(&gts, nullptr, filename, target, ruleIDSubs, graphIDSubs);
		}

		void LatexExporter::writeToLatex(vector<Hypergraph_sp> const& graphs,
				string filename,
				LatexExporter::TargetType target,
				unordered_map<IDType, IDType> const* nameSubs) const {
			writeToLatex(nullptr, &graphs, filename, target, nullptr, nameSubs);
		}

		void LatexExporter::writeToLatex(graphs::GTS const* gts,
				vector<graphs::Hypergraph_sp> const* graphs,
				string filename,
				LatexExporter::TargetType target,
				unordered_map<IDType, IDType> const* ruleIDSubs,
				unordered_map<IDType, IDType> const* graphIDSubs) const {

			ofstream of;
			of.open(filename.c_str());

			writeLatexHeader(of);

			// depending on the specified target, other file have to be generated; also, the \imgImport command may be different
			switch(target) {
				case TargetType::PS:
				{
					of << "\\usepackage{epsf}\n";
					of << "\\newcommand{\\imgImport}[1]{\\scalebox{" << scale << "}{\\epsffile{#1}}}\n";
					if(gts) {
						writeLatexBody(*gts, of, filename, "." + GraphDrawer::getStandardExtension(GraphDrawer::PictureFormat::PicPS),ruleIDSubs,graphIDSubs);
						writeGraphs(*gts, filename, GraphDrawer::PictureFormat::PicPS);
					} else if(graphs) {
						writeLatexAndGraphs(*graphs, of, filename, GraphDrawer::PictureFormat::PicPS, graphIDSubs);
					} else {
						logger.newCriticalError() << "Internal version of writeToLatex(...) called with wrong parameters.";
						logger.contMsg() << " Aborting file generation." << endLogMessage;
					}
					of.close();

					if(!compile) {
						break;
					}

					string latexCommand = string("latex ") + filename;
					system(latexCommand.c_str());
					string latexCommand2 = string("dvips ") + removeFileExtension(filename);
					system(latexCommand2.c_str());
					break;
				}
				case TargetType::PDF:
				{
					of << "\\newcommand{\\imgImport}[1]{\\scalebox{" << scale << "}{\\includegraphics{#1}}}\n";
					if(gts) {
						writeLatexBody(*gts, of, filename, "." + GraphDrawer::getStandardExtension(GraphDrawer::PictureFormat::PicPDF),ruleIDSubs,graphIDSubs);
						writeGraphs(*gts, filename, GraphDrawer::PictureFormat::PicPDF);
					} else if(graphs) {
						writeLatexAndGraphs(*graphs, of, filename, GraphDrawer::PictureFormat::PicPDF, graphIDSubs);
					} else {
						logger.newCriticalError() << "Internal version of writeToLatex(...) called with wrong parameters.";
						logger.contMsg() << " Aborting file generation." << endLogMessage;
					}
					of.close();

					if(!compile) {
						break;
					}

					string latexCommand = string("pdflatex ") + filename;
					system(latexCommand.c_str());
					break;
				}
			}
		}

		void LatexExporter::writeLatexHeader(ofstream& of) const {
			of << "\\documentclass[a4paper]{article}\n\n";
			of << "\\usepackage{graphicx}\n";
			of << "\\usepackage{amsmath}\n";
			of << "\\usepackage[top=1in, bottom=1in, left=1in, right=1in]{geometry}\n";
			of << "\\newsavebox{\\GBox}\n";
			of << "\\newcommand\\cappedBox[2]{%\n";
			of << " \\sbox{\\GBox}{\\begin{tabular}{c}#2\\end{tabular}}%\n";
      of << " \\ifdim\\wd\\GBox>#1\\resizebox{#1}{!}{\\usebox{\\GBox}}\\else\\usebox{\\GBox}\\fi}\n";
		}

		void LatexExporter::writeLatexAndGraphs(vector<graphs::Hypergraph_sp> const& graphs,
				std::ofstream& of,
				string filename,
				GraphDrawer::PictureFormat format,
				unordered_map<IDType, IDType> const* nameSubs) const {

			path dirName = removeFileExtension(filename) + string("_files");
			error_code errorCode;
			create_directory(dirName, errorCode);
			if(errorCode.value()) {
				logger.newWarning() << "An Error occurred while creating the directory for graph images: '";
				logger.contMsg() << errorCode.message() << "' ... will try to proceed." << endLogMessage;
			}

			of << "\n\\begin{document}\n";

			// create a tabular containing all initial graphs
			of << " \\hrule\\begin{center}Graphs:\\end{center}\\hrule\n";
			if(graphs.size()) {
				auto it = graphs.cbegin();

				of << " \n\\bigskip\n\n" << getNameOrNoName((*it)->getName()) << " (";
				if(nameSubs) {
					if (nameSubs->count((*it)->getID())) {
						of << "old-id=" << nameSubs->at((*it)->getID());
					} else {
						of << "old-id=unknown";
					}
				} else {
					of << "new-id=" << (*it)->getID();
				}
				of << "):\n";

				of << " \n\\bigskip\n\n";
				of << " \\begin{center}\\cappedBox{\\textwidth}{\\imgImport{" << dirName;
				of << "/graph" << (*it)->getID() << "." << GraphDrawer::getStandardExtension(format) << "}}\\end{center}\n";

				stringstream nameStub;
				nameStub << dirName.c_str() << "/graph" << ((*it)->getID());
				GraphDrawer::writeGraphToDotFile(**it, nameStub.str() + ".dot");
				GraphDrawer::convertDotTo(nameStub.str() + ".dot", nameStub.str(), format, true);
				nameStub.str(string());
				nameStub.clear();

				++it;
				for(; it != graphs.end(); ++it) {

					of << " \\hrule\n";
					of << " \n\\bigskip\n\n" << getNameOrNoName((*it)->getName()) << " (";
					if(nameSubs) {
						if (nameSubs->count((*it)->getID())) {
							of << "old-id=" << nameSubs->at((*it)->getID());
						} else {
							of << "old-id=unknown";
						}
					} else {
						of << "new-id=" << (*it)->getID();
					}
					of << "):\n";

					of << " \n\\bigskip\n\n";
					of << " \\begin{center}\\cappedBox{\\textwidth}{\\imgImport{" << dirName;
					of << "/graph" << (*it)->getID() << "." << GraphDrawer::getStandardExtension(format) << "}}\\end{center}\n";

					nameStub << dirName.c_str() << "/graph" << ((*it)->getID());
					GraphDrawer::writeGraphToDotFile(**it, nameStub.str() + ".dot");
					GraphDrawer::convertDotTo(nameStub.str() + ".dot", nameStub.str(), format, true);
					nameStub.str(string());
					nameStub.clear();
				}
			} else {
				of << "\\begin{center}none\\end{center}\n";
			}

			of << "\\end{document}";
		}

		void LatexExporter::writeLatexBody(GTS const& gts,
				ofstream& of,
				string filename,
				string ext,
				unordered_map<IDType, IDType> const* ruleIDSubs,
				unordered_map<IDType, IDType> const* graphIDSubs) const {

			string dirName = removeFileExtension(filename) + string("_files");

			of << "\n\\begin{document}\n";

			// create a tabular containing all initial graphs
			of << " \\hrule\\begin{center}Initial Graphs:\\end{center}\\hrule\n";
			if(gts.getInitialCount()) {
				vector<Hypergraph_sp> const& initialGraphs = gts.getInitialGraphs();
				vector<Hypergraph_sp>::const_iterator initIt = initialGraphs.begin();
				of << " \n\\bigskip\n\n";
				of << getNameOrNoName((*initIt)->getName()) << " (";
				if(graphIDSubs) {
					if(graphIDSubs->count((*initIt)->getID())) {
						of << "old-id=" << graphIDSubs->at((*initIt)->getID());
					} else {
						of << "old-id=unknown";
					}
				} else {
					of << "new-id=" << (*initIt)->getID();
				}
				of << "):\n";
				of << " \n\\bigskip\n\n";
				of << " \\begin{center}\\cappedBox{\\textwidth}{\\imgImport{" << dirName;
				of << "/initial" << (*initIt)->getID() << ext << "}}\\end{center}\n";
				++initIt;
				for(; initIt != initialGraphs.end(); ++initIt) {
					of << " \\hrule\n";
					of << " \n\\bigskip\n\n";
					of << getNameOrNoName((*initIt)->getName()) << " (";
					if(graphIDSubs) {
						if(graphIDSubs->count((*initIt)->getID())) {
							of << "old-id=" << graphIDSubs->at((*initIt)->getID());
						} else {
							of << "old-id=unknown";
						}
					} else {
						of << "new-id=" << (*initIt)->getID();
					}
					of << "):\n";
					of << " \n\\bigskip\n\n";
					of << " \\begin{center}\\cappedBox{\\textwidth}{\\imgImport{" << dirName;
					of << "/initial" << (*initIt)->getID() << ext << "}}\\end{center}\n";
				}
			} else {
				of << "\\begin{center}none\\end{center}\n";
			}

			// create a tabular containing all standard rules, formatted in "rule style"
			of << " \\hrule\\begin{center}Rules:\\end{center}\\hrule\n";
			if(gts.getStdRuleCount()) {

				auto ruleIt = gts.getStdRules().cbegin();
				of << " \n\\bigskip\n\n";
				of << getNameOrNoName(ruleIt->second->getName()) << " (";
				if(ruleIDSubs) {
					if(ruleIDSubs->count(ruleIt->second->getID())) {
						of << "old-id=" << ruleIDSubs->at(ruleIt->second->getID());
					} else {
						of << "old-id=unknown";
					}
				} else {
					of << "new-id=" << ruleIt->second->getID();
				}
				of << "):\n";
				of << " \n\\bigskip\n\n";
				of << " \\begin{tabular}{rcl}\n";
				of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "l" << ext << "}} &\n";
				of << "  $\\Longrightarrow$ &\n";
				of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "r" << ext << "}}\n";
				of << " \\end{tabular}\n";
				of << " \n\\bigskip\n\n";
				++ruleIt;

				for(; ruleIt != gts.getStdRules().cend(); ++ruleIt) {
					of << " \\hrule\n";
					of << " \n\\bigskip\n\n";
					of << getNameOrNoName(ruleIt->second->getName()) << " (";
					if(ruleIDSubs) {
						if(ruleIDSubs->count(ruleIt->second->getID())) {
							of << "old-id=" << ruleIDSubs->at(ruleIt->second->getID());
						} else {
							of << "old-id=unknown";
						}
					} else {
						of << "new-id=" << ruleIt->second->getID();
					}
					of << "):\n";
					of << " \n\\bigskip\n\n";
					of << " \\begin{tabular}{rcl}\n";
					of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "l" << ext << "}} &\n";
					of << "  $\\Longrightarrow$ &\n";
					of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "r" << ext << "}}\n";
					of << " \\end{tabular}\n";
					of << " \n\\bigskip\n\n";
				}

			} else {
				of << " \\begin{center}none\\end{center}\n";
			}

			// create a tabular containing all universally quantified rules, formatted in "rule style"
			of << " \\hrule\\begin{center}Universally Quantified Rules:\\end{center}\\hrule\n";
			if(gts.getUQRuleCount()) {

				auto ruleIt = gts.getUQRules().cbegin();
				of << " \n\\bigskip\n\n";
				of << getNameOrNoName(ruleIt->second->getName()) << " (";
				if(ruleIDSubs) {
					if(ruleIDSubs->count(ruleIt->second->getID())) {
						of << "old-id=" << ruleIDSubs->at(ruleIt->second->getID());
					} else {
						of << "old-id=unknown";
					}
				} else {
					of << "new-id=" << ruleIt->second->getID();
				}
				of << "):\n";
				of << " \n\\bigskip\n\n";
				of << " \\begin{tabular}{rcl}\n";
				of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "l" << ext << "}} &\n";
				of << "  $\\Longrightarrow$ &\n";
				of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "r" << ext << "}}\n";
				of << " \\end{tabular}\n";
				of << " \n\\bigskip\n\n";
				++ruleIt;

				for(; ruleIt != gts.getUQRules().cend(); ++ruleIt) {
					of << " \\hrule\n";
					of << " \n\\bigskip\n\n";
					of << getNameOrNoName(ruleIt->second->getName()) << " (";
					if(ruleIDSubs) {
						if(ruleIDSubs->count(ruleIt->second->getID())) {
							of << "old-id=" << ruleIDSubs->at(ruleIt->second->getID());
						} else {
							of << "old-id=unknown";
						}
					} else {
						of << "new-id=" << ruleIt->second->getID();
					}
					of << "):\n";
					of << " \n\\bigskip\n\n";
					of << " \\begin{tabular}{rcl}\n";
					of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "l" << ext << "}} &\n";
					of << "  $\\Longrightarrow$ &\n";
					of << "  \\cappedBox{0.4\\textwidth}{\\imgImport{" << dirName << "/rule" << ruleIt->first << "r" << ext << "}}\n";
					of << " \\end{tabular}\n";
					of << " \n\\bigskip\n\n";
				}

			} else {
				of << " \\begin{center}none\\end{center}\n";
			}

			of << "\\end{document}";

		}

		void LatexExporter::writeGraphs(GTS const& gts, string filename, GraphDrawer::PictureFormat format) const {

			path dirPath(removeFileExtension(filename) + string("_files"));
			error_code errorCode;
			create_directory(dirPath, errorCode);
			if(errorCode.value()) {
				logger.newWarning() << "An Error occurred while creating the directory for graph images: '";
				logger.contMsg() << errorCode.message() << "' ... will try to proceed." << endLogMessage;
			}

			// create dot and image files for all initial graphs
			vector<Hypergraph_sp> const& initialGraphs = gts.getInitialGraphs();
			stringstream nameStub;
			for(vector<Hypergraph_sp>::const_iterator iter = initialGraphs.begin(); iter != initialGraphs.end(); ++iter) {
				nameStub << dirPath.c_str() << "/initial" << ((*iter)->getID());
				GraphDrawer::writeGraphToDotFile(**iter, nameStub.str() + ".dot");
				GraphDrawer::convertDotTo(nameStub.str() + ".dot", nameStub.str(), format, true);
				nameStub.str(string());
				nameStub.clear();
			}

			// create dot and image files for all left and right sides of standard rules
			for(auto iter = gts.getStdRules().cbegin(); iter != gts.getStdRules().cend(); ++iter) {
				nameStub << dirPath.c_str() << "/rule" << iter->first;

				// generate the labels of nodes and edges to encode the mapping into the drawn left and right graphs
				unordered_map<IDType,string> leftLabels;
				unordered_map<IDType,string> rightLabels;
				Mapping& mapping = iter->second->getMapping();
				stringstream counterStream;
				unsigned int counter = 1;
				for(Mapping::iterator mapIt = mapping.begin(); mapIt != mapping.end(); ++mapIt, ++counter) {
					counterStream << counter;
					leftLabels[mapIt->first] = counterStream.str();
					if(rightLabels.count(mapIt->second) == 0) {
						rightLabels[mapIt->second] = counterStream.str();
					} else {
						rightLabels[mapIt->second] = rightLabels[mapIt->second] + "," + counterStream.str();
					}
					counterStream.str(string());
					counterStream.clear();
				}

				GraphDrawer::writeGraphToDotFile(*(iter->second->getLeft()), nameStub.str() + "l.dot", leftLabels);
				GraphDrawer::writeGraphToDotFile(*(iter->second->getRight()), nameStub.str() + "r.dot", rightLabels);
				GraphDrawer::convertDotTo(nameStub.str() + "l.dot", nameStub.str() + "l", format, true);
				GraphDrawer::convertDotTo(nameStub.str() + "r.dot", nameStub.str() + "r", format, true);
				nameStub.str(string());
				nameStub.clear();
			}

			// create dot and image files for all left and right sides of universally quantified rules
			for(auto iter = gts.getUQRules().cbegin(); iter != gts.getUQRules().cend(); ++iter) {

				nameStub << dirPath.c_str() << "/rule" << iter->first;
				UQRule_csp rule = iter->second;

				vector<unordered_set<IDType>> leftNewElem;
				vector<unordered_set<IDType>> rightNewElem;
				UQRuleInstance_sp oneInstance = rule->createOneInstance(&leftNewElem, &rightNewElem);

				unordered_map<IDType, GraphDrawer::BorderStyle> leftColorMap;
				unordered_map<IDType, GraphDrawer::BorderStyle> rightColorMap;
				if(leftNewElem.size() != rule->getQuantCount() || rightNewElem.size() != rule->getQuantCount()) {

					logger.newError() << "LatexExporter: An error occurred while writing UQRules, "
							"UQRule::createOneInstance() returned wrong results. Printing rule with id '";
					logger.contMsg() << rule->getID() << "' without color." << endLogMessage;

				} else {

					// generate a mapping, assigning a visual style to each quantification
					vector<GraphDrawer::BorderStyle> styleForQuant;

					if(rule->getQuantCount() > 4) {

						logger.newWarning() << "There are more than four quantifications, using default style for all." << endLogMessage;
						for(size_t i = 0; i < rule->getQuantCount(); ++i) {
							styleForQuant.push_back(GraphDrawer::BorderStyle::BorderBlue);
						}

					} else {

						styleForQuant.push_back(GraphDrawer::BorderStyle::BorderBlue);
						styleForQuant.push_back(GraphDrawer::BorderStyle::BorderGreen);
						styleForQuant.push_back(GraphDrawer::BorderStyle::BorderRed);
						styleForQuant.push_back(GraphDrawer::BorderStyle::BorderYellow);

					}

					// set styles
					for(size_t quantIt = 0; quantIt < rule->getQuantCount(); ++quantIt) {

						for(auto idIt = leftNewElem.at(quantIt).cbegin(); idIt != leftNewElem.at(quantIt).cend(); ++idIt) {
							leftColorMap[*idIt] = styleForQuant.at(quantIt);
						}

						for(auto idIt = rightNewElem.at(quantIt).cbegin(); idIt != rightNewElem.at(quantIt).cend(); ++idIt) {
							rightColorMap[*idIt] = styleForQuant.at(quantIt);
						}

					}

				}

				// generate the labels of nodes and edges to encode the mapping into the drawn left and right graphs
				unordered_map<IDType,string> leftLabels;
				unordered_map<IDType,string> rightLabels;
				Mapping& mapping = oneInstance->getMapping();
				stringstream counterStream;
				unsigned int counter = 1;
				for(Mapping::iterator mapIt = mapping.begin(); mapIt != mapping.end(); ++mapIt, ++counter) {
					counterStream << counter;
					leftLabels[mapIt->first] = counterStream.str();
					if(rightLabels.count(mapIt->second) == 0) {
						rightLabels[mapIt->second] = counterStream.str();
					} else {
						rightLabels[mapIt->second] = rightLabels[mapIt->second] + "," + counterStream.str();
					}
					counterStream.str(string());
					counterStream.clear();
				}

				GraphDrawer::writeGraphToDotFile(*(oneInstance->getLeft()), nameStub.str() + "l.dot", leftLabels, leftColorMap);
				GraphDrawer::writeGraphToDotFile(*(oneInstance->getRight()), nameStub.str() + "r.dot", rightLabels, rightColorMap);
				GraphDrawer::convertDotTo(nameStub.str() + "l.dot", nameStub.str() + "l", format, true);
				GraphDrawer::convertDotTo(nameStub.str() + "r.dot", nameStub.str() + "r", format, true);
				nameStub.str(string());
				nameStub.clear();
			}
		}

		inline string LatexExporter::getNameOrNoName(string name) const {
			if(name.compare("")) {
				return name;
			} else {
				return "$\\langle$noname$\\rangle$";
			}
		}

	} /* namespace io */
} /* namespace uncover */
