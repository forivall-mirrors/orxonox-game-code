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
 *      Noe Pedrazzini
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ShipPart_H__
#define _ShipPart_H__

#include "OrxonoxPrereqs.h"
#include "Item.h"

#include <string>


namespace orxonox // tolua_export
{ // tolua_export
    class _OrxonoxExport ShipPart // tolua_export
        : public Item
    { // tolua_export

        public:
            ShipPart(Context* context);
            virtual ~ShipPart();

            //virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void handleHit(float damage, float healthdamage, float shielddamage, Pawn* originator);

            //virtual void attachTo(Pawn* newParent);
            //virtual void detach();

            void addEntity(StaticEntity* entity);
            StaticEntity* getEntity(unsigned int index);
            bool hasEntity(StaticEntity* entity) const;

            void printEntities(); // FIXME: (noep) remove debug

            virtual void setDamageAbsorption(float value);
            inline float getDamageAbsorption()
                { return this->damageAbsorption_; }

            virtual void setHealth(float health);
            inline void addHealth(float health)
                { this->setHealth(this->health_ + health); }
            inline void removeHealth(float health)
                { this->setHealth(this->health_ - health); }
            inline float getHealth() const
                { return this->health_; }


            // FIXME: (noep) Why doesn't this work? Works fine in Engine.h
            //void addToSpaceShip(ModularSpaceShip* ship);

        protected:
            Pawn* parent_;
            unsigned int parentID_; // Object ID of the SpaceShip the Part is mounted on.

            float damageAbsorption_;
            float health_;

        private:
            std::vector<StaticEntity*> entityList_; // list of all entities which belong to this part

    }; // tolua_export
} // tolua_export

#endif /* _ShipPart_H__ */
