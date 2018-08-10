/***************************************************************************
 *   Copyright (C) 2009 by Marvin Heumüller                                *
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

#include "SignatureCreater.h"
#include <set>

using namespace std;
using namespace uncover::graphs;

namespace uncover {
	namespace graphs {

		SignatureCreater::SignatureCreater() {}

		SignatureCreater::~SignatureCreater() {}

		PartSig SignatureCreater::createHGSignature(Hypergraph const& h, HGSignature& result)
		{
			result.clear();
			
			for (auto it = h.cbeginVertices(); it != h.cendVertices(); ++it)
			{
				result.insert(make_pair(it->first, 1));
			}

			for (auto it = h.cbeginEdges(); it != h.cendEdges(); ++it)
			{
				result.insert(make_pair(it->first, hashFromString(it->second.getLabel())));
			}
			
			set<PartSig> signatures;
			signatures.insert(1);
			size_t lastSize = 0;
			PartSig sum = 0;
			while (lastSize < signatures.size())
			{
				lastSize = signatures.size();
				signatures = set<PartSig>();
				sum = 0;
				HGSignature resCopy = result;
				for (auto it = h.cbeginVertices(); it != h.cendVertices(); ++it)
				{
					IDType id = it->first;
					PartSig newC = resCopy[id];
					auto neigh = *(h.getConnectedEdges(id));
					for (auto it2 = neigh.cbegin(); it2 != neigh.cend(); ++it2)
					{
						newC += resCopy[*it2];
					}
					result[id] = newC;
					signatures.insert(newC);
					sum += newC;
				}
				
				for (auto it = h.cbeginEdges(); it != h.cendEdges(); ++it)
				{
					IDType id = it->first;
					PartSig newC = resCopy[id];
					vector<IDType> const* neigh = h.getVerticesOfEdge(id);
					vector<PartSig> certs;
					for (auto it2 = neigh->cbegin(); it2 != neigh->cend(); ++it2)
					{
						certs.push_back(resCopy[*it2]);
					}
					result[id] = newCert(resCopy[id], it->second.getLabel(), certs);
					signatures.insert(newC);
					sum += newC;
				}
			}

			return sum;
		}
		
		PartSig SignatureCreater::hashFromString(string s)
		{
			PartSig hash = 0;
			PartSig x = 0;
			
			for(std::size_t i = 0; i < s.length(); i++)
			{
				hash = (hash << 4) + s[i];
				if((x = hash & 0xF0000000L) != 0)
				{
					hash ^= (x >> 24);
				}
				hash &= ~x;
			}
			
			return hash;
		}
		
		PartSig SignatureCreater::newCert(PartSig const& old, string label, vector<PartSig> const& neighborCerts)
		{
			int shift0 = (hashFromString(label) & 0xf) + 1;
			int shift1 = 8;
			bool alter = true;
			PartSig res = old;
			for (auto it = neighborCerts.cbegin(); it != neighborCerts.cend(); ++it)
			{
				if (alter)
				{
					res += (*it << shift0) | (*it >> (32-shift0));
				}
				else
				{
					res += (*it << shift1) | (*it >> (32-shift1));
				}
			}
			return res;
		}
	}

}
