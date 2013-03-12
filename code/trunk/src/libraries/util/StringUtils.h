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
 *      ...
 *
 */

/**
    @defgroup String String functions
    @ingroup Util
*/

/**
    @file
    @ingroup String
    @brief Declaration of several string manipulation functions, used in many parts of the game.
*/

#ifndef _StringUtils_H__
#define _StringUtils_H__

#include "UtilPrereqs.h"
#include <string>
#include <vector>

namespace orxonox
{
    extern _UtilExport std::string BLANKSTRING;

    _UtilExport std::string  getUniqueNumberString();

    _UtilExport void         strip(std::string* str);
    _UtilExport std::string  getStripped(const std::string& str);

    _UtilExport std::string  removeTrailingWhitespaces(const std::string& str);

    _UtilExport void         vectorize(const std::string& str, char delimiter, std::vector<std::string>* output);

    _UtilExport size_t       getNextQuote(const std::string& str, size_t start = 0);
    _UtilExport bool         isBetweenQuotes(const std::string& str, size_t pos);

    _UtilExport bool         hasStringBetweenQuotes(const std::string& str);
    _UtilExport std::string  getStringBetweenQuotes(const std::string& str);

    _UtilExport std::string  stripEnclosingQuotes(const std::string& str);
    _UtilExport std::string  stripEnclosingBraces(const std::string& str);

    _UtilExport bool         isEmpty(const std::string& str);
    _UtilExport bool         isComment(const std::string& str);

    _UtilExport std::string  addSlashes(const std::string& str);
    _UtilExport std::string  removeSlashes(const std::string& str);

    _UtilExport void         lowercase(std::string* str);
    _UtilExport std::string  getLowercase(const std::string& str);

    _UtilExport void         uppercase(std::string* str);
    _UtilExport std::string  getUppercase(const std::string& str);

    _UtilExport int          nocaseCmp(const std::string& s1, const std::string& s2);
    _UtilExport int          nocaseCmp(const std::string& s1, const std::string& s2, size_t len);

    _UtilExport bool         hasComment(const std::string& str);
    _UtilExport std::string  getComment(const std::string& str);
    _UtilExport size_t       getNextCommentPosition(const std::string& str, size_t start = 0);

    _UtilExport size_t       replaceCharacters(std::string& str, char target, char replacement);

    _UtilExport unsigned int getLevenshteinDistance(const std::string& str1, const std::string& str2);

    _UtilExport std::string  getTimestamp(void);
}

#endif /* _StringUtils_H__ */
