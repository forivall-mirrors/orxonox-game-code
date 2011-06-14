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
 *      Christian Meyer
 *   Co-authors:
 *      Benjamin Grauer
 *      Fabian 'x3n' Landau
 *

//  splitLine
//  STL string tokenizer
//
//  Created by Clemens Wacha.
//  Version 1.0
//  Copyright (c) 2005 Clemens Wacha. All rights reserved.

 *   Extended by Fabian 'x3n' Landau by the SL_PARENTHESES mode.
 */

 /**
    @file
    @ingroup String
    @brief A helper class to split a string into several tokens.

    @anchor SubStringExample

    The class SubString can be used to split an std::string into multiple tokens, using
    a delimiter. SubString allows different options, for example to remove whitespaces
    around the delimiters or different safe-mode chars, like quotation marks and braces.

    You can access the tokens of the SubString using the [] operator like an array.
    SubString also supports to join the tokens (or a subset of the tokens) again using
    @ref orxonox::SubString::join() "join()". It's even possible to get a subset of the
    SubString as another SubString using @ref orxonox::SubString::subSet() "subSet()".

    Example:
    @code
    std::string text = "This is a test, \"Hello \\\" World\" and vector {1, 2, 3}";
    SubString tokens(text, SubString::WhiteSpaces, "", false, '\\', true, '"', true, '{', '}', true, '\0');

    for (unsigned int i = 0; i < tokens.size(); ++i)
        COUT(0) << i << ": " << tokens[i] << std::endl;
    @endcode

    The output of this code is:
     - 0: This
     - 1: is
     - 2: a
     - 3: test,
     - 4: Hello " World
     - 5: and
     - 6: vector
     - 7: 1, 2, 3

    The string was split using the delimiter " ". A string between quotation mark is not
    split, the same holds for strings between '{' and '}'. Note how the quotation marks and
    the braces were removed from the tokens, because the corresponding argument is 'true'.

    Also note that the comma after "test" in token 3 is still there - it is neither part of the
    delimiters SubString::WhiteSpaces nor part of the delimiterNeighbours parameter, so it
    remains a part of the token.
*/

#ifndef __SubString_H__
#define __SubString_H__

#include "UtilPrereqs.h"

#include <vector>
#include <string>

namespace orxonox
{
    /**
        @brief A class that splits a string into multiple tokens using different options.

        The string is split into multiple tokens using a delimiter. Different options like
        escape character, quotation marks, and more can be used to satisfy your needs.

        See @ref SubStringExample "this description" for an example.
    */
    class _UtilExport SubString
    {
        /// An enumerator for the internal state of the parser
        enum SPLIT_LINE_STATE
        {
            SL_NORMAL,            //!< Normal state
            SL_ESCAPE,            //!< After an escape character
            SL_SAFEMODE,          //!< In safe mode (usually between quotation marks).
            SL_SAFEESCAPE,        //!< In safe mode with the internal escape character, that escapes even the savemode character.
            SL_COMMENT,           //!< In Comment mode.
            SL_PARENTHESES,       //!< Between parentheses (usually '{' and '}')
            SL_PARENTHESESESCAPE, //!< Between parentheses with the internal escape character, that escapes even the closing parenthesis character.
        };

    public:
        SubString();
        SubString(const std::string& line,
                  const std::string& delimiters = SubString::WhiteSpaces,
                  const std::string& delimiterNeighbours = "",
                  bool bAllowEmptyEntries=false,
                  char escapeChar ='\\',
                  bool bRemoveEscapeChar = true,
                  char safemodeChar = '"',
                  bool bRemoveSafemodeChar = true,
                  char openparenthesisChar = '{',
                  char closeparenthesisChar = '}',
                  bool bRemoveParenthesisChars = true,
                  char commentChar = '\0');
        SubString(unsigned int argc, const char** argv);
        SubString(const SubString& other, unsigned int begin);
        SubString(const SubString& other, unsigned int begin, unsigned int end);
        ~SubString();

