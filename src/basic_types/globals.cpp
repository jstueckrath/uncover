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

#include "globals.h"
#include <time.h>
#include <iomanip>
#include <limits.h>
#include "OutOfIDsException.h"

using namespace uncover::basic_types;

namespace uncover {

	IDType const IDType_MAX = UINT_MAX;

	namespace logging {

		string gl_logTypeToString(LogTypes type) {
			switch(type) {
				case LT_UserInfo:
					return "Inf";
				case LT_CriticalError:
					return "Crt";
				case LT_Error:
					return "Err";
				case LT_Warning:
					return "War";
				case LT_Debug:
					return "Deb";
				case LT_None:
					return "Non";
				default:
					return "Unk";
			}
		}

		LogTypes gl_LogLevel(LogTypes::LT_Debug);

	}

	/**
	 * Stores the last ID used for a graph. DO NOT MODIFY!
	 */
	IDType gl_LastUsedStateID = 1;

	IDType gl_getNewStateID() {
		if(gl_LastUsedStateID == IDType_MAX)
			throw OutOfIDsException("The program has run out of unique state IDs and has to be terminated.");
		return gl_LastUsedStateID++;
	}

	/**
	 * Stores the last ID used for a rule. DO NOT MODIFY!
	 */
	IDType gl_LastUsedRuleID = 1;

	IDType gl_getNewRuleID() {
		if(gl_LastUsedRuleID == IDType_MAX)
			throw OutOfIDsException("The program has run out of unique rule IDs and has to be terminated.");
		return gl_LastUsedRuleID++;
	}

	/**
	 * Stores the last ID used for a (backward) application of a rule. DO NOT MODIFY!
	 */
	IDType gl_LastUsedStepID = 1;

	IDType gl_getNewStepID() {
		if(gl_LastUsedStepID == IDType_MAX)
			throw OutOfIDsException("The program has run out of unique transition step IDs and has to be terminated.");
		return gl_LastUsedStepID++;
	}

}


