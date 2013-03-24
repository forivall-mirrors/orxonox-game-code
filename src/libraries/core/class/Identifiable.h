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
    @ingroup Class Identifier
    @brief Declaration of Identifiable, the base of all classes that should own an Identifier.

    It stores the Identifier and the MetaObjectList and has all needed functions to create and use the class-hierarchy.
*/

#ifndef _Identifiable_H__
#define _Identifiable_H__

#include "core/CorePrereqs.h"

#include <set>
#include <vector>

namespace orxonox
{
    /**
        @brief Identifiable is needed to create the class-hierarchy at startup and to store the Identifier and the MetaObjectList.
    */
    class _CoreExport Identifiable
    {
        template <class T>
        friend class ClassIdentifier;

        public:
            Identifiable();
            virtual ~Identifiable();

            void destroy();
            void unregisterObject();

            /// Returns the Identifier of the object.
            inline Identifier* getIdentifier() const { return this->identifier_; }

            bool isA(const Identifier* identifier);
            bool isExactlyA(const Identifier* identifier);
            bool isChildOf(const Identifier* identifier);
            bool isDirectChildOf(const Identifier* identifier);
            bool isParentOf(const Identifier* identifier);
            bool isDirectParentOf(const Identifier* identifier);

            /// Returns true if the object's class is of the given type or a derivative.
            template <class B> inline bool isA(const SubclassIdentifier<B>* identifier)
                { return this->isA(*identifier); }
            /// Returns true if the object's class is exactly of the given type.
            template <class B> inline bool isExactlyA(const SubclassIdentifier<B>* identifier)
                { return this->isExactlyA(*identifier); }
            /// Returns true if the object's class is a child of the given type.
            template <class B> inline bool isChildOf(const SubclassIdentifier<B>* identifier)
                { return this->isChildOf(*identifier); }
            /// Returns true if the object's class is a direct child of the given type.
            template <class B> inline bool isDirectChildOf(const SubclassIdentifier<B>* identifier)
                { return this->isDirectChildOf(*identifier); }
            /// Returns true if the object's class is a parent of the given type.
            template <class B> inline bool isParentOf(const SubclassIdentifier<B>* identifier)
                { return this->isParentOf(*identifier); }
            /// Returns true if the object's class is a direct parent of the given type.
            template <class B> inline bool isDirectParentOf(const SubclassIdentifier<B>* identifier)
                { return this->isDirectParentOf(*identifier); }

            bool isA(const Identifiable* object);
            bool isExactlyA(const Identifiable* object);
            bool isChildOf(const Identifiable* object);
            bool isDirectChildOf(const Identifiable* object);
            bool isParentOf(const Identifiable* object);
            bool isDirectParentOf(const Identifiable* object);

            /**
            @brief
                Returns a valid pointer of any derived type that is
                registered in the class hierarchy.
            @return
                Returns NULL if the no pointer was found.
            */
            ORX_FORCEINLINE void* getDerivedPointer(unsigned int classID)
            {
                for (int i = this->objectPointers_.size() - 1; i >= 0; --i)
                {
                    if (this->objectPointers_[i].first == classID)
                        return this->objectPointers_[i].second;
                }
                return NULL;
            }

            /// Version of getDerivedPointer with template
            template <class T> ORX_FORCEINLINE T* getDerivedPointer(unsigned int classID)
            {   return static_cast<T*>(this->getDerivedPointer(classID));   }
            /// Const version of getDerivedPointer with template
            template <class T> ORX_FORCEINLINE const T* getDerivedPointer(unsigned int classID) const
            {   return const_cast<Identifiable*>(this)->getDerivedPointer<T>(classID);   }

        private:
            Identifier* identifier_;                                //!< The Identifier of the object
            std::set<const Identifier*>* parents_;                  //!< List of all parents of the object
            MetaObjectList* metaList_;                              //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in

            /// 'Fast map' that holds this-pointers of all derived types
            std::vector<std::pair<unsigned int, void*> > objectPointers_;
    };
}

#endif /* _Identifiable_H__ */
