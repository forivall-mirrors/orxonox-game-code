/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Benjamin Grauer
 *
 */

#ifndef _Util_String_H__
#define _Util_String_H__

#include "UtilPrereqs.h"

#include <string>
#include <sstream>

extern _UtilExport std::string blankString;

_UtilExport void         strip(std::string* str);
_UtilExport std::string  getStripped(const std::string& str);

_UtilExport std::string  removeTrailingWhitespaces(const std::string& str);

_UtilExport unsigned int getNextQuote(const std::string& str, unsigned int start);
_UtilExport bool         isBetweenQuotes(const std::string& str, unsigned int pos);

_UtilExport bool         hasStringBetweenQuotes(const std::string& str);
_UtilExport std::string  getStringBetweenQuotes(const std::string& str);

_UtilExport std::string  stripEnclosingQuotes(const std::string& str);
_UtilExport std::string  stripEnclosingBraces(const std::string& str);

_UtilExport bool         isEmpty(const std::string& str);
_UtilExport bool         isComment(const std::string& str);
_UtilExport bool         isNumeric(const std::string& str);

_UtilExport std::string  addSlashes(const std::string& str);
_UtilExport std::string  removeSlashes(const std::string& str);

_UtilExport void         lowercase(std::string* str);
_UtilExport std::string  getLowercase(const std::string& str);

_UtilExport void         uppercase(std::string* str);
_UtilExport std::string  getUppercase(const std::string& str);

_UtilExport int          nocaseCmp(const std::string& s1, const std::string& s2);
_UtilExport int          nocaseCmp(const std::string& s1, const std::string& s2, unsigned int len);

_UtilExport bool         hasComment(const std::string& str);
_UtilExport std::string  getComment(const std::string& str);
_UtilExport unsigned int getCommentPosition(const std::string& str);
_UtilExport unsigned int getNextCommentPosition(const std::string& str, unsigned int start = 0);

#endif /* _Util_String_H__ */
