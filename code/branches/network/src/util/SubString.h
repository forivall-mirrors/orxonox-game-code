/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Christian Meyer
 *   Co-authors:
 *      Benjamin Grauer
 *
//
//  splitLine
//  STL string tokenizer
//
//  Created by Clemens Wacha.
//  Version 1.0
//  Copyright (c) 2005 Clemens Wacha. All rights reserved.
//
 */

 /*!
 * @file substring.h
 * @brief a small class to get the parts of a string separated by commas
 *
 * This class is also identified as a Tokenizer. It splits up one long
 * String into multiple small ones by a designated Delimiter.
 *
 * Substring is Advanced, and it is possible, to split a string by ','
 * but also removing leading and trailing spaces around the comma.
 *
 * @example
 * Split the String std::string st = "1345, The new empire   , is , orxonox"
 * is splitted with:
 * SubString(st, ',', " \n\t")
 * into
 * "1345", "The new empire", "is", "orxonox"
 * As you can see, the useless spaces around ',' were removed.
 */

#ifndef __SUBSTRING_H__
#define __SUBSTRING_H__

#include <vector>
#include <string>

#include "UtilPrereqs.h"

//! A class that can load one string and split it in multipe ones
/**
 * SubString is a very Powerfull way to create a SubSet from a String
 * It can be used, to Split strings append them and join them again.
 */
class _UtilExport SubString
{
public:
  //! An enumerator for the State the Parser is in
  typedef enum {
    SL_NORMAL,            //!< Normal state
    SL_ESCAPE,            //!< After an escape character
    SL_SAFEMODE,          //!< In safe mode (between "" mostly).
    SL_SAFEESCAPE,        //!< In safe mode with the internal escape character, that escapes even the savemode character.
    SL_COMMENT,           //!< In Comment mode.
  } SPLIT_LINE_STATE;


public:
  SubString();
  SubString(const std::string& string, char delimiter = ',');
  SubString(const std::string& string,
            const std::string& delimiters, const std::string& delimiterNeighbours = "", bool emptyEntries=false,
            char escapeChar ='\\', char safemode_char = '"', char comment_char = '\0');
  SubString(unsigned int argc, const char** argv);
  /** @brief create a Substring as a copy of another one. @param subString the SubString to copy. */
  SubString(const SubString& subString) { *this = subString; };
  SubString(const SubString& subString, unsigned int subSetBegin);
  SubString(const SubString& subString, unsigned int subSetBegin, unsigned int subSetEnd);
  ~SubString();

  // operate on the SubString
  SubString& operator=(const SubString& subString);
  bool operator==(const SubString& subString) const;
  bool compare(const SubString& subString) const;
  bool compare(const SubString& subString, unsigned int length) const;
  SubString operator+(const SubString& subString) const;
  SubString& operator+=(const SubString& subString);
  /** @param subString the String to append @returns appended String. @brief added for convenience */
  SubString& append(const SubString subString) { return (*this += subString); };

  /////////////////////////////////////////
  // Split and Join the any String. ///////
  unsigned int split(const std::string& string = "", char delimiter = ',');
  unsigned int split(const std::string& string,
                     const std::string& delimiters, const std::string& delimiterNeighbours = "", bool emptyEntries = false,
                     char escapeChar ='\\', char safemode_char = '"', char comment_char = '\0');
  std::string join(const std::string& delimiter = " ") const;
  ////////////////////////////////////////

  // retrieve a SubSet from the String
  SubString subSet(unsigned int subSetBegin) const;
  SubString subSet(unsigned int subSetBegin, unsigned int subSetEnd) const;

  // retrieve Information from within
  /** @returns true if the SubString is empty */
  inline bool empty() const { return this->strings.empty(); };
  /** @returns the count of Strings stored in this substring */
  inline unsigned int size() const { return this->strings.size(); };
  /** @param i the i'th String @returns the i'th string from the subset of Strings */
  inline const std::string& operator[](unsigned int i) const { return this->strings[i]; };
  /** @param i the i'th String @returns the i'th string from the subset of Strings */
  inline const std::string& getString(unsigned int i) const { return (*this)[i]; };
  /** @returns the front of the StringList. */
  inline const std::string& front() const { return this->strings.front(); };
  /** @returns the back of the StringList. */
  inline const std::string& back() const { return this->strings.back(); };
  /** @brief removes the back of the strings list. */
  inline void pop_back() { this->strings.pop_back(); };

  // the almighty algorithm.
  static SPLIT_LINE_STATE splitLine(std::vector<std::string>& ret,
                                    const std::string& line,
                                    const std::string& delimiters = SubString::WhiteSpaces,
                                    const std::string& delimiterNeighbours = "",
                                    bool emptyEntries = false,
                                    char escape_char = '\\',
                                    char safemode_char = '"',
                                    char comment_char = '\0',
                                    SPLIT_LINE_STATE start_state = SL_NORMAL);
  // debugging.
  void debug() const;

public:
  static const std::string WhiteSpaces;
  static const std::string WhiteSpacesWithComma;
  static const SubString   NullSubString;

private:
  std::vector<std::string>  strings;                      //!< strings produced from a single string splitted in multiple strings
};

#endif /* __SUBSTRING_H__ */
