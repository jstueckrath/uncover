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

#ifndef XERCESC_HELPERS_H_
#define XERCESC_HELPERS_H_

#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <string>

namespace uncover {
	namespace io {

		/**
		 * This template provides a container which automatically releases a transcoded string, when the pointer runs out of scope.
		 * @author Jan Stückrath
		 */
		template <class T>
		class SafeXTString {

		  public:

				/**
				 * Generates a new SafeXTString with the specified (transcoded) string pointer as content.
				 * @param ptr a transcoded string pointer which will be safely stored
				 */
		    SafeXTString (T* ptr) {stringPtr=ptr;}

		    /**
		     * Destroys this SafeXTString object. The contained string is thereby properly released.
		     */
		    virtual ~SafeXTString() {xercesc::XMLString::release(&stringPtr);}

		    /**
		     * Returns the content stored in this SafeXTString object. Note that the returned pointer may not be released,
		     * since it will be released automatically when this object is destroyed.
		     * @return the content stored in this SafeXTString object
		     */
		    T* getContent() {return stringPtr;}

		    /**
		     * Replaces the content of this container object with the given content. Applying the = operator will properly
		     * release old content and store the new content.
		     * @param data the new content to be stored
		     * @return a reference to this container object
		     */
		    SafeXTString<T>& operator= (T* data) {xercesc::XMLString::release(&stringPtr); stringPtr = data; return *this;}

		    /**
		     * Applying this operator will stream out the content of this object without alteration. This operator must
		     * also be defined for the content.
		     * @param ost the output stream
		     * @param data the SafeXTString object to be streamed
		     * @return the output stream
		     */
		    friend std::ostream& operator<< (std::ostream& ost, SafeXTString<T>& data) { ost << data.stringPtr; return ost;}

		  private:

		    /**
		     * This pointer represent a string transcoded by Xercesc.
		     */
		    T* stringPtr;

		};

		/**
		 * This template provides a container which automatically release a given Xerces resource, when the pointer runs out of scope.
		 * @author Jan Stückrath
		 */
		template <class T>
		class SafeXRes {

		  public:

				/**
				 * Generates a new SafeXRes with the specified resource pointer as content.
				 * @param ptr a resource pointer which will be safely stored
				 */
				SafeXRes (T* ptr) {resPtr=ptr;}

		    /**
		     * Destroys this SafeXRes object. The contained resource is thereby properly released.
		     */
		    virtual ~SafeXRes() {resPtr->release();}

		    /**
		     * Returns the content stored in this SafeXRes object. Note that the returned pointer may not be released,
		     * since it will be released automatically when this object is destroyed.
		     * @return the content stored in this SafeXRes object
		     */
		    T* getContent() {return resPtr;}

		    /**
		     * Returns the pointer stored in this SafeXRes object. In this way SafeXRes can be used as if it would be a pointer.
		     * @return the pointer stored in this SafeXRes object
		     */
		    T* operator->() {return resPtr;}

		  private:

		    /**
		     * This pointer represent a releasable Xerces resource.
		     */
		    T* resPtr;

		};

		/**
		 * Generates and returns a string representing the error message in the given exception.
		 * @param e an XMLException produced by the Xerxes parser
		 * @return a string representation of the exceptions message
		 */
		extern std::string generateXerxesErrorMessage(const xercesc::XMLException& e);

		/**
		 * Generates and returns a string representing the error message in the given exception.
		 * @param e an SAXParserException produced by the Xerxes parser
		 * @return a string representation of the exceptions message
		 */
		extern std::string generateXerxesErrorMessage(const xercesc::SAXParseException& e);

	}
}

#endif /* XERCESC_HELPERS_H_ */
