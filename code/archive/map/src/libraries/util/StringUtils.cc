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

/**
    @file
    @brief Implementation of several string manipulation functions.
*/

#include "StringUtils.h"

#include <cctype>
#include "Convert.h"
#include "Math.h"

namespace orxonox
{
    std::string BLANKSTRING;

    std::string getUniqueNumberString()
    {
        return multi_cast<std::string>(getUniqueNumber());
    }

    /**
        @brief Removes all whitespaces from a string.
        @param str The string to strip
    */
    void strip(std::string* str)
    {
        size_t pos;
        while ((pos = str->find(' ')) < str->length())
            str->erase(pos, 1);
        while ((pos = str->find('\t')) < str->length())
            str->erase(pos, 1);
        while ((pos = str->find('\n')) < str->length())
            str->erase(pos, 1);
    }

    /**
        @brief Returns a copy of a string without whitespaces.
        @param str The string to strip
        @return The stripped line
    */
    std::string getStripped(const std::string& str)
    {
        std::string output(str);
        strip(&output);
        return output;
    }

    /**
        @brief Returns a copy of a string without trailing whitespaces.
        @param str The string
        @return The modified copy
    */
    std::string removeTrailingWhitespaces(const std::string& str)
    {
        size_t pos1 = 0;
        int pos2 = static_cast<int>(str.size() - 1);
        for (; pos1 < str.size() && (str[pos1] == ' ' || str[pos1] == '\t' || str[pos1] == '\n'); pos1++);
        for (; pos2 > 0         && (str[pos2] == ' ' || str[pos2] == '\t' || str[pos2] == '\n'); pos2--);
        return str.substr(pos1, pos2 - pos1 + 1);
    }

    /**
        @brief Returns the position of the next quote in the string, starting with start.
        @param str The string
        @param start The startposition
        @return The position of the next quote (std::string::npos if there is no next quote)
    */
    size_t getNextQuote(const std::string& str, size_t start)
    {
        size_t quote = start - 1;

        while ((quote = str.find('"', quote + 1)) != std::string::npos)
        {
            size_t backslash = quote;
            size_t numbackslashes = 0;
            for (; backslash > 0; backslash--, numbackslashes++)
                if (str[backslash - 1] != '\\')
                    break;

            if (numbackslashes % 2 == 0)
                break;
        }

        return quote;
    }

    /**
        @brief Returns true if pos is between two quotes.
        @param str The string
        @param pos The position to check
        @return True if pos is between two quotes
    */
    bool isBetweenQuotes(const std::string& str, size_t pos)
    {
        if (pos == std::string::npos)
            return false;

        size_t quotecount = 0;
        size_t quote = static_cast<size_t>(-1);
        while ((quote = getNextQuote(str, quote + 1)) < pos)
        {
            if (quote == pos)
                return false;
            quotecount++;
        }

        if (quote == std::string::npos)
            return false;

        return ((quotecount % 2) == 1);
    }

    /**
        @brief Returns true if the string contains something like '..."between quotes"...'.
        @param The string
        @return True if there is something between quotes
    */
    bool hasStringBetweenQuotes(const std::string& str)
    {
        size_t pos1 = getNextQuote(str, 0);
        size_t pos2 = getNextQuote(str, pos1 + 1);
        return (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1);
    }

    /**
        @brief If the string contains something like '..."between quotes"...' then 'between quotes' gets returned (without quotes).
        @param The string
        @param The string between the quotes
    */
    std::string getStringBetweenQuotes(const std::string& str)
    {
        size_t pos1 = getNextQuote(str, 0);
        size_t pos2 = getNextQuote(str, pos1 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos)
            return str.substr(pos1, pos2 - pos1 + 1);
        else
            return "";
    }

    /**
        @brief Removes enclosing quotes if available (including whitespaces at the outside of the quotes).
        @brief str The string to strip
        @return The string with removed quotes
    */
    std::string stripEnclosingQuotes(const std::string& str)
    {
        size_t start = std::string::npos;
        size_t end = 0;

        for (size_t pos = 0; (pos < str.size()) && (pos < std::string::npos); pos++)
        {
            if (str[pos] == '"')
            {
                start = pos;
                break;
            }

            if ((str[pos] != ' ') && (str[pos] != '\t') && (str[pos] != '\n'))
                return str;
        }

        for (size_t pos = str.size() - 1; pos < std::string::npos; pos--)
        {
            if (str[pos] == '"')
            {
                end = pos;
                break;
            }

            if ((str[pos] != ' ') && (str[pos] != '\t') && (str[pos] != '\n'))
                return str;
        }

        if ((start != std::string::npos) && (end != 0))
            return str.substr(start + 1, end - start - 1);
        else
            return str;
    }

