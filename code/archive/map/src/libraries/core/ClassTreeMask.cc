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
    @brief Implementation of the ClassTreeMask, ClassTreeMaskNode and ClassTreeMaskIterator classes.
*/

#include "ClassTreeMask.h"
#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###    ClassTreeMaskNode    ###
    // ###############################
    /**
        @brief Constructor: Creates the node, sets the subclass and the rule.
        @param subclass The subclass the rule refers to
        @param bIncluded The rule: included (true) or excluded (false)
    */
    ClassTreeMaskNode::ClassTreeMaskNode(const Identifier* subclass, bool bIncluded)
    {
        this->subclass_ = subclass;
        this->bIncluded_ = bIncluded;
    }

    /**
        @brief Destructor: Deletes all subnodes.
    */
    ClassTreeMaskNode::~ClassTreeMaskNode()
    {
        // Go through the list of all subnodes and delete them
        this->deleteAllSubnodes();
    }

    /**
        @brief Sets the rule for the node to "included".
        @param overwrite True = overwrite previously added rules for inheriting classes
    */
    void ClassTreeMaskNode::include(bool overwrite)
    {
        this->setIncluded(true, overwrite);
    }

    /**
        @brief Sets the rule for the node to "excluded".
        @param overwrite True = overwrite previously added rules for inheriting classes
    */
    void ClassTreeMaskNode::exclude(bool overwrite)
    {
        this->setIncluded(false, overwrite);
    }

    /**
        @brief Sets the rule for the node to a given value and erases all following rules.
        @param bIncluded The rule: included (true) or excluded (false)
        @param overwrite True = overwrite previously added rules for inheriting classes
    */
    void ClassTreeMaskNode::setIncluded(bool bIncluded, bool overwrite)
    {
        if (overwrite)
            this->deleteAllSubnodes();

        this->bIncluded_ = bIncluded;
    }

    /**
        @brief Adds a new subnode to the list of subnodes.
        @param subnode The new subnode
    */
    void ClassTreeMaskNode::addSubnode(ClassTreeMaskNode* subnode)
    {
        this->subnodes_.insert(this->subnodes_.end(), subnode);
    }

    /**
        @brief Deletes all subnodes of this node.
    */
    void ClassTreeMaskNode::deleteAllSubnodes()
    {
        // Go through the list of all subnodes and delete them
        for (std::list<ClassTreeMaskNode*>::iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); )
            delete (*(it++));

        // Clear the list
        this->subnodes_.clear();
    }


    // ###############################
    // ###  ClassTreeMaskIterator  ###
    // ###############################
    /**
        @brief Constructor: Initializes the iterator by creating a helper-list with the root-node and putting it to the stack.
        @param node The root-node
    */
    ClassTreeMaskIterator::ClassTreeMaskIterator(ClassTreeMaskNode* node)
    {
        // Create a list and put the root-note into it
        std::list<ClassTreeMaskNode*>::iterator it = this->rootlist_.insert(this->rootlist_.end(), node);

        // Put the iterator to the only element of the list and the corresponding end()-iterator on the stack
        this->nodes_.push(std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator>(it, this->rootlist_.end()));
    }

    /**
        @brief Destructor: Does nothing.
    */
    ClassTreeMaskIterator::~ClassTreeMaskIterator()
    {
    }

    /**
        @brief Iterates through the rule-tree.
        @return A reference to the iterator itself
    */
    const ClassTreeMaskIterator& ClassTreeMaskIterator::operator++()
    {
        // Check if the actual node has subnodes
        if ((*this->nodes_.top().first)->subnodes_.begin() != (*this->nodes_.top().first)->subnodes_.end())
        {
            // Yes it has: Push an iterator, pointing at the first subnode, on the stack
            this->nodes_.push(std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator>((*this->nodes_.top().first)->subnodes_.begin(), (*this->nodes_.top().first)->subnodes_.end()));
        }
        else
        {
            // No subnodes, meaning we reached a leaf: Go to the next node
            do
            {
                // Iterate one step in the current list
                ++this->nodes_.top().first;

                // Check if we reached the end of the list (the second item in the stored pair always represents the end)
                if (this->nodes_.top().first == this->nodes_.top().second)
                {
                    // Yes we've reached the end: Pop the list-iterator from the stack
                    this->nodes_.pop();

                    // Continue with the loop, meaning: Try to iterate through the previous list
                    continue;
                }

                // If we reached this point, we aren't yet at the end of the list: Leave the loop
                break;
            } while (!this->nodes_.empty()); // Stop looping if the stack is empty, meaning: We've iterated to the end
        }

        // Finally return a reference to the iterator itself
        return *this;
    }

    /**
        @brief Returns a pointer to the ClassTreeMaskNode whereon the iterator points.
        @return The pointer to the node
    */
    ClassTreeMaskNode* ClassTreeMaskIterator::operator*() const
    {
        return (*this->nodes_.top().first);
    }

    /**
        @brief Returns a pointer to the ClassTreeMaskNode whereon the iterator points.
        @return The pointer to the node
    */
    ClassTreeMaskNode* ClassTreeMaskIterator::operator->() const
    {
        return (*this->nodes_.top().first);
    }

    /**
        @brief Returns true if the stack is empty, meaning we've reached the end of the tree.
        @return True if we've reached the end of the tree
    */
    ClassTreeMaskIterator::operator bool() const
    {
        return (!this->nodes_.empty());
    }

    /**
        @brief Compares the current node with the given one and returns true if they match.
        @param compare The node to compare with
        @return The result of the comparison (true if they match)
    */
    bool ClassTreeMaskIterator::operator==(ClassTreeMaskNode* compare) const
    {
        if (!this->nodes_.empty())
            return ((*this->nodes_.top().first) == compare);
        else
            return (compare == 0);
    }

    /**
        @brief Compares the current node with the given one and returns true if they don't match.
        @param compare The node to compare with
        @return The result of the comparison (true if they don't match)
    */
    bool ClassTreeMaskIterator::operator!=(ClassTreeMaskNode* compare) const
    {
        if (!this->nodes_.empty())
            return ((*this->nodes_.top().first) != compare);
        else
            return (compare != 0);
    }


    // ###############################
    // ###      ClassTreeMask      ###
    // ###############################
    /**
        @brief Constructor: Adds the root-node of the tree with the first rule ("include everything").
    */
    ClassTreeMask::ClassTreeMask()
    {
        this->root_ = new ClassTreeMaskNode(ClassIdentifier<BaseObject>::getIdentifier(), true);
    }

    /**
        @brief Copyconstructor: Adds the root-node of the tree with the first rule ("include everything") and adds all rules from the other mask.
        @param other The other mask
    */
    ClassTreeMask::ClassTreeMask(const ClassTreeMask& other)
    {
        this->root_ = new ClassTreeMaskNode(ClassIdentifier<BaseObject>::getIdentifier(), true);
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
            this->add(it->getClass(), it->isIncluded(), false);
    }

    /**
        @brief Destructor: Deletes the root node (which will delete all subnodes too).
    */
    ClassTreeMask::~ClassTreeMask()
    {
        delete this->root_;
    }

    /**
        @brief Adds a new "include" rule for a given subclass to the mask.
        @param subclass The subclass
        @param overwrite True = overwrite previously added rules for inheriting classes
        @param clean True = clean the tree after adding the new rule
    */
    void ClassTreeMask::include(const Identifier* subclass, bool overwrite, bool clean)
    {
        this->add(subclass, true, overwrite, clean);
    }

    /**
        @brief Adds a new "exclude" rule for a given subclass to the mask.
        @param subclass The subclass
        @param overwrite True = overwrite previously added rules for inheriting classes
        @param clean True = clean the tree after adding the new rule
    */
    void ClassTreeMask::exclude(const Identifier* subclass, bool overwrite, bool clean)
    {
        this->add(subclass, false, overwrite, clean);
    }

    /**
        @brief Adds a new rule for a given subclass to the mask.
        @param subclass The subclass
        @param bInclude The rule: include (true) or exclude (false)
        @param overwrite True = overwrite previously added rules for inheriting classes
        @param clean True = clean the tree after adding the new rule
    */
    void ClassTreeMask::add(const Identifier* subclass, bool bInclude, bool overwrite, bool clean)
    {
        if (!subclass)
            return;
        // Check if the given subclass is a child of our root-class
        if (subclass->isA(this->root_->getClass()))
        {
            // Yes it is: Just add the rule to the three
            this->add(this->root_, subclass, bInclude, overwrite);
        }
        else
        {
            // No it's not: Search for classes inheriting from the given class and add the rules for them
            for (std::set<const Identifier*>::const_iterator it = subclass->getDirectChildrenBegin(); it != subclass->getDirectChildrenEnd(); ++it)
                if ((*it)->isA(this->root_->getClass()))
                    if (overwrite || (!this->nodeExists(*it))) // If we don't want to overwrite, only add nodes that don't already exist
                        this->add(this->root_, *it, bInclude, overwrite);
        }

        // Clean the rule-tree
        if (clean)
            this->clean();
    }

    /**
        @brief Adds a new rule for a given subclass to a node of the internal rule-tree (recursive function).
        @param node The node
        @param subclass The subclass
        @param bInclude The rule: include (true) or exclude (false)
        @param overwrite True = overwrite previously added rules for inheriting classes
    */
    void ClassTreeMask::add(ClassTreeMaskNode* node, const Identifier* subclass, bool bInclude, bool overwrite)
    {
        if (!subclass)
            return;
        // Check if the current node contains exactly the subclass we want to add
        if (subclass == node->getClass())
        {
            // We're at the right place, just change the mask and return
            node->setIncluded(bInclude, overwrite);
            return;
        }
        else if (subclass->isA(node->getClass()))
        {
            // Search for an already existing node, containing the subclass we want to add
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); ++it)
            {
                if (subclass->isA((*it)->getClass()))
                {
                    // We've found an existing node -> delegate the work with a recursive function-call and return
                    this->add(*it, subclass, bInclude, overwrite);
                    return;
                }
            }

            // There is no existing node satisfying our needs -> we create a new node
            ClassTreeMaskNode* newnode = new ClassTreeMaskNode(subclass, bInclude);

            // Search for nodes that should actually be subnodes of our new node and erase them
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); )
            {
                if ((*it)->getClass()->isChildOf(subclass))
                {
                    // We've found a subnode: add it to the new node and erase it from the current node
                    if (!overwrite)
                        newnode->addSubnode(*it);
                    else
                        delete (*it);

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

    /**
        @brief Adds a new "include" rule for a single subclass. The new rule doesn't change the mask for inheriting classes.
        @param subclass The subclass
        @param clean True = clean the tree after adding the new rule
    */
    void ClassTreeMask::includeSingle(const Identifier* subclass, bool clean)
    {
        this->addSingle(subclass, true, clean);
    }

    /**
        @brief Adds a new "exclude" rule for a single subclass. The new rule doesn't change the mask for inheriting classes.
        @param subclass The subclass
        @param clean True = clean the tree after adding the new rule
    */
    void ClassTreeMask::excludeSingle(const Identifier* subclass, bool clean)
    {
        this->addSingle(subclass, false, clean);
    }

    /**
        @brief Adds a new rule for a single subclass. The new rule doesn't change the mask for inheriting classes.
        @param bInclude The rule: include (true) or exclude (false)
        @param subclass The subclass
        @param clean True = clean the tree after adding the new rule
    */
    void ClassTreeMask::addSingle(const Identifier* subclass, bool bInclude, bool clean)
    {
        if (!subclass)
            return;
        for (std::set<const Identifier*>::const_iterator it = subclass->getDirectChildrenBegin(); it != subclass->getDirectChildrenEnd(); ++it)
            this->add(*it, this->isIncluded(*it), false, false);

        this->add(subclass, bInclude, false, clean);
    }

    /**
        @brief Resets the mask to "include everything".
    */
    void ClassTreeMask::reset()
    {
        delete this->root_;
        this->root_ = new ClassTreeMaskNode(ClassIdentifier<BaseObject>::getIdentifier(), true);
    }

    /**
        @brief Tells if a given subclass is included or not.
        @param subclass The subclass
        @return Included (true) or excluded (false)
    */
    bool ClassTreeMask::isIncluded(const Identifier* subclass) const
    {
        return this->isIncluded(this->root_, subclass);
    }

    /**
        @brief Tells if a given subclass of a node in the rule-tree is included or not (recursive function).
        @param node The node
        @param subclass The subclass
        @return Included (true) or excluded (false)
    */
    bool ClassTreeMask::isIncluded(ClassTreeMaskNode* node, const Identifier* subclass) const
    {
        if (!subclass)
            return false;
        // Check if the searched subclass is of the same type as the class in the current node or a derivative
        if (subclass->isA(node->getClass()))
        {
            // Check for the special case
            if (subclass == node->getClass())
                return node->isIncluded();

            // Go through the list of subnodes and look for a node containing the searched subclass and delegate the request by a recursive function-call.
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

    /**
        @brief Tells if a given subclass is excluded or not.
        @param subclass The subclass
        @return The inverted rule: Excluded (true) or included (false)
    */
    bool ClassTreeMask::isExcluded(const Identifier* subclass) const
    {
        return (!this->isIncluded(subclass));
    }

    /**
        @brief Removes all unneeded rules that don't change the information of the mask.
    */
    void ClassTreeMask::clean()
    {
        this->clean(this->root_);
    }

    /**
        @brief Removes all unneded rules that don't change the information of a node of a mask (recursive function).
        @param node The node
    */
    void ClassTreeMask::clean(ClassTreeMaskNode* node)
    {
        // Iterate through all subnodes of the given node
        for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); )
        {
            // Recursive function-call: Clean the subnode
            this->clean(*it);

            // Now check if the subnode contains the same rule as the current node
            if ((*it)->isIncluded() == node->isIncluded())
            {
                // It does: Move all subnodes of the redundant subnode to the current node
                node->subnodes_.insert(node->subnodes_.end(), (*it)->subnodes_.begin(), (*it)->subnodes_.end());
                (*it)->subnodes_.clear();

                // Remove the redundant subnode from the current node
                delete (*it);
                node->subnodes_.erase(it++);
            }
            else
            {
                // The subnode is necessary: Move on to the next subnode
                ++it;
            }
        }
    }

    /**
        @brief Checks if a node for the given subclass exists.
        @param subclass The Identifier of the subclass
        @return True = the node exists
    */
    bool ClassTreeMask::nodeExists(const Identifier* subclass)
    {
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
            if ((*it)->getClass() == subclass)
                return true;

        return false;
    }

    /**
        @brief Assignment operator: Adds all rules of the other mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator=(const ClassTreeMask& other)
    {
        // Make a copy to avoid troubles with self-assignments (like A = A).
        ClassTreeMask temp(other);

        // Removes all current rules
        this->reset();

        // Copy all rules from the other mask
        for (ClassTreeMaskIterator it = temp.root_; it; ++it)
            this->add(it->getClass(), it->isIncluded(), false, false);

        // Return a reference to the mask itself
        return (*this);
    }

    /**
        @brief Compares the mask with another mask and returns true if they represent the same logic.
        @param other The other mask
        @return True if both masks represent the same logic
    */
    bool ClassTreeMask::operator==(const ClassTreeMask& other) const
    {
        ClassTreeMask temp1 = other;
        ClassTreeMask temp2 = (*this);

        temp1.clean();
        temp2.clean();

        ClassTreeMaskIterator it1 = temp1.root_;
        ClassTreeMaskIterator it2 = temp2.root_;

        for ( ; it1 && it2; ++it1, ++it2)
            if (it1->getClass() != it2->getClass())
                return false;

        return true;
    }

    /**
        @brief Compares the mask with another mask and returns true if they represent different logics.
        @param other The other mask
        @return True if the masks represent different logics
    */
    bool ClassTreeMask::operator!=(const ClassTreeMask& other) const
    {
        return (!((*this) == other));
    }

    /**
        @brief Prefix operator + does nothing.
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator+() const
    {
        return (*this);
    }

    /**
        @brief Prefix operator - inverts the mask.
        @return The inverted mask
    */
    ClassTreeMask ClassTreeMask::operator-() const
    {
        return (!(*this));
    }

    /**
        @brief Adds two masks in the sense of a union (all classes that are included in at least one of the masks will be included in the resulting mask too).
        @param other The mask to unite with
        @return The union
    */
    ClassTreeMask ClassTreeMask::operator+(const ClassTreeMask& other) const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the first mask, calculate the rule with the or-operation
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) or other.isIncluded(subclass), false, false);
        }

        // Add all nodes from the second mask, calculate the rule with the or-operation
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) or other.isIncluded(subclass), false, false);
        }

        // Drop all redundant rules
        newmask.clean();

        // Return the new mask
        return newmask;
    }

    /**
        @brief Intersects two masks (only classes that are included in both masks will be included in the resulting mask too).
        @param other The mask to intersect with
        @return The intersection
    */
    ClassTreeMask ClassTreeMask::operator*(const ClassTreeMask& other) const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the first mask, calculate the rule with the and-operation
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) and other.isIncluded(subclass), false, false);
        }

        // Add all nodes from the second mask, calculate the rule with the and-operation
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) and other.isIncluded(subclass), false, false);
        }

        // Drop all redundant rules
        newmask.clean();

        // Return the new mask
        return newmask;
    }

    /**
        @brief Removes all elements of the second mask from the first mask (all classes that are included in the first mask stay included, except those that are included in the second mask too).
        @param other The mask to subtract.
        @return The difference
    */
    ClassTreeMask ClassTreeMask::operator-(const ClassTreeMask& other) const
    {
        return ((*this) * (!other));
    }

    /**
        @brief Inverts the mask (all included classes are now excluded and vice versa).
        @return The complement
    */
    ClassTreeMask ClassTreeMask::operator!() const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the other mask, inverting the rules
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, !this->isIncluded(subclass), false, false);
        }

        // Return the new mask
        return newmask;
    }

    /**
        @brief Unites this mask with another mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator+=(const ClassTreeMask& other)
    {
        (*this) = (*this) + other;
        return (*this);
    }

    /**
        @brief Intersects this mask with another mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator*=(const ClassTreeMask& other)
    {
        (*this) = (*this) * other;
        return (*this);
    }

    /**
        @brief Subtracts another mask from this mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator-=(const ClassTreeMask& other)
    {
        (*this) = (*this) - other;
        return (*this);
    }

    /**
        @brief Intersects two masks (only classes that are included in both masks will be included in the resulting mask too).
        @param other The mask to intersect with
        @return The intersection
    */
    ClassTreeMask ClassTreeMask::operator&(const ClassTreeMask& other) const
    {
        return ((*this) * other);
    }

    /**
        @brief Adds two masks in the sense of a union (all classes that are included in at least one of the masks will be included in the resulting mask too).
        @param other The mask to unite with
        @return The union
    */
    ClassTreeMask ClassTreeMask::operator|(const ClassTreeMask& other) const
    {
        return ((*this) + other);
    }

    /**
        @brief Joins two masks in the sense of a xor (exclusivity) operation (all classes that are included in exactly one of the masks, but not in both, will be included in the resulting mask too).
        @param other The mask to join with
        @return The result
    */
    ClassTreeMask ClassTreeMask::operator^(const ClassTreeMask& other) const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the first mask, calculate the rule with the xor-operation
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) xor other.isIncluded(subclass), false, false);
        }

        // Add all nodes from the second mask, calculate the rule with the xor-operation
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) xor other.isIncluded(subclass), false, false);
        }

        // Drop all redundant rules
        newmask.clean();

        // Return the new mask
        return newmask;
    }

    /**
        @brief Inverts the mask (all included classes are now excluded and vice versa).
        @return The complement
    */
    ClassTreeMask ClassTreeMask::operator~() const
    {
        return (!(*this));
    }

    /**
        @brief Intersects this mask with another mask (and-operation)
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator&=(const ClassTreeMask& other)
    {
        (*this) = (*this) & other;
        return (*this);
    }

    /**
        @brief Unites this mask with another mask (or-operation).
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator|=(const ClassTreeMask& other)
    {
        (*this) = (*this) | other;
        return (*this);
    }

    /**
        @brief Joins this mask with another mask with a xor-operation.
        @param other The other mask
        @return A reference to the mask itself
    */
    const ClassTreeMask& ClassTreeMask::operator^=(const ClassTreeMask& other)
    {
        (*this) = (*this) ^ other;
        return (*this);
    }

    /**
        @brief Converts the content of a mask into a human readable string and puts it on the ostream.
        @param out The ostream
        @param mask The mask
        @return A reference to the ostream
    */
    std::ostream& operator<<(std::ostream& out, const ClassTreeMask& mask)
    {
        // Iterate through all rules
        for (ClassTreeMaskIterator it = mask.root_; it; ++it)
        {
            // Calculate the prefix: + means included, - means excluded
            if (it->isIncluded())
                out << '+';
            else
                out << '-';

            // Put the name of the corresponding class on the stream
            out << it->getClass()->getName() << ' ';
        }

        return out;
    }


    // ###################################
    // ### ClassTreeMaskObjectIterator ###
    // ###################################
    /**
        @brief Initializes the iterator from a given ClassTreeMask.
        @param mask The mask
    */
    const ClassTreeMaskObjectIterator& ClassTreeMaskObjectIterator::operator=(const ClassTreeMask& mask)
    {
        // Clear everything, use a cleaned copy of the mask
        this->subclasses_.clear();
        ClassTreeMask temp = mask;
        temp.clean();

        // Create the subclass-list by going through the mask-tree, starting with the root-node
        this->create(temp.root_);

        // Move the subclass-iterator to the beginning of the subclass-list
        this->subclassIterator_ = this->subclasses_.begin();

        // If there is a first subclass, move the object-iterator to the first object of this class. Else go to the end
        if (this->subclassIterator_ != this->subclasses_.end())
            this->objectIterator_ = this->subclassIterator_->first->getObjects()->begin();
        else
            this->objectIterator_ = ObjectList<BaseObject>::end();

        // Check if the iterator points on a valid object. If not, go to the next object by calling ++
        if (!this->objectIterator_ || (this->subclassIterator_->second && !this->objectIterator_->isExactlyA(this->subclassIterator_->first)))
            this->operator++();

        return (*this);
    }

    /**
        @brief Iterate to the next object (if any).
        @return The iterator itself
    */
    const ClassTreeMaskObjectIterator& ClassTreeMaskObjectIterator::operator++()
    {
        if (this->objectIterator_)
        {
            // The iterator points on a valid object, therefore we also have a valid subclass-iterator at the moment
            do
            {
                // Go to the next object
                ++this->objectIterator_;

                while (!this->objectIterator_)
                {
                    // We reached the end of the current objectlist - go to the next class
                    ++this->subclassIterator_;

                    // Check if there really is a next class. If yes, move the object-iterator to the first object
                    if (this->subclassIterator_ != this->subclasses_.end())
                        this->objectIterator_ = this->subclassIterator_->first->getObjects()->begin();
                    else
                        return (*this);
                }

            // Repeat this until we reach a valid object or the end
            } while (this->subclassIterator_->second && !this->objectIterator_->isExactlyA(this->subclassIterator_->first));
        }
        return (*this);
    }

    /**
        @brief Recursive function to create the Iterators subclass-list by going through the node-tree of the mask.
        @param node The current node
    */
    void ClassTreeMaskObjectIterator::create(ClassTreeMaskNode* node)
    {
        // Add the class of this node to the list, if the class is included
        if (node->isIncluded())
        {
            // If there are some subnodes, the bool is "true", meaning we have to check for the exact clss when iterating
            if (node->hasSubnodes())
                this->subclasses_.insert(this->subclasses_.end(), std::pair<const Identifier*, bool>(node->getClass(), true));
            else
                this->subclasses_.insert(this->subclasses_.end(), std::pair<const Identifier*, bool>(node->getClass(), false));
        }

        // Now check if the node has subnodes
        if (node->hasSubnodes())
        {
            // Get all _direct_ children of the nodes class
            std::set<const Identifier*> directChildren = node->getClass()->getDirectChildren();

            // Iterate through all subnodes
            for (std::list<ClassTreeMaskNode*>::iterator it1 = node->subnodes_.begin(); it1 != node->subnodes_.end(); ++it1)
            {
                // Recursive call to this function with the subnode
                this->create(*it1);

                // Only execute the following code if the current node is included, meaning some of the subnodes might be included too
                if (node->isIncluded())
                {
                    scanChildren: // This is a label for goto

                    // Iterate through all direct children
                    for (std::set<const Identifier*>::iterator it2 = directChildren.begin(); it2 != directChildren.end(); ++it2)
                    {
                        // Check if the subnode (it1) is a child of the directChild (it2)
                        if ((*it1)->getClass()->isA(*it2))
                        {
                            // Yes it is - remove the directChild (it2) from the list, because it will already be handled by a recursive call to the create() function
                            directChildren.erase(it2);

                            // Check if the removed directChild was exactly the subnode
                            if (!(*it1)->getClass()->isExactlyA(*it2))
                            {
                                // No, it wasn't exactly the subnode - therefore there are some classes between

                                // Add the previously removed directChild (it2) to the subclass-list
                                this->subclasses_.insert(this->subclasses_.end(), std::pair<const Identifier*, bool>(*it2, true));

                                // Insert all directChildren of the directChild
                                directChildren.insert((*it2)->getDirectChildrenBegin(), (*it2)->getDirectChildrenEnd());

                                // Restart the scan with the expanded set of directChildren
                                goto scanChildren;
                            }
                            break;
                        }
                    }
                }
            }

            // Now add all directChildren which don't have subnodes on their own to the subclass-list
            // The bool is "false", meaning they have no subnodes and therefore need no further checks
            if (node->isIncluded())
                for (std::set<const Identifier*>::iterator it = directChildren.begin(); it != directChildren.end(); ++it)
                    this->subclasses_.insert(this->subclasses_.end(), std::pair<const Identifier*, bool>(*it, false));
        }
    }
}
