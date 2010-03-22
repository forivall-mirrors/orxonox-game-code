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

#include "SubString.h"
#include <cstdio>

namespace orxonox
{
    /**
     * @brief default constructor
     */
    SubString::SubString()
    {}


    /**
     * @brief create a SubString from
     * @param string the String to Split
     * @param delimiter the Character at which to split string (delimiter)
     */
    SubString::SubString(const std::string& string, char delimiter)
    {
        this->split(string, delimiter);
    }


    /**
     * @brief Splits a String into multiple splitters.
     * @param string the String to split
     * @param delimiters multiple set of characters at what to split. (delimiters)
     * @param delimiterNeighbours neighbours of the delimiters, that will be erased only when near a delimiter.
     * @param emptyEntries If empty entries should be allewed or removed.
     * @param escapeChar The Escape Character that overrides splitters commends and so on...
     * @param safemode_char within these characters splitting won't happen
     * @param comment_char the Comment character.
     */
    SubString::SubString(const std::string& string,
                         const std::string& delimiters, const std::string& delimiterNeighbours, bool emptyEntries,
                         char escapeChar, bool removeEscapeChar, char safemode_char, bool removeSafemodeChar,
                         char openparenthesis_char, char closeparenthesis_char, bool removeParenthesisChars, char comment_char)
    {
        SubString::splitLine(this->strings, this->bInSafemode, string, delimiters, delimiterNeighbours, emptyEntries, escapeChar, removeEscapeChar, safemode_char, removeSafemodeChar, openparenthesis_char, closeparenthesis_char, removeParenthesisChars, comment_char);
    }

    /**
     * @brief creates a SubSet of a SubString.
     * @param subString the SubString to take a set from.
     * @param subSetBegin the beginning to the end
     */
    SubString::SubString(const SubString& subString, unsigned int subSetBegin)
    {
        for (unsigned int i = subSetBegin; i < subString.size(); i++)
        {
            this->strings.push_back(subString[i]);
            this->bInSafemode.push_back(subString.isInSafemode(i));
        }
    }


    /**
     * @brief creates a SubSet of a SubString.
     * @param subString the SubString to take a Set from
     * @param subSetBegin the beginning to the end
     * @param subSetEnd the end of the SubSet (max subString.size() will be checked internaly)
     */
    SubString::SubString(const SubString& subString, unsigned int subSetBegin, unsigned int subSetEnd)
    {
        for (unsigned int i = subSetBegin; i < subString.size() && i < subSetEnd; i++)
        {
            this->strings.push_back(subString[i]);
            this->bInSafemode.push_back(subString.isInSafemode(i));
        }
    }

    /**
     * @brief creates a Substring from a count and values set.
     * @param argc: the Arguments Count.
     * @param argv: Argument Values.
     */
    SubString::SubString(unsigned int argc, const char** argv)
    {
        for(unsigned int i = 0; i < argc; ++i)
        {
            this->strings.push_back(std::string(argv[i]));
            this->bInSafemode.push_back(false);
        }
    }

    /**
     * @brief removes the object from memory
     */
    SubString::~SubString()
    { }

    /** @brief An empty String */
    // const std::string SubString::emptyString = "";
    /** @brief Helper that gets you a String consisting of all White Spaces */
    const std::string SubString::WhiteSpaces = " \n\t";
    /** @brief Helper that gets you a String consisting of all WhiteSpaces and the Comma */
    const std::string SubString::WhiteSpacesWithComma = " \n\t,";
    /** An Empty SubString */
    const SubString SubString::NullSubString = SubString();

    /**
     * @brief stores the Value of subString in this SubString
     * @param subString will be copied into this String.
     * @returns this SubString.
     */
    SubString& SubString::operator=(const SubString& subString)
    {
        this->strings = subString.strings;
        this->bInSafemode = subString.bInSafemode;
        return *this;
    }


    /**
     * @brief comparator.
     * @param subString the SubString to compare against this one.
     * @returns true if the Stored Strings match
     */
    bool SubString::operator==(const SubString& subString) const
    {
        return ((this->strings == subString.strings) && (this->bInSafemode == subString.bInSafemode));
    }

    /**
     * @brief comparator.
     * @param subString the SubString to compare against this one.
     * @returns true if the Stored Strings match
     */
    bool SubString::compare(const SubString& subString) const
    {
        return (*this == subString);
    }

