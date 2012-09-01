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
    @brief Declaration of the OrxonoxClass Class.

    All objects and interfaces of the game-logic (not the engine) are derived from OrxonoxClass.
    It stores the Identifier and the MetaObjectList and has all needed functions to create and use the class-hierarchy.
*/

#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include "CorePrereqs.h"

#include <set>
#include <vector>

/**
@def CCOUT
    Acts almost exactly like COUT(x), but prepends "ClassName: "
*/
#define CCOUT(level) \
    COUT(level) << this->getIdentifier()->getName() << ": "

namespace orxonox
{
    //! The class all objects and interfaces of the game-logic (not the engine) are derived from.
    /**
        The BaseObject and Interfaces are derived with 'virtual public OrxonoxClass' from OrxonoxClass.
        OrxonoxClass is needed to create the class-hierarchy at startup and to store the Identifier and the MetaObjectList.
    */
    class _CoreExport OrxonoxClass
    {
        template <class T>
        friend class ClassIdentifier;

        template <class T>
        friend class SmartPtr;

        template <class T>
        friend class WeakPtr;

        public:
            OrxonoxClass();
            virtual ~OrxonoxClass();

            void destroy();
            void unregisterObject();

            /** @brief Function to collect the SetConfigValue-macro calls. */
            void setConfigValues() {};

            /** @brief Returns the Identifier of the object. @return The Identifier */
            inline Identifier* getIdentifier() const { return this->identifier_; }

            bool isA(const Identifier* identifier);
            bool isExactlyA(const Identifier* identifier);
            bool isChildOf(const Identifier* identifier);
            bool isDirectChildOf(const Identifier* identifier);
            bool isParentOf(const Identifier* identifier);
            bool isDirectParentOf(const Identifier* identifier);

            template <class B> inline bool isA(const SubclassIdentifier<B>* identifier)
                { return this->isA(*identifier); }
            template <class B> inline bool isExactlyA(const SubclassIdentifier<B>* identifier)
                { return this->isExactlyA(*identifier); }
            template <class B> inline bool isChildOf(const SubclassIdentifier<B>* identifier)
                { return this->isChildOf(*identifier); }
            template <class B> inline bool isDirectChildOf(const SubclassIdentifier<B>* identifier)
                { return this->isDirectChildOf(*identifier); }
            template <class B> inline bool isParentOf(const SubclassIdentifier<B>* identifier)
                { return this->isParentOf(*identifier); }
            template <class B> inline bool isDirectParentOf(const SubclassIdentifier<B>* identifier)
                { return this->isDirectParentOf(*identifier); }

            bool isA(const OrxonoxClass* object);
            bool isExactlyA(const OrxonoxClass* object);
            bool isChildOf(const OrxonoxClass* object);
            bool isDirectChildOf(const OrxonoxClass* object);
            bool isParentOf(const OrxonoxClass* object);
            bool isDirectParentOf(const OrxonoxClass* object);

            inline unsigned int getReferenceCount() const
                { return this->referenceCount_; }

            /**
            @brief
                Returns a valid pointer of any derived type that is
                registered in the class hierarchy.
            @return
                Returns NULL if the no pointer was found.
            */
            FORCEINLINE void* getDerivedPointer(unsigned int classID)
            {
                for (int i = this->objectPointers_.size() - 1; i >= 0; --i)
                {
                    if (this->objectPointers_[i].first == classID)
                        return this->objectPointers_[i].second;
                }
                return NULL;
            }

            //! Version of getDerivedPointer with template
            template <class T> FORCEINLINE T* getDerivedPointer(unsigned int classID)
            {   return static_cast<T*>(this->getDerivedPointer(classID));   }
            //! Const version of getDerivedPointer with template
            template <class T> FORCEINLINE const T* getDerivedPointer(unsigned int classID) const
            {   return const_cast<OrxonoxClass*>(this)->getDerivedPointer<T>(classID);   }

        protected:
            virtual void preDestroy() {}

        private:
            /** @brief Increments the reference counter (for smart pointers). */
            inline void incrementReferenceCount()
                { ++this->referenceCount_; }
            /** @brief Decrements the reference counter (for smart pointers). */
            inline void decrementReferenceCount()
            {
                --this->referenceCount_;
                if (this->referenceCount_ == 0 && this->requestedDestruction_)
                    this->destroy();
            }

            /** @brief Register a weak pointer which points to this object. */
            template <class T>
            inline void registerWeakPtr(WeakPtr<T>* pointer)
                { this->weakPointers_.insert(reinterpret_cast<WeakPtr<OrxonoxClass>*>(pointer)); }
            /** @brief Unegister a weak pointer which pointed to this object before. */
            template <class T>
            inline void unregisterWeakPtr(WeakPtr<T>* pointer)
                { this->weakPointers_.erase(reinterpret_cast<WeakPtr<OrxonoxClass>*>(pointer)); }

            Identifier* identifier_;                   //!< The Identifier of the object
            std::set<const Identifier*>* parents_;     //!< List of all parents of the object
            MetaObjectList* metaList_;                 //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in
            int referenceCount_;                       //!< Counts the references from smart pointers to this object
            bool requestedDestruction_;                //!< Becomes true after someone called delete on this object
            std::set<WeakPtr<OrxonoxClass>*> weakPointers_; //!< All weak pointers which point to this object (and like to get notified if it dies)

            //! 'Fast map' that holds this-pointers of all derived types
            std::vector<std::pair<unsigned int, void*> > objectPointers_;
    };
}

#endif /* _OrxonoxClass_H__ */
