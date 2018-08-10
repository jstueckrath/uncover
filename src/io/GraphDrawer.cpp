/***************************************************************************
 *   Copyright (C) 2006 by Vitali Kozioura                                 *
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

#include "GraphDrawer.h"
#include <fstream>
#include <cmath>
#include "../logging/message_logging.h"
#include <sstream>

using namespace uncover::graphs;
using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace io {

		GraphDrawer::GraphDrawer() {}

		GraphDrawer::~GraphDrawer() {}

		void GraphDrawer::writeGraphToDotFile(
				AnonHypergraph const& graph,
				string filename,
				unordered_map<IDType,string> const& labels,
				unordered_map<IDType,GraphDrawer::BorderStyle> const& borderStyles) {

			std::ofstream of;
			of.open(filename.c_str());

			// export all vertices as nodes
			of << "graph \"" << graph.getName() << "\" {" << endl;
			for(auto it = graph.cbeginVertices(); it != graph.cendVertices(); it++)
			{
				of << "_" << it->first << " [shape=circle, margin=0, width=.15, label=\"";
				if(labels.count(it->first) != 0) {
					of << labels.find(it->first)->second;
				}
				of << "\"";
				if(borderStyles.count(it->first)) {
					of << ", " << BorderStyleToString(borderStyles.at(it->first));
				}
				of << "] \n";
			}

			// export all edges as nodes and connections
			for(auto it = graph.cbeginEdges(); it != graph.cendEdges(); it++)
			{
				string label = it->second.getLabel();
				of <<  "_" << it->first <<" [shape=box, style=rounded, label=\"" << label;
				if(labels.count(it->first) != 0) {
					of << " | " << labels.find(it->first)->second;
				}
				of << "\"";
				if(borderStyles.count(it->first)) {
					of << ", " << BorderStyleToString(borderStyles.at(it->first));
				}
				of << "] \n";

				vector<IDType> const* connectedV =  graph.getVerticesOfEdge(it->first);
				if(connectedV == nullptr) {
					logger.newCriticalError() << "Data structure Error while exporting graph to dot file! An edge had ";
					logger.contMsg() << "no list of attached vertices (edgeID=" << it->second.getID() << "). Edge ignored.";
					continue;
				}
				unordered_map<int, int> edgeLabelPos;
				const float PI = 3.1415;
				float angle = 0;
				int distance = 0;

				for(size_t i = 0; i < connectedV->size(); ++i)
				{
					int pos = edgeLabelPos[connectedV->at(i)]+1;


					if ((pos % 2)  == 0)
					{
						angle = atan(tan(35.0*PI/180)/(pos/2))/PI*180;
						distance = pos / 2;
					}
					else
					{
						angle = - atan(tan(35.0*PI/180)/((pos+1)/2))/PI*180;
						distance = (pos+1)/2;
					}

					of << "_" << it->first << " -- _" << connectedV->at(i) << "[taillabel=\"" << i << "\", labelangle=";
					of << angle << ", labeldistance=" << distance;
					if(borderStyles.count(it->first)) {
						of << ", " << BorderStyleToString(borderStyles.at(it->first));
					}
					of << "]\n";

					edgeLabelPos[connectedV->at(i)]++;
				}
			}
			of << "}\n";
		}

		void GraphDrawer::convertDotTo(string dotFile, string outFile, PictureFormat format, bool genExt) {
			stringstream command;
			command << "neato -T";
			switch(format) {
				case PicPS:
					command << "ps";
					break;
				case PicPNG:
					command << "png";
					break;
				case PicPDF:
					command << "pdf";
					break;
			}
			command << " -Gspline=true -Goverlap=false " << dotFile << " > " << outFile;
			if(genExt) {
				command << "." << getStandardExtension(format);
			}
			system(command.str().c_str());
		}

		string GraphDrawer::getStandardExtension(PictureFormat format) {

			switch (format) {

				case PicPS:
					return "ps";

				case PicPNG:
					return "png";

				case PicPDF:
					return "pdf";

				default:
					return "unknown";

			}

		}

		string GraphDrawer::BorderStyleToString(GraphDrawer::BorderStyle style) {

			switch(style) {

				case BorderGreen:
					return "color=green";

				case BorderRed:
					return "color=red";

				case BorderYellow:
					return "color=yellow";

				case BorderBlue:
					// same as default

				default:
					return "color=blue";

			}

		}

	}
}