    /**
     * @brief comparator.
     * @param subString the SubString to compare against this one.
     * @param length how many entries to compare. (from 0 to length)
     * @returns true if the Stored Strings match
     */
    bool SubString::compare(const SubString& subString, unsigned int length) const
    {
        if (length > this->size() || length > subString.size())
            return false;

        for (unsigned int i = 0; i < length; i++)
            if ((this->strings[i] != subString.strings[i]) || (this->bInSafemode[i] != subString.bInSafemode[i]))
                return false;
        return true;
    }


    /**
     * @brief append operator
     * @param subString the String to append.
     * @returns a SubString where this and subString are appended.
     */
    SubString SubString::operator+(const SubString& subString) const
    {
        return SubString(*this) += subString;
    }


    /**
     * @brief append operator.
     * @param subString append subString to this SubString.
     * @returns this substring appended with subString
     */
    SubString& SubString::operator+=(const SubString& subString)
    {
        for (unsigned int i = 0; i < subString.size(); i++)
        {
            this->strings.push_back(subString[i]);
            this->bInSafemode.push_back(subString.isInSafemode(i));
        }
        return *this;
    }


    /**
     * @brief Split the String at
     * @param string where to split
     * @param splitter delimiter.
     */
    unsigned int SubString::split(const std::string& string, char splitter)
    {
        this->strings.clear();
        this->bInSafemode.clear();
        char split[2];
        split[0] = splitter;
        split[1] = '\0';
        SubString::splitLine(this->strings, this->bInSafemode, string, split);
        return strings.size();
    }


    /**
     * @brief Splits a String into multiple splitters.
     * @param string the String to split
     * @param delimiters multiple set of characters at what to split. (delimiters)
     * @param delimiterNeighbours: Neighbours to the Delimiters that will be erased too.
     * @param emptyEntries: If empty entries are added to the List of SubStrings
     * @param escapeChar The Escape Character that overrides splitters commends and so on...
     * @param safemode_char within these characters splitting won't happen
     * @param comment_char the Comment character.
     */
    unsigned int SubString::split(const std::string& string,
                                  const std::string& delimiters, const std::string& delimiterNeighbours, bool emptyEntries,
                                  char escapeChar, bool removeExcapeChar, char safemode_char, bool removeSafemodeChar,
                                  char openparenthesis_char, char closeparenthesis_char, bool removeParenthesisChars, char comment_char)
    {
        this->strings.clear();
        this->bInSafemode.clear();
        SubString::splitLine(this->strings, this->bInSafemode, string, delimiters, delimiterNeighbours, emptyEntries, escapeChar, removeExcapeChar, safemode_char, removeSafemodeChar, openparenthesis_char, closeparenthesis_char, removeParenthesisChars, comment_char);
        return this->strings.size();
    }


    /**
     * @brief joins together all Strings of this Substring.
     * @param delimiter the String between the subStrings.
     * @returns the joined String.
     */
    std::string SubString::join(const std::string& delimiter) const
    {
        if (!this->strings.empty())
        {
            std::string retVal = this->strings[0];
            for (unsigned int i = 1; i < this->strings.size(); i++)
                retVal += delimiter + this->strings[i];
            return retVal;
        }
        else
            return "";
    }


    /**
     * @brief creates a SubSet of a SubString.
     * @param subSetBegin the beginning to the end
     * @returns the SubSet
     *
     * This function is added for your convenience, and does the same as
     * SubString::SubString(const SubString& subString, unsigned int subSetBegin)
     */
    SubString SubString::subSet(unsigned int subSetBegin) const
    {
        return SubString(*this, subSetBegin);
    }


    /**
     * @brief creates a SubSet of a SubString.
     * @param subSetBegin the beginning to
     * @param subSetEnd the end of the SubSet to select (if bigger than subString.size() it will be downset.)
     * @returns the SubSet
     *
     * This function is added for your convenience, and does the same as
     * SubString::SubString(const SubString& subString, unsigned int subSetBegin)
     */
    SubString SubString::subSet(unsigned int subSetBegin, unsigned int subSetEnd) const
    {
        return SubString(*this, subSetBegin, subSetEnd);
    }


