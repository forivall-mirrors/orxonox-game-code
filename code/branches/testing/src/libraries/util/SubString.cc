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
 *

//
//  splitLine
//  STL string tokenizer
//
//  Created by Clemens Wacha.
//  Version 1.0
//  Copyright (c) 2005 Clemens Wacha. All rights reserved.
//

 *   Extended by Fabian 'x3n' Landau by the SL_PARENTHESES mode.
 */

/**
    @file
    @brief Implementation of the SubString class.
*/

#include "SubString.h"
#include <cstdio>
#include "Output.h"

namespace orxonox
{
    const std::string SubString::WhiteSpaces          = " \n\t";
    const std::string SubString::WhiteSpacesWithComma = " \n\t,";
    const SubString SubString::NullSubString          = SubString();

    /**
        @brief Default constructor.
    */
    SubString::SubString()
    {
    }

    /**
        @brief Splits a string into multiple tokens.
        @param line The line to split
        @param delimiters Multiple characters at which to split the line
        @param delimiterNeighbours Neighbours of the delimiters that will be erased as well (for example white-spaces)
        @param bAllowEmptyEntries If true, empty tokens are also added to the SubString (if there are two delimiters without a char in between)
        @param escapeChar The escape character that is used to escape safemode chars (for example if you want to use a quotation mark between two other quotation marks).
        @param bRemoveEscapeChar If true, the escape char is removed from the tokens
        @param safemodeChar Within these characters splitting won't happen (usually the quotation marks)
        @param bRemoveSafemodeChar Removes the safemodeChar from the beginning and the ending of a token
        @param openparenthesisChar The beginning of a safemode is marked with this (usually an opening brace)
        @param closeparenthesisChar The ending of a safemode is marked with this (usually a closing brace)
        @param bRemoveParenthesisChars Removes the parenthesis chars from the beginning and the ending of a token
        @param commentChar The comment character (used to ignore the part of the line after the comment char).
    */
    SubString::SubString(const std::string& line,
                         const std::string& delimiters, const std::string& delimiterNeighbours, bool bAllowEmptyEntries,
                         char escapeChar, bool bRemoveEscapeChar, char safemodeChar, bool bRemoveSafemodeChar,
                         char openparenthesisChar, char closeparenthesisChar, bool bRemoveParenthesisChars, char commentChar)
    {
        SubString::splitLine(this->tokens_, this->bTokenInSafemode_, line, delimiters, delimiterNeighbours, bAllowEmptyEntries, escapeChar, bRemoveEscapeChar, safemodeChar, bRemoveSafemodeChar, openparenthesisChar, closeparenthesisChar, bRemoveParenthesisChars, commentChar);
    }