    /**
        @brief Removes enclosing {braces} (braces must be exactly on the beginning and the end of the string).
        @param str The string to strip
        @return The striped string
    */
    std::string stripEnclosingBraces(const std::string& str)
    {
        std::string output = str;

        while (output.size() >= 2 && output[0] == '{' && output[output.size() - 1] == '}')
            output = output.substr(1, output.size() - 2);

        return output;
    }

    /**
        @brief Determines if a string is a comment (starts with a comment-symbol).
        @param str The string to check
        @return True = it's a comment

        A comment is defined by a leading '#', '%', ';' or '//'.
    */
    bool isComment(const std::string& str)
    {
        // Strip the line, whitespaces are disturbing
        const std::string& teststring = getStripped(str);

        // There are four possible comment-symbols:
        //  1) #comment in script-language style
        //  2) %comment in matlab style
        //  3) ;comment in unreal tournament config-file style
        //  4) //comment in code style
        if (teststring.size() >= 2)
        {
            if (teststring[0] == '#' || teststring[0] == '%' || teststring[0] == ';' || (teststring[0] == '/' && teststring[1] == '/'))
                return true;
        }
        else if (teststring.size() == 1)
        {
            if (teststring[0] == '#' || teststring[0] == '%' || teststring[0] == ';')
                return true;
        }

        return false;
    }

    /**
        @brief Determines if a string is empty (contains only whitespaces).
        @param str The string to check
        @return True = it's empty
    */
    bool isEmpty(const std::string& str)
    {
        return getStripped(str).empty();
    }

    /**
        @brief Determines if a string contains only numbers and maximal one '.'.
        @param str The string to check
        @return True = it's a number
    */
    bool isNumeric(const std::string& str)
    {
        bool foundPoint = false;

        for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        {
            if (((*it) < '0' || (*it) > '9'))
            {
                if ((*it) != '.' && !foundPoint)
                    foundPoint = true;
                else
                    return false;
            }
        }

        return true;
    }

    /**
        @brief Adds backslashes to the given string which makes special chars visible. Existing slashes will be doubled.
        @param str The string to manipulate
        @return The string with added slashes
    */
    std::string addSlashes(const std::string& str)
    {
        std::string output(str.size() * 2, ' ');
        size_t i = 0;
        for (size_t pos = 0; pos < str.size(); ++pos)
        {
            switch (str[pos])
            {
            case '\\': output[i] = '\\'; output[i + 1] = '\\'; break;
            case '\n': output[i] = '\\'; output[i + 1] =  'n'; break;
            case '\t': output[i] = '\\'; output[i + 1] =  't'; break;
            case '\v': output[i] = '\\'; output[i + 1] =  'v'; break;
            case '\b': output[i] = '\\'; output[i + 1] =  'b'; break;
            case '\r': output[i] = '\\'; output[i + 1] =  'r'; break;
            case '\f': output[i] = '\\'; output[i + 1] =  'f'; break;
            case '\a': output[i] = '\\'; output[i + 1] =  'a'; break;
            case  '"': output[i] = '\\'; output[i + 1] =  '"'; break;
            case '\0': output[i] = '\\'; output[i + 1] =  '0'; break;
            default  : output[i] = str[pos]; ++i; continue;
            }
            i += 2;
        }
        output.resize(i);

        return output;
    }

    /**
        @brief Removes backslashes from the given string. Double backslashes are interpreted as one backslash.
        @param str The string to manipulate
        @return The string with removed slashes
    */
    std::string removeSlashes(const std::string& str)
    {
        if (str.size() <= 1)
            return str;

        std::string output(str.size(), ' ');
        size_t i = 0;
        size_t pos = 0;
        while (pos < str.size() - 1)
        {
            if (str[pos] == '\\')
            {
                switch (str[pos + 1])
                {
                case '\\': output[i] = '\\'; break;
                case  'n': output[i] = '\n'; break;
                case  't': output[i] = '\t'; break;
                case  'v': output[i] = '\v'; break;
                case  'b': output[i] = '\b'; break;
                case  'r': output[i] = '\r'; break;
                case  'f': output[i] = '\f'; break;
                case  'a': output[i] = '\a'; break;
                case  '"': output[i] =  '"'; break;
                case  '0': output[i] = '\0'; break;
                default: ++pos; continue;
                }
                pos += 2; ++i;
            }
            else
                output[i++] = str[pos++];
        }
        if (pos < str.size())
            output[i++] = str[pos];
        output.resize(i);

        return output;
    }

