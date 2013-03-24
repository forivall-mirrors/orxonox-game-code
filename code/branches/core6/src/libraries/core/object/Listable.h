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
#include "core/class/Identifiable.h"

namespace orxonox
{
    /**
        @brief Listable stores the MetaObjectList which is used when storing instances in object lists.
    */
    class _CoreExport Listable : virtual public Identifiable
    {
        template <class T>
        friend class ClassIdentifier;

        public:
            Listable();
            virtual ~Listable();

            void unregisterObject();

        private:
            MetaObjectList* metaList_; //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in
    };
}

#endif /* _Listable_H__ */
