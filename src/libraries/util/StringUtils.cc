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
#include <ctime>
#include <boost/scoped_array.hpp>
#include "Convert.h"
#include "Math.h"

namespace orxonox
{
    /// A blank string (""). Used to return a blank string by reference.
    std::string BLANKSTRING;

    /// Returns a string of a unique number. This function is guaranteed to never return the same string twice.
    std::string getUniqueNumberString()
    {
        return multi_cast<std::string>(getUniqueNumber());
    }

    /// Removes all whitespaces from a string.
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

    /// Returns a copy of a string without whitespaces.
    std::string getStripped(const std::string& str)
    {
        std::string output(str);
        strip(&output);
        return output;
    }

    /// Returns a copy of a string without trailing whitespaces.
    std::string removeTrailingWhitespaces(const std::string& str)
    {
        size_t pos1 = 0;
        int pos2 = static_cast<int>(str.size() - 1);
        for (; pos1 < str.size() && (str[pos1] == ' ' || str[pos1] == '\t' || str[pos1] == '\n'); pos1++);
        for (; pos2 > 0         && (str[pos2] == ' ' || str[pos2] == '\t' || str[pos2] == '\n'); pos2--);
        return str.substr(pos1, pos2 - pos1 + 1);
    }

    /// Splits a given string by a delimiter and stores it in an output vector. See @ref SubString for a more sophisticated implementation.
    void vectorize(const std::string& str, char delimiter, std::vector<std::string>* output)
    {
        output->clear();
        for (size_t start = 0, end = 0; end != std::string::npos; start = end + 1)
        {
            end = str.find_first_of(delimiter, start);
            output->push_back(str.substr(start, end - start));
        }
    }

    /**
        @brief Returns the position of the next quotation mark in the string, starting with start. Escaped quotation marks (with \ in front) are not considered.
        @param str The string
        @param start The first position to look at
        @return The position of the next quotation mark (@c std::string::npos if there is none)
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
        @brief Returns true if pos is between two quotation marks.
        @param str The string
        @param pos The position to check
        @return True if pos is between two quotation marks
    */
    bool isBetweenQuotes(const std::string& str, size_t pos)
    {
        if (pos == std::string::npos)
            return false;

        size_t quotecount = 0;
        size_t quote = static_cast<size_t>(-1);
        while ((quote = getNextQuote(str, quote + 1)) < pos)
            quotecount++;

        if (quote == pos)
            return false;
        if (quote == std::string::npos)
            return false;

        return ((quotecount % 2) == 1);
    }

    /// Returns true if the string contains something like '..."between quotaton marks"...'.
    bool hasStringBetweenQuotes(const std::string& str)
    {
        size_t pos1 = getNextQuote(str, 0);
        size_t pos2 = getNextQuote(str, pos1 + 1);
        return (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1);
    }

    /// If the string contains something like '..."between quotaton marks"...' then 'between quotaton marks' gets returned, otherwise "".
    std::string getStringBetweenQuotes(const std::string& str)
    {
        size_t pos1 = getNextQuote(str, 0);
        size_t pos2 = getNextQuote(str, pos1 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos)
            return str.substr(pos1 + 1, pos2 - pos1 - 1);
        else
            return "";
    }

