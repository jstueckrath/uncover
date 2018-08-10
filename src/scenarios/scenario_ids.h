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

#ifndef SCENARIOIDS_H_
#define SCENARIOIDS_H_

#define SCN_NONAME_ID 0
#define SCN_NONAME_NAME "noname"

/* Analysis */

#define SCN_BACKWARD_ANALYSIS_ID 100
#define SCN_BACKWARD_ANALYSIS_NAME "backward_analysis"
#define SCN_BACKWARD_ANALYSIS_SHORT_1 "backw"

#define SCN_LEQ_CHECK_ID 200
#define SCN_LEQ_CHECK_NAME "leq_check"
#define SCN_LEQ_CHECK_SHORT_1 "leq"

#define SCN_LE_RULE_CREATOR_ID 300
#define SCN_LE_RULE_CREATOR_NAME "le_rule_creator"
#define SCN_LE_RULE_CREATOR_SHORT_1 "lerc"

/* Converter and Translator */

#define SCN_GXL_PIC_CONVERTER_ID 1000
#define SCN_GXL_PIC_CONVERTER_NAME "gxl_pic_converter"
#define SCN_GXL_PIC_CONVERTER_SHORT_1 "gxl2pic"

#define SCN_GTXL_LATEX_CONVERTER_ID 1100
#define SCN_GTXL_LATEX_CONVERTER_NAME "gtxl_latex_converter"
#define SCN_GTXL_LATEX_CONVERTER_SHORT_1 "gtxl2latex"

/* Tests */

#define SCN_TEST_XML_ID 10000
#define SCN_TEST_XML_NAME "test_xml"

#define SCN_TEST_RULE_PREPARER_ID 11000
#define SCN_TEST_RULE_PREPARER_NAME "test_rule_preparer"
#define SCN_TEST_RULE_PREPARER_SHORT_1 "testprep"

#define SCN_TEST_BACKWARD_STEP_ID 12000
#define SCN_TEST_BACKWARD_STEP_NAME "test_backward_step"
#define SCN_TEST_BACKWARD_STEP_SHORT_1 "testbws"

#define SCN_TEST_MATCHER_ID 13000
#define SCN_TEST_MATCHER_NAME "test_matcher"

#define SCN_

#endif /* SCENARIOIDS_H_ */
