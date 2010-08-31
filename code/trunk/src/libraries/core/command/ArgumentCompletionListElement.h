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

#ifndef _ArgumentCompletionListElement_H__
#define _ArgumentCompletionListElement_H__

#include "core/CorePrereqs.h"

#include <list>
#include <string>

namespace orxonox
{
    const int ACL_MODE_NORMAL    = 1;
    const int ACL_MODE_COMPARABLE = 2;
    const int ACL_MODE_DISPLAY   = 4;

    typedef std::list<ArgumentCompletionListElement> ArgumentCompletionList;

    class _CoreExport ArgumentCompletionListElement
    {
        public:
            ArgumentCompletionListElement(const std::string& normalcase) : mode_(ACL_MODE_NORMAL), normal_(normalcase) {}
            ArgumentCompletionListElement(const std::string& normalcase, const std::string& lowercase) : mode_(ACL_MODE_NORMAL | ACL_MODE_COMPARABLE), normal_(normalcase), comparable_(lowercase) {}
            ArgumentCompletionListElement(const std::string& normalcase, const std::string& lowercase, const std::string& display) : mode_(ACL_MODE_NORMAL | ACL_MODE_COMPARABLE | ACL_MODE_DISPLAY), normal_(normalcase), comparable_(lowercase), display_(display) {}

            const std::string& getString() const
                { return this->normal_; }
            const std::string& getComparable() const
                { return (this->mode_ & ACL_MODE_COMPARABLE) ? this->comparable_ : this->normal_; }
            const std::string& getDisplay() const
                { return (this->mode_ & ACL_MODE_DISPLAY) ? this->display_ : this->normal_; }

            bool hasComparable() const
                { return (this->mode_ & ACL_MODE_COMPARABLE); }
            bool hasDisplay() const
                { return (this->mode_ & ACL_MODE_DISPLAY); }

            bool operator<(const ArgumentCompletionListElement& other) const
                { return (this->getComparable() < other.getComparable()); }

        private:
            unsigned char mode_;
            std::string normal_;
            std::string comparable_;
            std::string display_;
    };
}

#endif /* _ArgumentCompletionListElement_H__ */