        // operate on the SubString
        SubString& operator=(const SubString& other);
        bool operator==(const SubString& other) const;
        bool compare(const SubString& other) const;
        bool compare(const SubString& other, unsigned int length) const;
        SubString operator+(const SubString& other) const;
        SubString& operator+=(const SubString& other);
        /// Appends the tokens of another SubString to this. @return This SubString.
        inline SubString& append(const SubString& other) { return (*this += other); }

        /////////////////////////////////////////
        // Split and Join the any String. ///////
        unsigned int split(const std::string& line,
                           const std::string& delimiters = SubString::WhiteSpaces,
                           const std::string& delimiterNeighbours = "",
                           bool bAllowEmptyEntries = false,
                           char escapeChar ='\\',
                           bool bRemoveEscapeChar = true,
                           char safemodeChar = '"',
                           bool bRemoveSafemodeChar = true,
                           char openparenthesisChar = '{',
                           char closeparenthesisChar = '}',
                           bool bRemoveParenthesisChars = true,
                           char commentChar = '\0');

        std::string join(const std::string& delimiter = " ") const;
        ////////////////////////////////////////

        // retrieve a SubSet from the String
        SubString subSet(unsigned int begin) const;
        SubString subSet(unsigned int begin, unsigned int end) const;

        // retrieve Information from within
        /// Returns true if the SubString is empty
        inline bool empty() const { return this->tokens_.empty(); }
        /// Returns the number of tokens stored in this SubString
        inline unsigned int size() const { return this->tokens_.size(); }
        /// Returns the i'th token from the subset of strings @param index The index of the requested token
        inline const std::string& operator[](unsigned int index) const { return this->tokens_[index]; }
        /// Returns the i'th token from the subset of strings @param index The index of the requested token
        inline const std::string& getString(unsigned int index) const { return (*this)[index]; }
        /// Returns all tokens as std::vector
        inline const std::vector<std::string>& getAllStrings() const { return this->tokens_; }
        /// Returns true if the token is in safemode. @param index The index of the token
        inline bool isInSafemode(unsigned int index) const { return this->bTokenInSafemode_[index]; }
        /// Returns the front of the list of tokens.
        inline const std::string& front() const { return this->tokens_.front(); }
        /// Returns the back of the list of tokens.
        inline const std::string& back() const { return this->tokens_.back(); }
        /// Removes the back of the list of tokens.
        inline void pop_back() { this->tokens_.pop_back(); this->bTokenInSafemode_.pop_back(); }

        void debug() const;

    public:
        static const std::string WhiteSpaces;           ///< All whitespaces (usually used as delimiters or delimiterNeighbours
        static const std::string WhiteSpacesWithComma;  ///< All whitespaces and the comma (usually used as delimiters)
        static const SubString   NullSubString;         ///< An empty SubString

    private:
        // the almighty algorithm.
        static SPLIT_LINE_STATE splitLine(std::vector<std::string>& tokens,
                                          std::vector<bool>& bTokenInSafemode,
                                          const std::string& line,
                                          const std::string& delimiters = SubString::WhiteSpaces,
                                          const std::string& delimiterNeighbours = "",
                                          bool bAllowEmptyEntries = false,
                                          char escapeChar = '\\',
                                          bool bRemoveEscapeChar = true,
                                          char safemodeChar = '"',
                                          bool bRemoveSafemodeChar = true,
                                          char openparenthesisChar = '{',
                                          char closeparenthesisChar = '}',
                                          bool bRemoveParenthesisChars = true,
                                          char commentChar = '\0',
                                          SPLIT_LINE_STATE start_state = SL_NORMAL);

        std::vector<std::string>  tokens_;              ///< The tokens after splitting the input line
        std::vector<bool>         bTokenInSafemode_;    ///< Saves for each token if it was in safe mode (between quotation marks or parenthesis)
    };
}

#endif /* __SubString_H__ */
