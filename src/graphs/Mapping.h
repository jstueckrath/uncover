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

#ifndef DATA_STRUCTURESMATCH_H
#define DATA_STRUCTURESMATCH_H

#include "../basic_types/globals.h"
#include "../basic_types/IDPartition.h"

namespace uncover {
	namespace graphs {

		/**
		 * This is the data structure representing a morphism mapping from a graph to another graph. The morphism
		 * property is not (automatically) ensured by this class! It just stores the mapping and provides
		 * manipulation methods.
		 * @author Jan Stückrath
		 */
		class Mapping
		{

			public:

				/**
				 * An iterator for enumerating all (IDType,IDType)-pairs stores in this Mapping.
				 */
				typedef unordered_map<IDType,IDType>::iterator iterator;

				/**
				 * A const_iterator for enumerating all (IDType,IDType)-pairs stores in this Mapping.
				 */
				typedef unordered_map<IDType,IDType>::const_iterator const_iterator;

				/**
				 * Alias for an inverse Mapping, mapping IDs to collections of IDs. This type is essentially
				 * an mapping with reversed direction, but without the function support of the Mapping class.
				 */
				typedef unordered_map<IDType, unordered_set<IDType>> Inversion;

				/**
				 * Generates a new empty Mapping object.
				 */
				Mapping();

				/**
				 * Destroys this Mapping.
				 */
				~Mapping();

				/**
				 * Removes the mapping for the given ID, if it exists.
				 * @param source the ID of which to mapping should be deleted
				 * @return true, iff there was a mapping deleted (false if the given ID had no mapping)
				 */
				bool delSrc(IDType const& source);

				/**
				 * Removes all mappings of the given IDs. IDs without mappings are ignored and the number of deleted
				 * mappings is returned.
				 * @param sources the set of IDs of which to mappings should be deleted
				 * @return the number of mappings deleted (i.e. the number of given ID which had a mapping)
				 */
				size_t delSrc(unordered_set<IDType> const& sources);

				/**
				 * Removes all mappings to the target ID. The number of removed mappings is returned.
				 * @param target all mappings to this ID will be deleted
				 * @return the number of mappings deleted
				 */
				size_t delTar(IDType const& target);

				/**
				 * Removes all mappings to one of the target IDs. The number of removed mappings is returned.
				 * @param targets all mappings to one of this IDs will be deleted
				 * @return the number of mappings deleted
				 */
				size_t delTar(unordered_set<IDType> const& targets);

				/**
				 * Returns whether the given ID is mapped to some ID by this Mapping.
				 * @param source the ID which should be checked
				 * @return true, iff the given ID is mapped by this Mapping
				 */
				bool hasSrc(IDType const& source) const;

				/**
				 * Returns whether some ID is mapped to the given ID by this Mapping.
				 * @param target the ID to be checked
				 * @return true, iff there is an ID which is mapped to the given ID
				 */
				bool hasTar(IDType const& target) const;

				/**
				 * Returns the ID to which the given ID is mapped.
				 * @param source the ID which should be mapped
				 * @return the ID to which the given ID is mapped
				 */
				IDType const& getTar(IDType const& source) const;

				/**
				 * Concatenates the given Mapping to the right of this Mapping, effectively changing this Mapping.
				 * The new Mapping contains a pair (x,y) if and only if x was mapped to some z by the old Mapping and
				 * z was mapped to y by the given Mapping.
				 * @param map the Mapping which will be concatenated to this Mapping
				 */
				void concat(Mapping const& map);

				/**
				 * Returns the ID to which the given ID is mapped. If the given ID is not mapped, a new mapping from
				 * the given ID to the ID 0 is added and 0 is returned. This operator can also be used to set mappings
				 * of this Mapping.
				 * @param src the ID of which its image will be returned
				 * @return a reference to the ID to which the given ID is mapped; 0 if the given ID had no mapping
				 */
				IDType& operator[](IDType const& src);

