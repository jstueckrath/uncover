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

#include "XercesLoggingErrorHandler.h"
#include "../logging/message_logging.h"
#include "xercesc_helpers.h"
#include "XMLioException.h"
#include <sstream>

using namespace xercesc;
using namespace uncover::logging;
using namespace std;

namespace uncover {
	namespace io {

		XercesLoggingErrorHandler::XercesLoggingErrorHandler() {}

		XercesLoggingErrorHandler::~XercesLoggingErrorHandler() {}

		void XercesLoggingErrorHandler::warning(const SAXParseException& exc) {
			logger.newWarning() << "Xercesc Warning: " << generateXerxesErrorMessage(exc) << endLogMessage;
		}

		void XercesLoggingErrorHandler::error(const SAXParseException& exc) {
			stringstream s;
			s << "Xercesc Error: " << generateXerxesErrorMessage(exc);
			throw XMLioException(s.str());
		}

		void XercesLoggingErrorHandler::fatalError(const SAXParseException& exc) {
			stringstream s;
			s << "Xercesc Fatal Error: " << generateXerxesErrorMessage(exc);
			throw XMLioException(s.str());
		}

		void XercesLoggingErrorHandler::resetErrors() {}

	}
 /* namespace io */
} /* namespace uncover */
