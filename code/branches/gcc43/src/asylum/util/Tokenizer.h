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
 *      Nicolas Perrenoud <nicolape_at_ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Tokenizer_H__
#define _Tokenizer_H__

#include <string>
#include <iostream>
#include <vector>

/**
 * String tokenizer
 *
 * Splits a given string into several smaller strings
 * using a delmiter (default is the comma).
 * Returns the result as a vector<string> object
 *
 * @author Nicolas Perrenoud<nicolape_at_ee.ethz.ch>
 */

std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters = ",")
{
  std::vector<std::string> tokens;

  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
  }
  return tokens;
}


#endif /* _Tokenizer_H__ */
