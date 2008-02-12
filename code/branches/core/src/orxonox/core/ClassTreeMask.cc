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

#include "ClassTreeMask.h"
#include "Identifier.h"
#include "BaseObject.h"

namespace orxonox
{
    // ###############################
    // ###    ClassTreeMaskNode    ###
    // ###############################
    ClassTreeMaskNode::ClassTreeMaskNode(const Identifier* subclass, bool bIncluded)
    {
        this->subclass_ = subclass;
        this->bIncluded_ = bIncluded;
    }

    ClassTreeMaskNode::~ClassTreeMaskNode()
    {
        for (std::list<ClassTreeMaskNode*>::iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); )
            delete (*(it++));
    }

    void ClassTreeMaskNode::include()
    {
        this->bIncluded_ = true;
    }

    void ClassTreeMaskNode::exclude()
    {
        this->bIncluded_ = false;
    }

    void ClassTreeMaskNode::setIncluded(bool bIncluded)
    {
        this->bIncluded_ = bIncluded;
    }

    void ClassTreeMaskNode::addSubnode(ClassTreeMaskNode* subnode)
    {
        this->subnodes_.insert(this->subnodes_.end(), subnode);
    }

    bool ClassTreeMaskNode::isIncluded() const
    {
        return this->bIncluded_;
    }

    bool ClassTreeMaskNode::isExcluded() const
    {
        return (!this->bIncluded_);
    }

    const Identifier* ClassTreeMaskNode::getClass() const
    {
        return this->subclass_;
    }


    // ###############################
    // ###  ClassTreeMaskIterator  ###
    // ###############################
    ClassTreeMaskIterator::ClassTreeMaskIterator(ClassTreeMaskNode* node)
    {
        std::list<ClassTreeMaskNode*>::iterator it = this->rootlist_.insert(this->rootlist_.end(), node);
        this->nodes_.push(std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator>(it, this->rootlist_.end()));
    }

    ClassTreeMaskIterator::~ClassTreeMaskIterator()
    {
    }

    ClassTreeMaskIterator& ClassTreeMaskIterator::operator++()
    {
        if ((*this->nodes_.top().first)->subnodes_.begin() != (*this->nodes_.top().first)->subnodes_.end())
            this->nodes_.push(std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator>((*this->nodes_.top().first)->subnodes_.begin(), (*this->nodes_.top().first)->subnodes_.end()));
        else
        {
            do
            {
                ++this->nodes_.top().first;
                if (this->nodes_.top().first == this->nodes_.top().second)
                {
                    this->nodes_.pop();
                    continue;
                }

                break;
            } while (!this->nodes_.empty());
        }

        return *this;
    }

    ClassTreeMaskNode* ClassTreeMaskIterator::operator*() const
    {
        return (*this->nodes_.top().first);
    }

    ClassTreeMaskNode* ClassTreeMaskIterator::operator->() const
    {
        return (*this->nodes_.top().first);
    }

    ClassTreeMaskIterator::operator bool()
    {
        return (!this->nodes_.empty());
    }

    bool ClassTreeMaskIterator::operator==(ClassTreeMaskNode* compare)
    {
        if (!this->nodes_.empty())
            return ((*this->nodes_.top().first) == compare);
        else
            return (compare == 0);
    }

    bool ClassTreeMaskIterator::operator!=(ClassTreeMaskNode* compare)
    {
        if (!this->nodes_.empty())
            return ((*this->nodes_.top().first) != compare);
        else
            return (compare != 0);
    }


    // ###############################
    // ###      ClassTreeMask      ###
    // ###############################
    ClassTreeMask::ClassTreeMask()
    {
        this->root_ = new ClassTreeMaskNode(ClassManager<BaseObject>::getIdentifier("BaseObject"), true);
    }

    ClassTreeMask::ClassTreeMask(const ClassTreeMask& other)
    {
        this->root_ = new ClassTreeMaskNode(ClassManager<BaseObject>::getIdentifier("BaseObject"), true);
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
            this->add(it->getClass(), it->isIncluded());
    }

    ClassTreeMask::~ClassTreeMask()
    {
        delete this->root_;
    }

    void ClassTreeMask::include(const Identifier* subclass)
    {
        this->add(subclass, true);
    }

    void ClassTreeMask::exclude(const Identifier* subclass)
    {
        this->add(subclass, false);
    }

    void ClassTreeMask::add(const Identifier* subclass, bool bInclude)
    {
        this->add(this->root_, subclass, bInclude);
    }

    void ClassTreeMask::add(ClassTreeMaskNode* node, const Identifier* subclass, bool bInclude)
    {
        // Check if the current node contains exactly the subclass we want to add
        if (subclass == node->getClass())
        {
            // We're at the right place, just change the mask and return
            node->setIncluded(bInclude);
            return;
        }
        else
        {
            // Search for an already existing node, containing the subclass we want to add
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); ++it)
            {
                if (subclass->isA((*it)->getClass()))
                {
                    // We've found an existing node -> delegate the work and return
                    this->add(*it, subclass, bInclude);
                    return;
                }
            }

            // There is no existing node satisfying our needs -> we create a new node
            ClassTreeMaskNode* newnode = new ClassTreeMaskNode(subclass, bInclude);

            // Search for nodes that should actually be subnodes of our new node
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); )
            {
                if ((*it)->getClass()->isChildOf(subclass))
                {
                    // We've found a subnode: add it to the new node an erase it from the current node
                    newnode->addSubnode(*it);
                    node->subnodes_.erase(it++);
                }
                else
                {
                    ++it;
                }
            }

            // Finally add the new node as a subnode to the current node
            node->addSubnode(newnode);
        }
    }

    void ClassTreeMask::reset()
    {
        delete this->root_;
        this->root_ = new ClassTreeMaskNode(ClassManager<BaseObject>::getIdentifier("BaseObject"), true);
    }

    bool ClassTreeMask::isIncluded(const Identifier* subclass) const
    {
        return this->isIncluded(this->root_, subclass);
    }

    bool ClassTreeMask::isIncluded(ClassTreeMaskNode* node, const Identifier* subclass) const
    {
//std::cout << "1_1: " << subclass->getName() << " (" << subclass << ") / " << node->getClass()->getName() << " (" << node->getClass() << ")" << std::endl;
        // Check if the searched subclass is of the same type as the class in the current node or a derivative
        if (subclass->isA(node->getClass()))
        {
            // Check for the special case
            if (subclass == node->getClass())
                return node->isIncluded();

            // Go through the list of subnodes and look for a node containing the searched subclass
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); ++it)
                if (subclass->isA((*it)->getClass()))
                    return isIncluded(*it, subclass);

            // There is no subnode containing our class -> the rule of the current node takes in effect
            return node->isIncluded();
        }
        else
        {
            // The class is not included in the mask: return false
            return false;
        }
    }

    bool ClassTreeMask::isExcluded(const Identifier* subclass) const
    {
        return (!this->isIncluded(subclass));
    }

    void ClassTreeMask::clean()
    {
        this->clean(this->root_);
    }

    void ClassTreeMask::clean(ClassTreeMaskNode* node)
    {
//std::cout << "4_1: " << node->getClass()->getName() << ": " << node->isIncluded() << "\n";
        for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); )
        {
//std::cout << "4_2: " << (*it)->getClass()->getName() << ": " << (*it)->isIncluded() << "\n";
            this->clean(*it);
//std::cout << "4_3\n";
            if ((*it)->isIncluded() == node->isIncluded())
            {
//std::cout << "4_4\n";
                node->subnodes_.insert(node->subnodes_.end(), (*it)->subnodes_.begin(), (*it)->subnodes_.end());
                (*it)->subnodes_.clear();
                node->subnodes_.erase(it);
                it = node->subnodes_.begin();
//std::cout << "4_5\n";
            }
            else
            {
//std::cout << "4_6\n";
                ++it;
            }
        }
