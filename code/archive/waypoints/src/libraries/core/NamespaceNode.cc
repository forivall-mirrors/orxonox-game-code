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

#include "NamespaceNode.h"
#include "util/Output.h"

namespace orxonox
{
    NamespaceNode::NamespaceNode(const std::string& name, NamespaceNode* parent)
    {
        this->name_ = name;
        this->parent_ = parent;
        this->bRoot_ = false;
        this->bHidden_ = false;
    }

    NamespaceNode::~NamespaceNode()
    {
        for (std::map<std::string, NamespaceNode*>::iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); )
            delete (it++)->second;
    }

    std::set<NamespaceNode*> NamespaceNode::getNodeRelative(const std::string& name)
    {
        std::set<NamespaceNode*> nodes;

        if (name.empty())
        {
            nodes.insert(this);
        }
        else
        {
            size_t pos = name.find("::");
            std::string firstPart = name;
            std::string secondPart;

            if (pos != std::string::npos)
            {
                firstPart = name.substr(0, pos);
                secondPart = name.substr(pos + 2, std::string::npos);
            }

            if (firstPart == "..")
            {
                if (this->bRoot_)
                {
                    orxout(internal_warning) << "Can't go to enclosing namespace with '..' operator in namespace " << this->name_ << ", namespace is root." << endl;
                    nodes = this->getNodeRelative(secondPart);
                }
                else if (!this->parent_)
                {
                    orxout(internal_warning) << "Can't go to enclosing namespace with '..' operator in namespace " << this->name_ << ", no parent namespace set." << endl;
                    nodes = this->getNodeRelative(secondPart);
                }
                else
                {
                    nodes = this->parent_->getNodeRelative(secondPart);
                }
            }
            else if (name.find('@') != 0)
            {
                std::map<std::string, NamespaceNode*>::iterator it = this->subnodes_.find(firstPart);
                if (it == this->subnodes_.end())
                    it = this->subnodes_.insert(this->subnodes_.begin(), std::pair<std::string, NamespaceNode*>(firstPart, new NamespaceNode(firstPart, this)));

                if (it->second->isHidden())
                {
                    orxout(internal_warning) << "Subnamespace '" << firstPart << "' in namespace '" << this->name_ << "' is hidden and can't be accessed." << endl;
                    nodes.insert(this);
                }
                else
                {
                    nodes = it->second->getNodeRelative(secondPart);
                }
            }
            else
            {
                bool bFoundMatchingNamespace = false;

                for (std::map<std::string, NamespaceNode*>::iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); ++it)
                {
                    if (it->first.find(firstPart) == (it->first.size() - firstPart.size()))
                    {
                        std::set<NamespaceNode*> temp2 = it->second->getNodeRelative(secondPart);
                        nodes.insert(temp2.begin(), temp2.end());
                        bFoundMatchingNamespace = true;
                    }
                }

                if (!bFoundMatchingNamespace)
                {
                    orxout(internal_warning) << "No file included with name '" << firstPart.substr(1, std::string::npos) << "' at this part of the level file, using parent namespace instead." << endl;
                    nodes = this->getNodeRelative(secondPart);
                }
            }
        }

        return nodes;
    }

    bool NamespaceNode::includes(const NamespaceNode* ns) const
    {
        if (ns == this)
        {
            return true;
        }
        else
        {
            for (std::map<std::string, NamespaceNode*>::const_iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); ++it)
                if (it->second->includes(ns))
                    return true;
        }

        return false;
    }

    std::string NamespaceNode::toString() const
    {
        std::string output = this->name_;

        if (this->subnodes_.size() > 0)
        {
            output += " (";

            int i = 0;
            for (std::map<std::string, NamespaceNode*>::const_iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); i++, ++it)
            {
                if (i > 0)
                    output += ", ";

                output += it->second->toString();
            }

            output += ')';
        }

        return output;
    }

    std::string NamespaceNode::toString(const std::string& indentation) const
    {
        std::string output = (indentation + this->name_ + '\n');

        for (std::map<std::string, NamespaceNode*>::const_iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); ++it)
            output += it->second->toString(indentation + "  ");

        return output;
    }
}
