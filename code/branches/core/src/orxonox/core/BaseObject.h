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
    @file BaseObject.h
    @brief Definition of the BaseObject class.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "CorePrereqs.h"
#include "util/XMLIncludes.h"

#include "CoreIncludes.h"

namespace orxonox
{
    //! The BaseObject is the parent of all classes representing an instance in the game.
    class _CoreExport BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual Element& XMLPort(Element& xmlelement, bool loading);

            /** @brief Returns a pointer to the level that loaded this object. @return The level */
            inline const Level* getLevel() const { return this->level_; }

        private:
            const Level* level_;                        //!< The level that loaded this object
    };
    ExportClass(BaseObject, Core);
}

#endif /* _BaseObject_H__ */
