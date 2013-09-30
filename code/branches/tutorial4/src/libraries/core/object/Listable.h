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
    @ingroup Object ObjectList
    @brief Declaration of Listable, the base of all classes whose instances can be stored in object lists.
*/

#ifndef _Listable_H__
#define _Listable_H__

#include "core/CorePrereqs.h"

#include <vector>

#include "core/class/Identifiable.h"

namespace orxonox
{
    /**
        @brief Listable stores the entries of all object lists pointing to this instance.
    */
    class _CoreExport Listable : virtual public Identifiable
    {
        friend class Context;

        public:
            Listable();
            Listable(Context* context);
            virtual ~Listable();

            void unregisterObject();

            void setContext(Context* context);
            inline Context* getContext() const
                { return this->context_; }

        private:
            static SmallObjectAllocator& getObjectListElementAllocator();

            template <class T>
            static ObjectListElement<T>* createObjectListElement(T* object)
            {
                void* chunk = Listable::getObjectListElementAllocator().alloc();
                return new (chunk) ObjectListElement<T>(object);
            }

            static void deleteObjectListElement(ObjectListBaseElement* element);

            Context* context_;                             //!< The object will register itself in the object lists of this context
            std::vector<ObjectListBaseElement*> elements_; //!< The corresponding ObjectListElements in all object lists the object is registered in
    };
}

#endif /* _Listable_H__ */
