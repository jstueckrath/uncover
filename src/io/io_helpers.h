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

#ifndef IO_HELPERS_H_
#define IO_HELPERS_H_

#include "../basic_types/globals.h"

namespace uncover {
	namespace io {

		/**
		 * Takes a filename and returns the filename without extension. If the name contains multiple extensions,
		 * only the right most is deleted. If the name contains no extension, the name is returned unchanged.
		 * @param filename the filename to process
		 * @return the filename without extension
		 */
		inline string removeFileExtension(string filename) {
			size_t point = filename.rfind('.');
			if(point == string::npos) {
				return filename;
			} else {
				return filename.substr(0,point);
			}
		}

		/**
		 * Takes a filename including path and returns the filename without its path. If the name contains no
		 * path, the name is returned unchanged.
		 * @param filename the filename to process (including its path)
		 * @return the filename without its path
		 */
		inline string removeFilePath(string filename) {
			size_t point = filename.rfind('/');
			if(point == string::npos) {
				return filename;
			} else {
				return filename.substr(point+1);
			}
		}

	}
}

#endif /* IO_HELPERS_H_ */
