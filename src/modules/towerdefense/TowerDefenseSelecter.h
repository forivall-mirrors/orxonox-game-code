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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

#ifndef _TowerDefenseSelecter_H__
#define _TowerDefenseSelecter_H__

#include "towerdefense/TowerDefensePrereqs.h"
#include "worldentities/ControllableEntity.h"
#include "TDCoordinate.h"

namespace orxonox
{
class _TowerDefenseExport TowerDefenseSelecter : public ControllableEntity
    {
        public:
            TowerDefenseSelecter(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~TowerDefenseSelecter();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);
            void fire(unsigned int firemode);
            virtual void fired(unsigned int firemode);
            TDCoordinate* selectedPos_;



        private:

            bool moveUpPressed_;
            bool moveDownPressed_;
            bool moveLeftPressed_;
            bool moveRightPressed_;
    };
}

#endif /* _TowerDefenseSelecter_H__ */
