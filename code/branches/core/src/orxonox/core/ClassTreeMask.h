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

/**
    @file ClassTreeMask.h
    @brief Definition of the ClassTreeMask, ClassTreeMaskNode and ClassTreeMaskIterator classes.

    ClassTreeMask is a class to define a mask of the class-tree beginning with BaseObject.
    You can include or exclude classes by calling the corresponding functions with the
    Identifier of the class.

    You can work with a ClassTreeMask in the sense of the set-theory, meaning that you can create
    unions, intersections, complements and differences by using overloaded operators.



    The ClassTreeMask is internally represented by a tree. The nodes in the tree are
    ClassTreeMaskNodes, containing the rule (included or excluded) for this class and all
    subclasses and a list of all subnodes. To minimize the size, the tree contains only
    nodes changing the mask. By adding new rules, the tree gets reordered dynamically.

    Adding a new rule overwrites all rules assigned to inherited classes. Use overwrite = false
    if you don't like this feature. Useless rules that don't change the information of the mask
    aren't saved in the internal tree. Use clean = false if you wan't to save them.

    With overwrite = false and clean = false it doesn't matter in which way you create the mask.
    You can manually drop useless rules from the tree by calling clean().



    Because of the complicated shape of the internal tree, there is an iterator to iterate
    through all ClassTreeMaskNodes of a mask. It starts with the BaseObject and moves on to
    the first subclass until it reaches a leaf of the tree. Then the iterator moves one step
    back and iterates to the second subclass. If there are no more subclasses, it steps another
    step back, and so on.

    Example: A and B are childs of BaseObject, A1 and A2 are childs of A, B1 and B2 are childs of B.
    The ClassTreeMaskIterator would move trough the tree in the following order:
    BaseObject, A, A1, A2, B, B1, B2.

    Note that the iterator doesn't move trough the whole class-tree, but only through the
    internal tree of the mask, containing the minimal needed set of nodes to describe the mask.
*/

#ifndef _ClassTreeMask_H__
#define _ClassTreeMask_H__

#include <list>
#include <stack>

#include "CorePrereqs.h"

namespace orxonox
{
    // ###############################
    // ###    ClassTreeMaskNode    ###
    // ###############################
    //! The ClassTreeMaskNode is a node in the internal tree of the ClassTreeMask, containing the rules of the mask.
    /**
        The ClassTreeMaskNode is used to store the rule (included or excluded) for a given
        class (described by the corresponding Identifier). The nodes are used in the internal
        tree of ClassTreeMask. To build a tree, they store a list of all subnodes.
    */
    class _CoreExport ClassTreeMaskNode
    {
        friend class ClassTreeMask;
        friend class ClassTreeMaskIterator;

        public:
            ClassTreeMaskNode(const Identifier* subclass, bool bIncluded = true);
            ~ClassTreeMaskNode();

            void include(bool overwrite = true);
            void exclude(bool overwrite = true);
            void setIncluded(bool bIncluded, bool overwrite = true);

            void addSubnode(ClassTreeMaskNode* subnode);

            bool isIncluded() const;
            bool isExcluded() const;

            const Identifier* getClass() const;

        private:
            void deleteAllSubnodes();

            const Identifier* subclass_;                //!< The Identifier of the subclass the rule refers to
            bool bIncluded_;                            //!< The rule: included or excluded
            std::list<ClassTreeMaskNode*> subnodes_;    //!< A list containing all subnodes in the tree
    };


    // ###############################
    // ###  ClassTreeMaskIterator  ###
    // ###############################
    //! The ClassTreeMaskIterator moves through all ClassTreeMaskNodes of the internal tree of a ClassTreeMask, containing the rules.
    /**
        Because of the complicated shape of the internal rule-tree of ClassTreeMask, an
        iterator is used to move through all nodes of the tree. It starts with the BaseObject
        and moves on to the first subclass until it reaches a leaf of the tree. Then the
        iterator moves one step back and iterates to the second subclass. If there are no more
        subclasses, it steps another step back, and so on.
    */
    class _CoreExport ClassTreeMaskIterator
    {
        public:
            ClassTreeMaskIterator(ClassTreeMaskNode* node);
            ~ClassTreeMaskIterator();

            ClassTreeMaskIterator& operator++();
            ClassTreeMaskNode* operator*() const;
            ClassTreeMaskNode* operator->() const;
            operator bool();
            bool operator==(ClassTreeMaskNode* compare);
            bool operator!=(ClassTreeMaskNode* compare);

        private:
            std::stack<std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator> > nodes_;    //!< A stack to store list-iterators
            std::list<ClassTreeMaskNode*> rootlist_;                                                                            //!< A list for internal use (it only stores the root-node)
    };


    // ###############################
    // ###      ClassTreeMask      ###
    // ###############################
    //! The ClassTreeMask is a set of rules, containing the information for each class whether it's included or not.
    /**
        With a ClassTreeMask, you can include or exclude subtrees of the class-tree, starting
        with a given subclass, described by the corresponding Identifier. To minimize the size
        of the mask, the mask saves only relevant rules. But you can manually add rules that
        don't change the information of the mask by using clean = false. If you want to drop
        useless rules, call the clean() function.
    */
    class _CoreExport ClassTreeMask
    {
        public:
            ClassTreeMask();
            ClassTreeMask(const ClassTreeMask& other);
            ~ClassTreeMask();

            void include(const Identifier* subclass, bool overwrite = true, bool clean = true);
            void exclude(const Identifier* subclass, bool overwrite = true, bool clean = true);
            void add(const Identifier* subclass, bool bInclude, bool overwrite = true, bool clean = true);
            void reset();
            void clean();

            bool isIncluded(const Identifier* subclass) const;
            bool isExcluded(const Identifier* subclass) const;

            ClassTreeMask& operator=(const ClassTreeMask& other);

            ClassTreeMask& operator+();
            ClassTreeMask operator-() const;

            ClassTreeMask operator+(const ClassTreeMask& other) const;
            ClassTreeMask operator*(const ClassTreeMask& other) const;
            ClassTreeMask operator-(const ClassTreeMask& other) const;
            ClassTreeMask operator!() const;

            ClassTreeMask& operator+=(const ClassTreeMask& other);
            ClassTreeMask& operator*=(const ClassTreeMask& other);
            ClassTreeMask& operator-=(const ClassTreeMask& other);

            ClassTreeMask operator&(const ClassTreeMask& other) const;
            ClassTreeMask operator|(const ClassTreeMask& other) const;
            ClassTreeMask operator^(const ClassTreeMask& other) const;
            ClassTreeMask operator~() const;

            ClassTreeMask& operator&=(const ClassTreeMask& other);
            ClassTreeMask& operator|=(const ClassTreeMask& other);
            ClassTreeMask& operator^=(const ClassTreeMask& other);

            friend std::ostream& operator<<(std::ostream& out, const ClassTreeMask& mask);

        private:
            void add(ClassTreeMaskNode* node, const Identifier* subclass, bool bInclude, bool overwrite = true);
            bool isIncluded(ClassTreeMaskNode* node, const Identifier* subclass) const;
            void clean(ClassTreeMaskNode* node);
            bool nodeExists(const Identifier* subclass);

            ClassTreeMaskNode* root_;   //!< The root-node of the internal rule-tree, usually BaseObject
    };
}

#endif /* _ClassTreeMask_H__ */
