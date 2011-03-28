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
    @file
    @ingroup Command ArgumentCompletion
    @brief Definition of ArgumentCompletionList, which is  used in @ref ArgumentCompletionFunctions.h "argument completion functions", and its element.
*/

#ifndef _ArgumentCompletionListElement_H__
#define _ArgumentCompletionListElement_H__

#include "core/CorePrereqs.h"

#include <list>
#include <string>

namespace orxonox
{
    const int ACL_MODE_NORMAL     = 1;  ///< A flag, used if there's a normal string
    const int ACL_MODE_COMPARABLE = 2;  ///< A flag, used if there's a different string used to compare
    const int ACL_MODE_DISPLAY    = 4;  ///< A flag, used if there's a different string used to be displayed

    typedef std::list<ArgumentCompletionListElement> ArgumentCompletionList;

    /**
        @brief This class is used in argument completion lists and contains up to three different strings, used in different situations.

        A list containing elements of type ArgumentCompletionListElement is returned by
        an @ref ArgumentCompletionFunctions.h "argument completion function". These elements
        are composed of up to three strings with different usage:
         - normal: What is used as the actual argument
         - comparable: What is used to compere the argument to the input of the user (usually lowercase, except for case-sensitive arguments)
         - display: This is displayed in the list of possible arguments - can differ from what is actually used for better readability

        @remarks An element with an empty ("") 'comparable' string will be ignored by the argument
        completion algorithms, but it's 'display' string will still be printed in the list. This
        can be used to add additional information, whitespaces, linebreaks or whatever is needed
        to format the output.
    */
    class _CoreExport ArgumentCompletionListElement
    {
        public:
            /// Constructor: Normal, comparable, and display string are all the same.
            ArgumentCompletionListElement(const std::string& normalcase) : mode_(ACL_MODE_NORMAL), normal_(normalcase) {}
            /// Constructor: Normal and display string are the same, a different (usually lowercase) string is used for comparison.
            ArgumentCompletionListElement(const std::string& normalcase, const std::string& lowercase) : mode_(ACL_MODE_NORMAL | ACL_MODE_COMPARABLE), normal_(normalcase), comparable_(lowercase) {}
            /// Constructor: Normal, comparable, and display are all different strings.
            ArgumentCompletionListElement(const std::string& normalcase, const std::string& lowercase, const std::string& display) : mode_(ACL_MODE_NORMAL | ACL_MODE_COMPARABLE | ACL_MODE_DISPLAY), normal_(normalcase), comparable_(lowercase), display_(display) {}

            /// Returns the normal string which is used as the actual argument.
            const std::string& getString() const
                { return this->normal_; }
            /// Returns the comparable string which is used to compare arguments and user input
            const std::string& getComparable() const
                { return (this->mode_ & ACL_MODE_COMPARABLE) ? this->comparable_ : this->normal_; }
            /// Returns the display string which is used in the displayed list of possible arguments
            const std::string& getDisplay() const
                { return (this->mode_ & ACL_MODE_DISPLAY) ? this->display_ : this->normal_; }

            /// Returns true if there's a different string for comparison.
            bool hasComparable() const
                { return (this->mode_ & ACL_MODE_COMPARABLE); }
            /// Returns true if there's a different string to display.
            bool hasDisplay() const
                { return (this->mode_ & ACL_MODE_DISPLAY); }

            /// Overloaded operator for usage in maps and sets.
            bool operator<(const ArgumentCompletionListElement& other) const
                { return (this->getComparable() < other.getComparable()); }

        private:
            unsigned char mode_;        ///< The flags
            std::string normal_;        ///< The normal string
            std::string comparable_;    ///< The comparable (usually lowercase) string
            std::string display_;       ///< The string to display
    };
}

#endif /* _ArgumentCompletionListElement_H__ */
