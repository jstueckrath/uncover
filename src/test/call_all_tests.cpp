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

#ifdef COMPILE_UNIT_TESTS

#define BOOST_TEST_MODULE "all tests"
#include <boost/test/included/unit_test.hpp>
#include "../logging/message_logging.h"

// disable buffering of cout because of issues with Boost.Test
static struct DisableStdCoutBuffering
{
    DisableStdCoutBuffering()
    {
        std::cout.setf(std::ios_base::unitbuf);
    }
} s_disableStdCoutBuffering;

using namespace uncover::logging;

// initializing the message logger to be cout
MessageLogger uncover::logging::logger(std::cout);

// **********************************************************************
// all test suites are loaded automatically
// **********************************************************************

#endif