    /**
        @brief creates a new SubString based on a subset of an other SubString.
        @param other The other SubString
        @param begin The beginning of the subset
        @param length The length of the subset

        The subset ranges from the token with index @a begin and contains @a length elements.
    */
    SubString::SubString(const SubString& other, size_t begin, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            if (begin + i >= other.size())
                break;

            this->tokens_.push_back(other[begin + i]);
            this->bTokenInSafemode_.push_back(other.isInSafemode(begin + i));
        }
    }

    /**
        @brief Creates a SubString from a count and values set.
        @param argc The number of arguments
        @param argv An array of pointers to the arguments
    */
    SubString::SubString(size_t argc, const char** argv)
    {
        for (size_t i = 0; i < argc; ++i)
        {
            this->tokens_.push_back(std::string(argv[i]));
            this->bTokenInSafemode_.push_back(false);
        }
    }

    /**
        @brief Destructor
    */
    SubString::~SubString()
    { }

    /**
        @brief Stores the tokens of @a other in this SubString
        @return This SubString.
    */
    SubString& SubString::operator=(const SubString& other)
    {
        this->tokens_ = other.tokens_;
        this->bTokenInSafemode_ = other.bTokenInSafemode_;
        return *this;
    }

    /**
        @brief Compares this SubString to another SubString and returns true if they contain the same values.
    */
    bool SubString::operator==(const SubString& other) const
    {
        return ((this->tokens_ == other.tokens_) && (this->bTokenInSafemode_ == other.bTokenInSafemode_));
    }

    /**
        @brief Compares this SubString to another SubString and returns true if the first @a length values match.
        @param other The other SubString
        @param length How many tokens to compare
    */
    bool SubString::compare(const SubString& other, size_t length) const
    {
        if (std::min(length, this->size()) != std::min(length, other.size()))
            return false;

        for (size_t i = 0; i < std::min(length, this->size()); ++i)
            if ((this->tokens_[i] != other.tokens_[i]) || (this->bTokenInSafemode_[i] != other.bTokenInSafemode_[i]))
                return false;

        return true;
    }

    /**
        @brief Concatenates the tokens of two SubStrings and returns the resulting new SubString
        @return A new SubString that contains the tokens of this and the other SubString
    */
    SubString SubString::operator+(const SubString& other) const
    {
        return SubString(*this) += other;
    }

    /**
        @brief Appends the tokens of @a other to this SubString
        @return This SubString
    */
    SubString& SubString::operator+=(const SubString& other)
    {
        for (size_t i = 0; i < other.size(); ++i)
        {
            this->tokens_.push_back(other[i]);
            this->bTokenInSafemode_.push_back(other.isInSafemode(i));
        }
        return *this;
    }

    /**
        @copydoc SubString(const std::string&,const std::string&,const std::string&,bool,char,bool,char,bool,char,char,bool,char)
    */
    size_t SubString::split(const std::string& line,
                            const std::string& delimiters, const std::string& delimiterNeighbours, bool bAllowEmptyEntries,
                            char escapeChar, bool bRemoveEscapeChar, char safemodeChar, bool bRemoveSafemodeChar,
                            char openparenthesisChar, char closeparenthesisChar, bool bRemoveParenthesisChars, char commentChar)
    {
        this->tokens_.clear();
        this->bTokenInSafemode_.clear();
        SubString::splitLine(this->tokens_, this->bTokenInSafemode_, line, delimiters, delimiterNeighbours, bAllowEmptyEntries, escapeChar, bRemoveEscapeChar, safemodeChar, bRemoveSafemodeChar, openparenthesisChar, closeparenthesisChar, bRemoveParenthesisChars, commentChar);
        return this->tokens_.size();
    }

    /**
        @brief Joins the tokens of this SubString using the given delimiter and returns a string.
        @param delimiter This delimiter will be placed between each two tokens
        @return The joined string.
    */
    std::string SubString::join(const std::string& delimiter) const
    {
        if (!this->tokens_.empty())
        {
            std::string retVal = this->tokens_[0];
            for (size_t i = 1; i < this->tokens_.size(); ++i)
                retVal += delimiter + this->tokens_[i];
            return retVal;
        }
        else
            return "";
    }

    /**
        @brief Creates a subset of this SubString.
        @param begin The beginning of the subset
        @param length The length of the subset
        @return A new SubString containing the defined subset.

        The subset ranges from the token with index @a begin and contains @a length elements.

        This function is added for your convenience, and does the same as
        SubString::SubString(const SubString& other, size_t begin, size_t length)
    */
    SubString SubString::subSet(size_t begin, size_t length) const
    {
        return SubString(*this, begin, length);
    }

    /**
        @copydoc SubString(const std::string&,const std::string&,const std::string&,bool,char,bool,char,bool,char,char,bool,char)
        @param tokens The array, where the splitted strings will be stored in
        @param bTokenInSafemode A vector wich stores for each character of the string if it is in safemode or not
        @param start_state The internal state of the parser

        This is the actual splitting algorithm from Clemens Wacha.
        Supports delimiters, escape characters, ignores special characters between safemodeChar and between commentChar and line end "\n".

        Extended by Orxonox to support parenthesis as additional safe-mode.
    */
    SubString::SPLIT_LINE_STATE
    SubString::splitLine(std::vector<std::string>& tokens,
                         std::vector<bool>& bTokenInSafemode,
                         const std::string& line,
                         const std::string& delimiters,
                         const std::string& delimiterNeighbours,
                         bool bAllowEmptyEntries,
                         char escapeChar,
                         bool bRemoveEscapeChar,
                         char safemodeChar,
                         bool bRemoveSafemodeChar,
                         char openparenthesisChar,
                         char closeparenthesisChar,
                         bool bRemoveParenthesisChars,
                         char commentChar,
                         SPLIT_LINE_STATE start_state)
    {
        SPLIT_LINE_STATE state = start_state;
        size_t i = 0;
        size_t fallBackNeighbours = 0;

        std::string token;
        bool inSafemode = false;

        if(start_state != SL_NORMAL && tokens.size() > 0)
        {
            token = tokens[tokens.size()-1];
            tokens.pop_back();
        }
        if(start_state != SL_NORMAL && bTokenInSafemode.size() > 0)
        {
            inSafemode = bTokenInSafemode[bTokenInSafemode.size()-1];
            bTokenInSafemode.pop_back();
        }

        while(i < line.size())
        {
            switch(state)
            {
            case SL_NORMAL:
                if(line[i] == escapeChar)
                {
                    state = SL_ESCAPE;
                    if (!bRemoveEscapeChar)
                        token += line[i];
                    fallBackNeighbours = 0;
                }
                else if(line[i] == safemodeChar)
                {
                    state = SL_SAFEMODE;
                    inSafemode = true;
                    if (!bRemoveSafemodeChar)
                        token += line[i];
                    fallBackNeighbours = 0;
                }
                else if(line[i] == openparenthesisChar)
                {
                    state = SL_PARENTHESES;
                    inSafemode = true;
                    if (!bRemoveParenthesisChars)
                        token += line[i];
                    fallBackNeighbours = 0;
                }
                else if(line[i] == commentChar)
                {
                    if (fallBackNeighbours > 0)
                        token = token.substr(0, token.size() - fallBackNeighbours);
                    fallBackNeighbours = 0;
                    // FINISH
                    if(bAllowEmptyEntries || token.size() > 0)
                    {
                        tokens.push_back(token);
                        token.clear();
                        bTokenInSafemode.push_back(inSafemode);
                        inSafemode = false;
                    }
                    token += line[i];       // EAT
                    state = SL_COMMENT;
                }
                else if(delimiters.find(line[i]) != std::string::npos)
                {
                    // line[i] is a delimiter
                    if (fallBackNeighbours > 0)
                        token = token.substr(0, token.size() - fallBackNeighbours);
                    fallBackNeighbours = 0;
                    // FINISH
                    if(bAllowEmptyEntries || token.size() > 0)
                    {
                        tokens.push_back(token);
                        token.clear();
                        bTokenInSafemode.push_back(inSafemode);
                        inSafemode = false;
                    }
                    state = SL_NORMAL;
                }
                else
                {
                    if (delimiterNeighbours.find(line[i]) != std::string::npos)
                    {
                        if (token.size() > 0)
                            ++fallBackNeighbours;
                        else
                        {
                            ++i;
                            continue;
                        }
                    }
                    else
                        fallBackNeighbours = 0;
                    token += line[i];       // EAT
                }
                break;
            case SL_ESCAPE:
                if (!bRemoveSafemodeChar)
                    token += line[i];
                else
                {
                    if(line[i] == 'n') token += '\n';
                    else if(line[i] == 't') token += '\t';
                    else if(line[i] == 'v') token += '\v';
                    else if(line[i] == 'b') token += '\b';
                    else if(line[i] == 'r') token += '\r';
                    else if(line[i] == 'f') token += '\f';
                    else if(line[i] == 'a') token += '\a';
                    else if(line[i] == '?') token += '\?';
                    else token += line[i];  // EAT
                }
                state = SL_NORMAL;
                break;
            case SL_SAFEMODE:
                if(line[i] == safemodeChar)
                {
                    state = SL_NORMAL;
                    if (!bRemoveSafemodeChar)
                        token += line[i];
                }
                else if(line[i] == escapeChar)
                {
                    state = SL_SAFEESCAPE;
                }
                else
                {
                    token += line[i];       // EAT
                }
                break;

            case SL_SAFEESCAPE:
                if(line[i] == 'n') token += '\n';
                else if(line[i] == 't') token += '\t';
                else if(line[i] == 'v') token += '\v';
                else if(line[i] == 'b') token += '\b';
                else if(line[i] == 'r') token += '\r';
                else if(line[i] == 'f') token += '\f';
                else if(line[i] == 'a') token += '\a';
                else if(line[i] == '?') token += '\?';
                else token += line[i];  // EAT
                state = SL_SAFEMODE;
                break;

            case SL_PARENTHESES:
                if(line[i] == closeparenthesisChar)
                {
                    state = SL_NORMAL;
                    if (!bRemoveParenthesisChars)
                        token += line[i];
                }
                else if(line[i] == escapeChar)
                {
                    state = SL_PARENTHESESESCAPE;
                }
                else
                {
                    token += line[i];       // EAT
                }
                break;

            case SL_PARENTHESESESCAPE:
                if(line[i] == 'n') token += '\n';
                else if(line[i] == 't') token += '\t';
                else if(line[i] == 'v') token += '\v';
                else if(line[i] == 'b') token += '\b';
                else if(line[i] == 'r') token += '\r';
                else if(line[i] == 'f') token += '\f';
                else if(line[i] == 'a') token += '\a';
                else if(line[i] == '?') token += '\?';
                else token += line[i];  // EAT
                state = SL_PARENTHESES;
                break;

            case SL_COMMENT:
                if(line[i] == '\n')
                {
                    // FINISH
                    if(token.size() > 0)
                    {
                        tokens.push_back(token);
                        token.clear();
                        bTokenInSafemode.push_back(inSafemode);
                        inSafemode = false;
                    }
                    state = SL_NORMAL;
                }
                else
                {
                    token += line[i];       // EAT
                }
                break;

            default:
                // nothing
                break;
            }
            ++i;
        }

        // FINISH
        if (fallBackNeighbours > 0)
            token = token.substr(0, token.size() - fallBackNeighbours);
        if(bAllowEmptyEntries || token.size() > 0)
        {
            tokens.push_back(token);
            token.clear();
            bTokenInSafemode.push_back(inSafemode);
            inSafemode = false;
        }
        return(state);
    }

    /**
        @brief Some nice debug information about this SubString.
    */
    void SubString::debug() const
    {
        orxout(debug_output) << "Substring-information::count=" << this->tokens_.size() << " ::";
        for (size_t i = 0; i < this->tokens_.size(); ++i)
            orxout(debug_output) << "s" << i << "='" << this->tokens_[i].c_str() << "'::";
        orxout(debug_output) << endl;
    }
}
