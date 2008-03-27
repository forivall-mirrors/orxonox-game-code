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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Benjamin Grauer
 *
 */

#include "String.h"

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
    if (teststring[0] == '#' || teststring[0] == '%' || teststring[0] == ';' || (teststring[0] == '/' && teststring[0] == '/'))
        return true;

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

/**
    @brief Replaces each char between A and Z with its lowercase equivalent.
    @param str The string to convert
*/
void lowercase(std::string* str)
{
    static unsigned const char difference_between_A_and_a = 'A' - 'a';

    for (std::string::iterator it = (*str).begin(); it != (*str).end(); ++it)
        if ((*it) >= 'A' && (*it) <= 'Z')
            (*it) -= difference_between_A_and_a;
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
    static unsigned const char difference_between_A_and_a = 'A' - 'a';

    for (std::string::iterator it = (*str).begin(); it != (*str).end(); ++it)
        if ((*it) >= 'a' && (*it) <= 'z')
            (*it) += difference_between_A_and_a;
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
 * @brief compares two strings without ignoring the case
 * @param s1 first string
 * @param s2 second string
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
 * @brief compares two strings without ignoring the case
 * @param s1 first string
 * @param s2 second string
 * @param len how far from the beginning to start.
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
