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
    @ingroup Class
    @brief Declaration of the ClassTreeMask, ClassTreeMaskNode, and ClassTreeMaskIterator classes.

    ClassTreeMask is a class to define a mask of the class-tree beginning with orxonox::BaseObject.
    You can include or exclude classes by calling the corresponding functions with the
    orxonox::Identifier of the class. This mask can then be used to filter out objects that
    are instances of classes which aren't included in the tree, for example when Loading a
    level file or if a Trigger should be triggered by only a few classes.

    See the description of orxonox::ClassTreeMask for a short example.

    You can work with a ClassTreeMask in the sense of the set-theory, meaning that you can create
    unions, intersections, complements and differences by using overloaded operators.

    @par Tree structure

    The ClassTreeMask is internally represented by a tree. The nodes in the tree are
    ClassTreeMaskNodes, containing the rule (included or excluded) for this class and all
    subclasses and a list of all subnodes. To minimize the size, the tree contains only
    nodes changing the mask. By adding new rules, the tree gets reordered dynamically.

    Adding a new rule overwrites all rules assigned to inherited classes. Use <tt>overwrite = false</tt>
    if you don't like this feature. Useless rules that don't change the information of the mask
    aren't saved in the internal tree. Use <tt>clean = false</tt> if you still want to save them.

    With <tt>overwrite = false</tt> and <tt>clean = false</tt> it doesn't matter in which order
    you create the mask. You can manually drop useless rules from the tree by calling
    @ref orxonox::ClassTreeMask::clean() "clean()".

    @par Objects

    To iterate through all objects of the classes that were included by a ClassTreeMask,
    use orxonox::ClassTreeMaskObjectIterator. The description of this class also contains
    a short example of how to use it.
*/

#ifndef _ClassTreeMask_H__
#define _ClassTreeMask_H__

#include "CorePrereqs.h"

#include <list>
#include <stack>
#include "BaseObject.h"
#include "object/Iterator.h"

namespace orxonox
{
    // ###################################
    // ###      ClassTreeMaskNode      ###
    // ###################################
    /**
        @brief The ClassTreeMaskNode is a node in the internal tree of the ClassTreeMask, containing the rules of the mask.

        The ClassTreeMaskNode is used to store the rule (included or excluded) for a given
        class (described by the corresponding Identifier). The nodes are used in the internal
        tree of ClassTreeMask. To build a tree, they store a list of all subnodes.
    */
    class _CoreExport ClassTreeMaskNode
    {
        friend class ClassTreeMask;
        friend class ClassTreeMaskIterator;
        friend class ClassTreeMaskObjectIterator;

        public:
            ClassTreeMaskNode(const Identifier* subclass, bool bIncluded = true);
            ~ClassTreeMaskNode();

            void include(bool overwrite = true);
            void exclude(bool overwrite = true);
            void setIncluded(bool bIncluded, bool overwrite = true);

            void addSubnode(ClassTreeMaskNode* subnode);

            /// Tells if the rule is "included" or not.
            inline bool isIncluded() const { return this->bIncluded_; }
            /// Tells if the rule is "excluded" or not.
            inline bool isExcluded() const { return (!this->bIncluded_); }

            /// Returns the Identifier of the class the rule refers to.
            inline const Identifier* getClass() const { return this->subclass_; }

            /// Returns true if the node has some subnodes.
            inline bool hasSubnodes() const { return !this->subnodes_.empty(); }

        private:
            void deleteAllSubnodes();

            const Identifier* subclass_;                ///< The Identifier of the subclass the rule refers to
            bool bIncluded_;                            ///< The rule: included or excluded
            std::list<ClassTreeMaskNode*> subnodes_;    ///< A list containing all subnodes of this node
    };


