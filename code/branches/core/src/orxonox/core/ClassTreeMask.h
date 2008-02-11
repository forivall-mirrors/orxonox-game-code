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
 *      ...
 *
 */

#ifndef _ClassTreeMask_H__
#define _ClassTreeMask_H__

#include <list>

#include "CorePrereqs.h"

namespace orxonox
{
    class ClassTreeMaskNode
    {
        friend class ClassTreeMask;

        public:
            ClassTreeMaskNode(const Identifier* subclass, bool bIncluded = true);
            ~ClassTreeMaskNode();

            void include();
            void exclude();
            void setIncluded(bool bIncluded);

            void addSubnode(ClassTreeMaskNode* subnode);

            bool isIncluded() const;
            bool isExcluded() const;

            const Identifier* getClass() const;

        private:
            const Identifier* subclass_;
            bool bIncluded_;
            std::list<ClassTreeMaskNode*> subnodes_;
    };

    class ClassTreeMask
    {
        public:
            ClassTreeMask();
            ~ClassTreeMask();

            void include(const Identifier* subclass);
            void exclude(const Identifier* subclass);
            void add(const Identifier* subclass, bool bInclude);
            void reset();

            bool isIncluded(const Identifier* subclass);
            bool isExcluded(const Identifier* subclass);

        private:
            void add(ClassTreeMaskNode* node, const Identifier* subclass, bool bInclude);
            bool isIncluded(ClassTreeMaskNode* node, const Identifier* subclass);

            ClassTreeMaskNode* root_;
    };
}

#endif /* _ClassTreeMask_H__ */