				/**
				 * Returns the set of all IDs which are mapped to the given ID.
				 * @param tar the target ID to which mappings should be found
				 * @return the set of all IDs mapped to the given ID
				 */
				unordered_set<IDType> getInverseMatch(IDType const& tar) const;

				/**
				 * Returns an Inversion of this Mapping. The Inversion maps all target IDs of this Mapping to the set
				 * of all source IDs which are mapped to each target ID. An ID which has no ID mapped to it, has no
				 * mapping in the Inversion (obviously).
				 * @return the Inversion of this Mapping
				 */
				Mapping::Inversion getInverseMatch() const;

				/**
				 * Clears the given Inversion and stores the Inversion of this Mapping in the given one. The Inversion
				 * maps all target IDs of this Mapping to the set of all source IDs which are mapped to each target ID.
				 * An ID which has no ID mapped to it, has no mapping in the Inversion (obviously).
				 * @param invmatch the Inversion which will be set to the Inversion of this Mapping
				 */
				void getInverseMatch(Mapping::Inversion& invmatch) const;

				/**
				 * Returns whether this Mapping is injective on all IDs on which it is defined.
				 * @return true, iff the Mapping is injective
				 */
				bool isInjective() const;

				/**
				 * Returns an iterator to the first element of this Mapping. Note that there is not guaranteed order on
				 * the pairs of this Mapping!
				 * @return an iterator to the first element of this Mapping
				 */
				Mapping::iterator begin() noexcept;

				/**
				 * Returns a const_iterator to the first element of this Mapping. Note that there is not guaranteed
				 * order on the pairs of this Mapping!
				 * @return a const_iterator to the first element of this Mapping
				 */
				Mapping::const_iterator begin() const noexcept;

				/**
				 * Returns a const_iterator to the first element of this Mapping. Note that there is not guaranteed
				 * order on the pairs of this Mapping!
				 * @return a const_iterator to the first element of this Mapping
				 */
				Mapping::const_iterator cbegin() const noexcept;

				/**
				 * Returns an iterator pointing beyond the last element of this Mapping. Note that there is not guaranteed
				 * order on the pairs of this Mapping!
				 * @return an iterator pointing beyond the last element of this Mapping
				 */
				Mapping::iterator end() noexcept;

				/**
				 * Returns a const_iterator pointing beyond the last element of this Mapping. Note that there is not
				 * guaranteed order on the pairs of this Mapping!
				 * @return a const_iterator pointing beyond the last element of this Mapping
				 */
				Mapping::const_iterator end() const noexcept;

				/**
				 * Returns a const_iterator pointing beyond the last element of this Mapping. Note that there is not
				 * guaranteed order on the pairs of this Mapping!
				 * @return a const_iterator pointing beyond the last element of this Mapping
				 */
				Mapping::const_iterator cend() const noexcept;

				/**
				 * Removes all pairs stores in this Mapping.
				 */
				void clear();

				/**
				 * Streams a string representation of the given Mapping to the given string.
				 * @param ost the stream to which it will be streamed
				 * @param map the Mapping which will be streamed
				 * @return a reference to the given stream object
				 */
				friend std::ostream& operator<<(std::ostream& ost, Mapping const& map);

				/**
				 * Returns true, if and only if the given Mappings are equal (i.e. contain the same mappings).
				 * @param m1 the first Mapping
				 * @param m2 the second Mapping
				 * @return true, iff the given Mappings are equal
				 */
				friend bool operator==(const Mapping& m1, const Mapping& m2 );

			private:

				/**
				 * Stores the data of this Mapping.
				 */
				unordered_map<IDType, IDType> match;

		};

		/**
		 * Alias for a shared pointer to a Mapping object.
		 */
		typedef shared_ptr<Mapping> Mapping_sp;

		/**
		 * Alias for a shared pointer to a constant Mapping object.
		 */
		typedef shared_ptr<Mapping const> Mapping_csp;

	}
}

#endif