    // ###################################
    // ###    ClassTreeMaskIterator    ###
    // ###################################
    /**
        @brief The ClassTreeMaskIterator moves through all ClassTreeMaskNodes of the internal tree of a ClassTreeMask which contains the rules.

        Because of the complicated shape of the internal rule-tree of ClassTreeMask, an
        iterator is used to move through all nodes of the tree. It starts with the BaseObject
        and moves on to the first subclass until it reaches a leaf of the tree. Then the
        iterator moves one step back and iterates to the second subclass. If there are no more
        subclasses, it steps another step back, and so on.

        Example: A and B are children of BaseObject, A1 and A2 are children of A, B1 and B2 are children of B.
        The ClassTreeMaskIterator would move trough the tree in the following order:
        BaseObject, A, A1, A2, B, B1, B2.

        Note that the iterator doesn't move trough the whole class-tree, but only through the
        internal tree of the mask, containing the minimal needed set of nodes to describe the mask.
    */
    class _CoreExport ClassTreeMaskIterator
    {
        public:
            ClassTreeMaskIterator(ClassTreeMaskNode* node);
            ~ClassTreeMaskIterator();

            const ClassTreeMaskIterator& operator++();
            ClassTreeMaskNode* operator*() const;
            ClassTreeMaskNode* operator->() const;
            operator bool() const;
            bool operator==(ClassTreeMaskNode* compare) const;
            bool operator!=(ClassTreeMaskNode* compare) const;

        private:
            std::stack<std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator> > nodes_;    ///< A stack to store list-iterators
            std::list<ClassTreeMaskNode*> rootlist_;                                                                            ///< A list for internal use (it only stores the root-node)
    };


    // ###################################
    // ###        ClassTreeMask        ###
    // ###################################
    /**
        @brief The ClassTreeMask is a set of rules, containing the information for each class whether it's included or not.

        With a ClassTreeMask, you can include or exclude subtrees of the class-tree, starting
        with a given subclass, described by the corresponding Identifier. To minimize the size
        of the mask, the mask saves only relevant rules. But you can manually add rules that
        don't change the information of the mask by using <tt>clean = false</tt>. If you want to drop
        useless rules, call the clean() function.

        Example:
        @code
        ClassTreeMask mymask;
        mymask.exclude(Class(A));
        mymask.exclude(Class(B));
        mymask.include(Class(ChildOfA));
        @endcode

        In this example, the classes A and B are excluded from the mask, but one of the child
        classes of A is included again.
    */
    class _CoreExport ClassTreeMask
    {
        friend class ClassTreeMaskObjectIterator;

        public:
            ClassTreeMask();
            ClassTreeMask(const ClassTreeMask& other);
            ~ClassTreeMask();

            void include(const Identifier* subclass, bool overwrite = true, bool clean = true);
            void exclude(const Identifier* subclass, bool overwrite = true, bool clean = true);
            void add(const Identifier* subclass, bool bInclude, bool overwrite = true, bool clean = true);

            void includeSingle(const Identifier* subclass, bool clean = true);
            void excludeSingle(const Identifier* subclass, bool clean = true);
            void addSingle(const Identifier* subclass, bool bInclude, bool clean = true);

            void reset();
            void clean();

            bool isIncluded(const Identifier* subclass) const;
            bool isExcluded(const Identifier* subclass) const;

            /// Begin of the ClassTreeMaskObjectIterator.
            inline const ClassTreeMask& begin() const { return (*this); }
            /// End of the ClassTreeMaskObjectIterator.
            inline BaseObject*          end()   const { return 0; }

            ClassTreeMask& operator=(const ClassTreeMask& other);

            bool operator==(const ClassTreeMask& other) const;
            bool operator!=(const ClassTreeMask& other) const;

            const ClassTreeMask& operator+() const;
            ClassTreeMask operator-() const;

            ClassTreeMask operator+(const ClassTreeMask& other) const;
            ClassTreeMask operator*(const ClassTreeMask& other) const;
            ClassTreeMask operator-(const ClassTreeMask& other) const;
            ClassTreeMask operator!() const;

