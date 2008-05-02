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

#include "String.h"

#include <cctype>
#include <iostream>

/**
    @brief Removes all whitespaces from a string.
    @param str The string to strip
*/
void strip(std::string* str)
{
    unsigned int pos;
    while ((pos = (*str).find(" ")) < (*str).length())
        (*str).erase(pos, 1);
    while ((pos = (*str).find("\t")) < (*str).length())
        (*str).erase(pos, 1);
}

/**
    @brief Returns a copy of a string without whitespaces.
    @param str The string to strip
    @return The stripped line
*/
std::string getStripped(const std::string& str)
{
    std::string output = std::string(str);
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
    unsigned int pos1 = 0;
    int pos2 = str.size() - 1;
    for (; pos1 < str.size() && (str[pos1] == ' ' || str[pos1] == '\t' || str[pos1] == '\n'); pos1++);
    for (; pos2 > 0          && (str[pos2] == ' ' || str[pos2] == '\t' || str[pos2] == '\n'); pos2--);
    return str.substr(pos1, pos2 - pos1 + 1);
}

/**
    @brief Returns the position of the next quote in the string, starting with start.
    @param str The string
    @param start The startposition
    @return The position of the next quote (std::string::npos if there is no next quote)
*/
unsigned int getNextQuote(const std::string& str, unsigned int start)
{
    unsigned int quote = start - 1;

    while ((quote = str.find('\"', quote + 1)) != std::string::npos)
    {
        unsigned int backslash = quote;
        unsigned int numbackslashes = 0;
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
bool isBetweenQuotes(const std::string& str, unsigned int pos)
{
    if (pos == std::string::npos)
        return false;

    unsigned int quotecount = 0;
    unsigned int quote = 0;
    while ((quote = getNextQuote(str, quote)) < pos)
    {
        quotecount++;
    }

    if (quote == std::string::npos)
        return false;

    return ((quotecount % 2) == 1);
}

/**
    @brief Returns true if the string contains something like '..."between quotes"...'
    @param The string
    @return True if there is something between quotes
*/
bool hasStringBetweenQuotes(const std::string& str)
{
    unsigned int pos1 = getNextQuote(str, 0);
    unsigned int pos2 = getNextQuote(str, pos1 + 1);
    return (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1);
}

/**
    @brief If the string contains something like '..."between quotes"...' then 'between quotes' gets returned (without quotes).
    @param The string
    @param The string between the quotes
*/
std::string getStringBetweenQuotes(const std::string& str)
{
    unsigned int pos1 = getNextQuote(str, 0);
    unsigned int pos2 = getNextQuote(str, pos1 + 1);
    if (pos1 != std::string::npos && pos2 != std::string::npos)
        return str.substr(pos1, pos2 - pos1 + 1);
    else
        return "";
}

/**
    @brief Removes enclosing quotes if available.
    @brief str The string to strip
    @return The string with removed quotes
*/
std::string stripEnclosingQuotes(const std::string& str)
{
    unsigned int start = std::string::npos;
    unsigned int end = 0;

    for (unsigned int pos = 0; (pos < str.size()) && (pos < std::string::npos); pos++)
    {
        if (str[pos] == '"')
        {
            start = pos;
            break;
        }

        if ((str[pos] != ' ') && (str[pos] != '\t') && (str[pos] != '\n'))
            return str;
    }

    for (unsigned int pos = str.size() - 1; pos < std::string::npos; pos--)
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
    @brief Determines if a string in is a comment.
    @param str The string to check
    @return True = it's a comment

    A comment is defined by a leading '#', '%', ';' or '//'.
*/
bool isComment(const std::string& str)
{
    // Strip the line, whitespaces are disturbing
    std::string teststring = getStripped(str);

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
    std::string temp = getStripped(str);
    return ((temp == "") || (temp.size() == 0));
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

std::string addSlashes(const std::string& str)
{
    std::string output = str;

    for (unsigned int pos = 0; (pos = output.find('\\', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\\\"); }
    for (unsigned int pos = 0; (pos = output.find('\n', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\n"); }
    for (unsigned int pos = 0; (pos = output.find('\t', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\t"); }
    for (unsigned int pos = 0; (pos = output.find('\v', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\v"); }
    for (unsigned int pos = 0; (pos = output.find('\b', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\b"); }
    for (unsigned int pos = 0; (pos = output.find('\r', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\r"); }
    for (unsigned int pos = 0; (pos = output.find('\f', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\f"); }
    for (unsigned int pos = 0; (pos = output.find('\a', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\a"); }
    for (unsigned int pos = 0; (pos = output.find('"', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\\""); }
    for (unsigned int pos = 0; (pos = output.find('\0', pos)) < std::string::npos; pos += 2) { output.replace(pos, 1, "\\0"); }

    return output;
}

std::string removeSlashes(const std::string& str)
{
    if (str.size() <= 1)
        return str;

    std::string output = "";
    for (unsigned int pos = 0; pos < str.size() - 1; )
    {
        if (str[pos] == '\\')
        {
            if (str[pos + 1] == '\\') { output += '\\'; pos += 2; continue; }
            else if (str[pos + 1] == 'n') { output += '\n'; pos += 2; continue; }
            else if (str[pos + 1] == 't') { output += '\t'; pos += 2; continue; }
            else if (str[pos + 1] == 'v') { output += '\v'; pos += 2; continue; }
            else if (str[pos + 1] == 'b') { output += '\b'; pos += 2; continue; }
            else if (str[pos + 1] == 'r') { output += '\r'; pos += 2; continue; }
            else if (str[pos + 1] == 'f') { output += '\f'; pos += 2; continue; }
            else if (str[pos + 1] == 'a') { output += '\a'; pos += 2; continue; }
            else if (str[pos + 1] == '"') { output += '"'; pos += 2; continue; }
            else if (str[pos + 1] == '0') { output += '\0'; pos += 2; continue; }
        }
        output += str[pos];
        pos++;
        if (pos == str.size() - 1)
            output += str[pos];
    }

    return output;
}

/**
    @brief Replaces each char between A and Z with its lowercase equivalent.
    @param str The string to convert
*/
void lowercase(std::string* str)
{
    for (unsigned int i = 0; i < str->size(); ++i)
    {
        (*str)[i] = (char)tolower((*str)[i]);
    }
}

/**
    @brief Returns a copy of the given string without uppercase chars.
    @param str The string
    @return The copy
*/
std::string getLowercase(const std::string& str)
{
    std::string output = std::string(str);
    lowercase(&output);
    return output;
}

/**
    @brief Replaces each char between a and z with its uppercase equivalent.
    @param str The string to convert
*/
void uppercase(std::string* str)
{
    for (unsigned int i = 0; i < str->size(); ++i)
    {
        (*str)[i] = (char)toupper((*str)[i]);
    }
}

/**
    @brief Returns a copy of the given string without lowercase chars.
    @param str The string
    @return The copy
*/
std::string getUppercase(const std::string& str)
{
    std::string output = std::string(str);
    uppercase(&output);
    return output;
}

/**
    @brief compares two strings without ignoring the case
    @param s1 first string
    @param s2 second string
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
    @brief compares two strings without ignoring the case
    @param s1 first string
    @param s2 second string
    @param len how far from the beginning to start.
*/
int nocaseCmp(const std::string& s1, const std::string& s2, unsigned int len)
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
unsigned int getCommentPosition(const std::string& str)
{
    return getNextCommentPosition(str, 0);
}

/**
    @brief Returns the position of the next comment-symbol, starting with start.
    @param str The string
    @param start The startposition
    @return The position
*/
unsigned int getNextCommentPosition(const std::string& str, unsigned int start)
{
    for (unsigned int i = start; i < str.size(); i++)
        if (isComment(str.substr(i)))
            return i;

    return std::string::npos;
}
