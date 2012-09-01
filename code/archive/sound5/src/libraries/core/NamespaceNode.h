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

#ifndef _NamespaceNode_H__
#define _NamespaceNode_H__

#include "CorePrereqs.h"

#include <map>
#include <set>
#include <string>

namespace orxonox
{
    class NamespaceNode
    {
        public:
            NamespaceNode(const std::string& name, NamespaceNode* parent = 0);
            ~NamespaceNode();

            std::set<NamespaceNode*> getNodeRelative(const std::string& name);

            void setRoot(bool bRoot) { this->bRoot_ = bRoot; }
            void setHidden(bool bHidden) { this->bHidden_ = bHidden; }

            bool isRoot() const { return this->bRoot_; }
            bool isHidden() const { return this->bHidden_; }

            bool includes(const NamespaceNode*) const;

            std::string toString() const;
            std::string toString(const std::string& indentation) const;

        private:
            std::string name_;
            NamespaceNode* parent_;
            std::map<std::string, NamespaceNode*> subnodes_;
            bool bHidden_;
            bool bRoot_;
    };
}

#endif /* _NamespaceNode_H__ */
