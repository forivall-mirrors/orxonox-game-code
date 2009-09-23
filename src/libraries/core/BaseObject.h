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
    @brief Definition of the BaseObject class.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "CorePrereqs.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    //! The BaseObject is the parent of all classes representing an instance in the game.
    class _CoreExport BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject(BaseObject* creator);
            virtual ~BaseObject();

            /** @brief Returns if the object was initialized (passed the object registration). @return True was the object is initialized */
            inline bool isInitialized() const { return this->bInitialized_; }

            inline void setCreator(BaseObject* creator) { this->creator_ = creator; }
            inline BaseObject* getCreator() const { return this->creator_; }

        private:
            bool                   bInitialized_;              //!< True if the object was initialized (passed the object registration)
            BaseObject*            creator_;
    };
}

#endif /* _BaseObject_H__ */