//std::cout << "4_7\n";
    }

    ClassTreeMask& ClassTreeMask::operator=(const ClassTreeMask& other)
    {
        ClassTreeMask temp(other);

        this->reset();

        for (ClassTreeMaskIterator it = temp.root_; it; ++it)
            this->add(it->getClass(), it->isIncluded());

        return (*this);
    }

    ClassTreeMask& ClassTreeMask::operator+()
    {
        return (*this);
    }

    ClassTreeMask ClassTreeMask::operator-() const
    {
        return (!(*this));
    }

    ClassTreeMask ClassTreeMask::operator+(const ClassTreeMask& other) const
    {
        ClassTreeMask newmask;
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) or other.isIncluded(subclass));
        }
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) or other.isIncluded(subclass));
        }
        newmask.clean();

        return newmask;
    }

    ClassTreeMask ClassTreeMask::operator*(const ClassTreeMask& other) const
    {
        ClassTreeMask newmask;
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) and other.isIncluded(subclass));
        }
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) and other.isIncluded(subclass));
        }
        newmask.clean();

        return newmask;
    }

    ClassTreeMask ClassTreeMask::operator-(const ClassTreeMask& other) const
    {
        return ((*this) * (!other));
    }

    ClassTreeMask ClassTreeMask::operator!() const
    {
        ClassTreeMask newmask;
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, !this->isIncluded(subclass));
        }
        return newmask;
    }

    ClassTreeMask& ClassTreeMask::operator+=(const ClassTreeMask& other)
    {
        (*this) = (*this) + other;
        return (*this);
    }

    ClassTreeMask& ClassTreeMask::operator*=(const ClassTreeMask& other)
    {
        (*this) = (*this) * other;
        return (*this);
    }

    ClassTreeMask& ClassTreeMask::operator-=(const ClassTreeMask& other)
    {
        (*this) = (*this) - other;
        return (*this);
    }

    ClassTreeMask ClassTreeMask::operator&(const ClassTreeMask& other) const
    {
        return ((*this) * other);
    }

    ClassTreeMask ClassTreeMask::operator|(const ClassTreeMask& other) const
    {
        return ((*this) + other);
    }

    ClassTreeMask ClassTreeMask::operator^(const ClassTreeMask& other) const
    {
        ClassTreeMask newmask;
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) xor other.isIncluded(subclass));
        }
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) xor other.isIncluded(subclass));
        }
        newmask.clean();

        return newmask;
    }

    ClassTreeMask ClassTreeMask::operator~() const
    {
        return (!(*this));
    }

    ClassTreeMask& ClassTreeMask::operator&=(const ClassTreeMask& other)
    {
        (*this) = (*this) & other;
        return (*this);
    }

    ClassTreeMask& ClassTreeMask::operator|=(const ClassTreeMask& other)
    {
        (*this) = (*this) | other;
        return (*this);
    }

    ClassTreeMask& ClassTreeMask::operator^=(const ClassTreeMask& other)
    {
        (*this) = (*this) ^ other;
        return (*this);
    }

    std::ostream& operator<<(std::ostream& out, const ClassTreeMask& mask)
    {
        for (ClassTreeMaskIterator it = mask.root_; it; ++it)
        {
            if (it->isIncluded())
                out << "+";
            else
                out << "-";

            out << it->getClass()->getName() << " ";
        }

        return out;
    }
}
