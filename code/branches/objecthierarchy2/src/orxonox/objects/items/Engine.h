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

#ifndef _Engine_H__
#define _Engine_H__

#include "OrxonoxPrereqs.h"

#include "Item.h"
#include "objects/Tickable.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport Engine : public Item, public Tickable
    {
        public:
            Engine(BaseObject* creator);
            virtual ~Engine();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void tick(float dt);

            virtual void addToSpaceShip(SpaceShip* ship);
            inline SpaceShip* getShip() const
                { return this->ship_; }

            inline void setBoostFactor(float factor)
                { this->boostFactor_ = factor; }
            inline float getBoostFactor() const
                { return this->boostFactor_; }

            inline void setSpeedFactor(float factor)
                { this->speedFactor_ = factor; }
            inline float getSpeedFactor() const
                { return this->speedFactor_; }

            inline void setMaxSpeedFront(float speed)
                { this->maxSpeedFront_ = speed; }
            inline void setMaxSpeedBack(float speed)
                { this->maxSpeedBack_ = speed; }
            inline void setMaxSpeedLeftRight(float speed)
                { this->maxSpeedLeftRight_ = speed; }
            inline void setMaxSpeedUpDown(float speed)
                { this->maxSpeedUpDown_ = speed; }

            inline void setAccelerationFront(float acceleration)
                { this->accelerationFront_ = acceleration; }
            inline void setAccelerationBrake(float acceleration)
                { this->accelerationBrake_ = acceleration; }
            inline void setAccelerationBack(float acceleration)
                { this->accelerationBack_ = acceleration; }
            inline void setAccelerationLeftRight(float acceleration)
                { this->accelerationLeftRight_ = acceleration; }
            inline void setAccelerationUpDown(float acceleration)
                { this->accelerationUpDown_ = acceleration; }

            virtual const Vector3& getDirection() const;

        private:
            void networkcallback_shipID();

            SpaceShip* ship_;
            unsigned int shipID_;

            float boostFactor_;
            float speedFactor_;

            float maxSpeedFront_;
            float maxSpeedBack_;
            float maxSpeedLeftRight_;
            float maxSpeedUpDown_;

            float accelerationFront_;
            float accelerationBrake_;
            float accelerationBack_;
            float accelerationLeftRight_;
            float accelerationUpDown_;
    };
}

#endif /* _Engine_H__ */