    /**
        @brief Removes enclosing quotation marks if available (including whitespaces at the outside of the quotation marks).
        @return The striped string without quotation marks
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
        @brief Removes enclosing braces '{' and '}' (the braces must be exactly on the beginning and the end of the string).
        @return The striped string without braces
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

    /// Determines if a string is empty (contains only whitespaces).
    bool isEmpty(const std::string& str)
    {
        return getStripped(str).empty();
    }

    /**
        @brief Adds backslashes to the given string which makes special chars visible. Existing slashes will be doubled.

        This function converts all special chars like line breaks, tabs, quotation marks etc. into
        a human readable format by adding a backslash. So for example "\n" will be converted to
        "\\" + "n".

        This is usually used when a string is written to a file.

        @see removeSlashes
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

        This function removes all backslashes and converts the human readable equivalents of
        special chars like "\\" + "n" into their real meaning (in this case a line break or "\n").

        This is usually used when reading a string from a file.

        @see addSlashes
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

    /// Replaces each char between A and Z with its lowercase equivalent.
    void lowercase(std::string* str)
    {
        for (size_t i = 0; i < str->size(); ++i)
        {
            (*str)[i] = static_cast<char>(tolower((*str)[i]));
        }
    }

    /// Returns a copy of the given string where all chars are converted to lowercase.
    std::string getLowercase(const std::string& str)
    {
        std::string output(str);
        lowercase(&output);
        return output;
    }

    /// Replaces each char between a and z with its uppercase equivalent.
    void uppercase(std::string* str)
    {
        for (size_t i = 0; i < str->size(); ++i)
        {
            (*str)[i] = static_cast<char>(toupper((*str)[i]));
        }
    }

    /// Returns a copy of the given string where all chars are converted to uppercase.
    std::string getUppercase(const std::string& str)
    {
        std::string output(str);
        uppercase(&output);
        return output;
    }

    /**
        @brief Compares two strings ignoring different casing.
        @return s1 == s1 -> returns 0 / s1 < s2 -> returns -1 / s1 >= s2 -> returns 1
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
        @brief Compares the first @a len chars of two strings ignoring different casing.
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

    /// Returns true if the string contains a comment, introduced by #, %, ; or //.
    bool hasComment(const std::string& str)
    {
        return (getNextCommentPosition(str) != std::string::npos);
    }

    /// If the string contains a comment, the comment gets returned (including the comment symbol and white spaces in front of it), an empty string otherwise.
    std::string getComment(const std::string& str)
    {
        size_t pos = getNextCommentPosition(str);
        if (pos == std::string::npos)
            return "";
        else
            return str.substr(pos);
    }

    /**
        @brief Returns the beginning of the next comment including whitespaces in front of the comment symbol.
        @param str The string
        @param start The first position to look at
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
    size_t replaceCharacters(std::string& str, char target, char replacement)
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

    /**
        @brief Calculates the Levenshtein distance between two strings.

        The Levenshtein distance is defined by the number of transformations needed to convert str1
        into str2. Possible transformations are substituted, added, or removed characters.
    */
    unsigned int getLevenshteinDistance(const std::string& str1, const std::string& str2)
    {
        size_t cols = str1.size() + 1;
        size_t rows = str2.size() + 1;
        boost::scoped_array<int> matrix(new int[rows * cols]);

        for (size_t r = 0; r < rows; ++r)
            for (size_t c = 0; c < cols; ++c)
                matrix[r*cols + c] = 0;

        for (size_t i = 1; i < cols; ++i)
            matrix[0*cols + i] = i;
        for (size_t i = 1; i < rows; ++i)
            matrix[i*cols + 0] = i;

        for (size_t r = 1; r < rows; ++r)
            for (size_t c = 1; c < cols; ++c)
                matrix[r*cols + c] = (str1[c-1] != str2[r-1]);

        for (size_t r = 1; r < rows; ++r)
            for (size_t c = 1; c < cols; ++c)
                matrix[r*cols + c] = std::min(std::min(matrix[(r-1)*cols + c] + 1,
                                                       matrix[r*cols + c-1] + 1),
                                              matrix[(r-1)*cols + c-1] + (str1[c-1] != str2[r-1]));

        return matrix[(rows-1)*cols + cols-1];
    }

    /**
    @brief
        Get a timestamp for the curent time instant.
    @return
        Returns a string with the timestamp.
    */
    std::string getTimestamp(void)
    {
        struct tm *pTime;
        time_t ctTime; std::time(&ctTime);
        pTime = std::localtime( &ctTime );
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << (pTime->tm_mon + 1)
            << std::setw(2) << std::setfill('0') << pTime->tm_mday
            << std::setw(2) << std::setfill('0') << (pTime->tm_year + 1900)
            << "_" << std::setw(2) << std::setfill('0') << pTime->tm_hour
            << std::setw(2) << std::setfill('0') << pTime->tm_min
            << std::setw(2) << std::setfill('0') << pTime->tm_sec;
        return oss.str();
    }
}
