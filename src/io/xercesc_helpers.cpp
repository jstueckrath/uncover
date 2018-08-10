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

#include "xercesc_helpers.h"
#include <sstream>

namespace uncover {
	namespace io {

		std::string generateXerxesErrorMessage(const xercesc::XMLException& e) {
			std::stringstream ss;
			SafeXTString<char> message(xercesc::XMLString::transcode(e.getMessage()));
			ss << message.getContent();
			return ss.str();
		}

		std::string generateXerxesErrorMessage(const xercesc::SAXParseException& e) {
			std::stringstream ss;
			SafeXTString<char> message(xercesc::XMLString::transcode(e.getMessage()));
			ss << message.getContent();
			return ss.str();
		}

	}
}