    /**
     * @brief splits line into tokens and stores them in ret.
     * @param ret the Array, where the Splitted strings will be stored in
     * to the beginning of the current token is stored
     * @param line the inputLine to split
     * @param delimiters a String of Delimiters (here the input will be splitted)
     * @param delimiterNeighbours Naighbours to the Delimitter, that will be removed if they are to the left or the right of a Delimiter.
     * @param emptyEntries: if empty Strings are added to the List of Strings.
     * @param escape_char: Escape carater (escapes splitters)
     * @param safemode_char: the beginning of the safemode is marked with this
     * @param removeSafemodeChar removes the safemode_char from the beginning and the ending of a token
     * @param openparenthesis_char the beginning of a safemode is marked with this
     * @param closeparenthesis_char the ending of a safemode is marked with this
     * @param removeParenthesisChars removes the parenthesis from the beginning and the ending of a token
     * @param comment_char: the beginning of a comment is marked with this: (until the end of a Line)
     * @param start_state: the Initial state on how to parse the String.
     * @return SPLIT_LINE_STATE the parser was in when returning
     *
     * This is the Actual Splitting Algorithm from Clemens Wacha
     * Supports delimiters, escape characters,
     * ignores special  characters between safemode_char and between comment_char and linend '\n'.
     */
    SubString::SPLIT_LINE_STATE
    SubString::splitLine(std::vector<std::string>& ret,
                         std::vector<bool>& bInSafemode,
                         const std::string& line,
                         const std::string& delimiters,
                         const std::string& delimiterNeighbours,
                         bool emptyEntries,
                         char escape_char,
                         bool removeExcapeChar,
                         char safemode_char,
                         bool removeSafemodeChar,
                         char openparenthesis_char,
                         char closeparenthesis_char,
                         bool removeParenthesisChars,
                         char comment_char,
                         SPLIT_LINE_STATE start_state)
    {
        SPLIT_LINE_STATE state = start_state;
        unsigned int i = 0;
        unsigned int fallBackNeighbours = 0;

        std::string token;
        bool inSafemode = false;

        if(start_state != SL_NORMAL && ret.size() > 0)
        {
            token = ret[ret.size()-1];
            ret.pop_back();
        }
        if(start_state != SL_NORMAL && bInSafemode.size() > 0)
        {
            inSafemode = bInSafemode[bInSafemode.size()-1];
            bInSafemode.pop_back();
        }

        while(i < line.size())
        {
            switch(state)
            {
            case SL_NORMAL:
                if(line[i] == escape_char)
                {
                    state = SL_ESCAPE;
                    if (!removeExcapeChar)
                        token += line[i];
                }
                else if(line[i] == safemode_char)
                {
                    state = SL_SAFEMODE;
                    inSafemode = true;
                    if (!removeSafemodeChar)
                        token += line[i];
                }
                else if(line[i] == openparenthesis_char)
                {
                    state = SL_PARENTHESES;
                    inSafemode = true;
                    if (!removeParenthesisChars)
                        token += line[i];
                }
                else if(line[i] == comment_char)
                {
                    if (fallBackNeighbours > 0)
                        token = token.substr(0, token.size() - fallBackNeighbours);
                    /// FINISH
                    if(emptyEntries || token.size() > 0)
                    {
                        ret.push_back(token);
                        token.clear();
                        bInSafemode.push_back(inSafemode);
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
                    /// FINISH
                    if(emptyEntries || token.size() > 0)
                    {
                        ret.push_back(token);
                        token.clear();
                        bInSafemode.push_back(inSafemode);
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
                            i++;
                            continue;
                        }
                    }
                    else
                        fallBackNeighbours = 0;
                    token += line[i];       // EAT
                }
                break;
            case SL_ESCAPE:
                if (!removeSafemodeChar)
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
                if(line[i] == safemode_char)
                {
                    state = SL_NORMAL;
                    if (!removeSafemodeChar)
                        token += line[i];
                }
                else if(line[i] == escape_char)
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
                if(line[i] == closeparenthesis_char)
                {
                    state = SL_NORMAL;
                    if (!removeParenthesisChars)
                        token += line[i];
                }
                else if(line[i] == escape_char)
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
                    /// FINISH
                    if(token.size() > 0)
                    {
                        ret.push_back(token);
                        token.clear();
                        bInSafemode.push_back(inSafemode);
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
            i++;
        }

        /// FINISH
        if (fallBackNeighbours > 0)
            token = token.substr(0, token.size() - fallBackNeighbours);
        if(emptyEntries || token.size() > 0)
        {
            ret.push_back(token);
            token.clear();
            bInSafemode.push_back(inSafemode);
            inSafemode = false;
        }
        return(state);
    }


    /**
     * @brief Some nice debug information about this SubString
     */
    void SubString::debug() const
    {
        printf("Substring-information::count=%zd ::", this->strings.size());
        for (unsigned int i = 0; i < this->strings.size(); i++)
            printf("s%d='%s'::", i, this->strings[i].c_str());
        printf("\n");
    }
}