    /**
        @brief Replaces each char between A and Z with its lowercase equivalent.
        @param str The string to convert
    */
    void lowercase(std::string* str)
    {
        for (size_t i = 0; i < str->size(); ++i)
        {
            (*str)[i] = static_cast<char>(tolower((*str)[i]));
        }
    }

    /**
        @brief Returns a copy of the given string without uppercase chars.
        @param str The string
        @return The copy
    */
    std::string getLowercase(const std::string& str)
    {
        std::string output(str);
        lowercase(&output);
        return output;
    }

    /**
        @brief Replaces each char between a and z with its uppercase equivalent.
        @param str The string to convert
    */
    void uppercase(std::string* str)
    {
        for (size_t i = 0; i < str->size(); ++i)
        {
            (*str)[i] = static_cast<char>(toupper((*str)[i]));
        }
    }

    /**
        @brief Returns a copy of the given string without lowercase chars.
        @param str The string
        @return The copy
    */
    std::string getUppercase(const std::string& str)
    {
        std::string output(str);
        uppercase(&output);
        return output;
    }

    /**
        @brief Compares two strings ignoring different casing.
        @param s1 First string
        @param s2 Second string
    */
    int nocaseCmp(const std::string& s1, const std::string& s2)
    {
        std::string::const_iterator it1=s1.begin();
        std::string::const_iterator it2=s2.begin();

        //stop when either string's end has been reached
        while ( (it1!=s1.end()) && (it2!=s2.end()) )
        {
            if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
                // return -1 to indicate smaller than, 1 otherwise
                return (::toupper(*it1)  < ::toupper(*it2)) ? -1 : 1;
            //proceed to the next character in each string
            ++it1;
            ++it2;
        }
        size_t size1=s1.size(), size2=s2.size();// cache lengths
        //return -1,0 or 1 according to strings' lengths
        if (size1==size2)
            return 0;
        return (size1<size2) ? -1 : 1;
    }


    /**
        @brief Compares the first 'len' chars of two strings ignoring different casing.
        @param s1 First string
        @param s2 Second string
        @param len Maximal number of chars to compare
    */
    int nocaseCmp(const std::string& s1, const std::string& s2, size_t len)
    {
        if (len == 0)
            return 0;
        std::string::const_iterator it1=s1.begin();
        std::string::const_iterator it2=s2.begin();

        //stop when either string's end has been reached
        while ( (it1!=s1.end()) && (it2!=s2.end()) && len-- > 0)
        {
            if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
                // return -1 to indicate smaller than, 1 otherwise
                return (::toupper(*it1)  < ::toupper(*it2)) ? -1 : 1;
            //proceed to the next character in each string
            ++it1;
            ++it2;
        }
        return 0;
    }

    /**
        @brief Returns true if the string contains a comment, introduced by #, %, ; or //.
        @param str The string
        @return True if the string contains a comment
    */
    bool hasComment(const std::string& str)
    {
        return (getCommentPosition(str) != std::string::npos);
    }

    /**
        @brief If the string contains a comment, the comment gets returned (including the comment symbol), an empty string otherwise.
        @param str The string
        @return The comment
    */
    std::string getComment(const std::string& str)
    {
        return str.substr(getCommentPosition(str));
    }

    /**
        @brief If the string contains a comment, the position of the comment-symbol gets returned, std::string::npos otherwise.
        @param str The string
        @return The position
    */
    size_t getCommentPosition(const std::string& str)
    {
        return getNextCommentPosition(str, 0);
    }

    /**
        @brief Returns the position of the next comment-symbol, starting with start.
        @param str The string
        @param start The startposition
        @return The position
    */
    size_t getNextCommentPosition(const std::string& str, size_t start)
    {
        for (size_t i = start; i < str.size(); i++)
            if (isComment(str.substr(i)))
                return i;

        return std::string::npos;
    }

    /**
        @brief Replaces individual charaters
        @param str String to be manipulated
        @param target Character to be replaced
        @param replacement Replacement character
        @return Number of replacements
    */
    _UtilExport size_t replaceCharacters(std::string& str, char target, char replacement)
    {
        size_t j = 0;
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == target)
            {
                str[i] = replacement;
                ++j;
            }
        }
        return j;
    }
}
