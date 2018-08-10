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

#ifndef ALGORITHMS_HYPERGRAPHSIGNATURE_CREATER_H
#define ALGORITHMS_HYPERGRAPHSIGNATURE_CREATER_H

#include "Hypergraph.h"

namespace uncover {
	namespace graphs {

		/**
		 * The SignatureCreater provides functions to generate signatures for Hypergraphs. A signature is a hash
		 * satisfying that two Hypergraphs are isomorphic if their signatures are equal. The other way does not
		 * necessarily hold!
		 * @see Hypergraph
		 * @see HGSignature
		 * @author Jan Stückrath
		 * @author Marvin Heumüller
		 */
		class SignatureCreater
		{

			public:

				/**
				 * Computes the signature for a Hypergraph. The current content of the given 'signature' parameter is
				 * deleted and replaced by a detailed signature. A shorter signature is returned.
				 * @param graph the Hypergraph for which the signature will be created
				 * @param signature a container to store the computed (detailed) signature
				 * @return a short signature
				 */
				static PartSig createHGSignature(Hypergraph const& graph, HGSignature& signature);

				/**
				 * Destroys this SignatureCreater.
				 */
				virtual ~SignatureCreater();

			private:

				/**
				 * This constructor cannot be used.
				 */
				SignatureCreater();

				/**
				 * Computes a hash value for a string. This function uses the ELF hash function to generate hash values
				 * for labels of edges.
				 * @param s the string to be hashed
				 * @return a hash value
				 */
				static PartSig hashFromString(string s);

				/**
				 * Computes a hash value for an edge based on its label and attached vertices.
				 * @param old the old hash of the edge (usually the a hash of the label)
				 * @param label the label of the edge
				 * @param neighborCert the hash values of each attached vertices
				 * @return a hash of the edge
				 */
				static PartSig newCert(PartSig const& old, string label, vector<PartSig> const& neighborCert);

		};

	}
}

#endif