            const ClassTreeMask& operator+=(const ClassTreeMask& other);
            const ClassTreeMask& operator*=(const ClassTreeMask& other);
            const ClassTreeMask& operator-=(const ClassTreeMask& other);

            ClassTreeMask operator&(const ClassTreeMask& other) const;
            ClassTreeMask operator|(const ClassTreeMask& other) const;
            ClassTreeMask operator^(const ClassTreeMask& other) const;
            ClassTreeMask operator~() const;

            const ClassTreeMask& operator&=(const ClassTreeMask& other);
            const ClassTreeMask& operator|=(const ClassTreeMask& other);
            const ClassTreeMask& operator^=(const ClassTreeMask& other);

            friend std::ostream& operator<<(std::ostream& out, const ClassTreeMask& mask);

        private:
            void add(ClassTreeMaskNode* node, const Identifier* subclass, bool bInclude, bool overwrite = true);
            bool isIncluded(ClassTreeMaskNode* node, const Identifier* subclass) const;
            void clean(ClassTreeMaskNode* node);
            bool nodeExists(const Identifier* subclass);

            ClassTreeMaskNode* root_;   ///< The root-node of the internal rule-tree, usually BaseObject
    };


    // ###################################
    // ### ClassTreeMaskObjectIterator ###
    // ###################################
    /**
        @brief The ClassTreeMaskObjectIterator iterates through all objects of the classes that were included by a ClassTreeMask.

        This is done the following way:
        @code
        ClassTreeMask mask;
        for (ClassTreeMaskObjectIterator it = mask.begin(); it != mask.end(); ++it)
            it->doSomething();
        @endcode

        @note The ClassTreeMaskObjectIterator handles all objects as BaseObjects. If
              you want to use another class, you should use a dynamic_cast.

        The performance of ClassTreeMaskObjectIterator is good as long as you don't exclude
        subclasses of included classes. Of course you can still exlucde subclasses, but
        if this is done more often, we need a new implementation using a second ObjectList
        in the Identifier, containing all objects of exactly one class.
    */
    class _CoreExport ClassTreeMaskObjectIterator
    {
        public:
            /// Default-constructor: Does nothing.
            inline ClassTreeMaskObjectIterator() {}
            /// Copy-Constructor: Initializes the iterator from another ClassTreeMask.
            inline ClassTreeMaskObjectIterator(const ClassTreeMask& mask) { (*this) = mask; }

            ClassTreeMaskObjectIterator& operator=(const ClassTreeMask& mask);

            const ClassTreeMaskObjectIterator& operator++();

            /// Returns true if the ClassTreeMaskObjectIterator points at the given object.
            inline bool operator==(BaseObject* pointer) const { return (this->objectIterator_ && (*this->objectIterator_) == pointer) || (!this->objectIterator_ && pointer == 0); }
            /// Returns true if the ClassTreeMaskObjectIterator doesn't point at the given object.
            inline bool operator!=(BaseObject* pointer) const { return (this->objectIterator_ && (*this->objectIterator_) != pointer) || (!this->objectIterator_ && pointer != 0); }
            /// Returns true if the ClassTreeMaskObjectIterator hasn't already reached the end.
            inline operator bool() const { return (this->objectIterator_); }
            /// Returns the object the ClassTreeMaskObjectIterator currently points at.
            inline BaseObject* operator*() const { return (*this->objectIterator_); }
            /// Returns the object the ClassTreeMaskObjectIterator currently points at.
            inline BaseObject* operator->() const { return (*this->objectIterator_); }

        private:
            void create(ClassTreeMaskNode* node);

            std::list<std::pair<const Identifier*, bool> >           subclasses_;       ///< A list of all Identifiers through which objects the iterator should iterate
            std::list<std::pair<const Identifier*, bool> >::iterator subclassIterator_; ///< The current class of the iterator
            Iterator<BaseObject>                                     objectIterator_;   ///< The current object of the iterator
    };
}

#endif /* _ClassTreeMask_H__ */
