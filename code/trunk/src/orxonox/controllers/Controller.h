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

#ifndef _Controller_H__
#define _Controller_H__

#include "OrxonoxPrereqs.h"
#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport Controller : public BaseObject
    {
        // set friend classes to access setControllableEntity
        friend class PlayerInfo;
        friend class ControllableEntity;

        public:
            Controller(Context* context);
            virtual ~Controller();

            inline void setPlayer(PlayerInfo* player)
                { this->player_ = player; }
            inline PlayerInfo* getPlayer() const
                { return this->player_; }

            virtual void hit(Pawn* originator, btManifoldPoint& contactpoint, float damage) {};

/* Override needed for different visual effects (e.g. in "NewHumanController.cc") depending on
   the DIFFERENT AMOUNT OF DAMAGE done to the shield and to the health of "victim" (see Projectile.cc, Pawn.cc)

//            virtual inline void hit(Pawn* originator, btManifoldPoint& contactpoint, float damage, float healthdamage, float shielddamage) {};
*/

            void setGodMode( bool mode ){ this->bGodMode_ = mode; }
            bool getGodMode(){ return this->bGodMode_; }

            inline ControllableEntity* getControllableEntity() const
                { return this->controllableEntity_; }
            virtual void changedControllableEntity() {}

//        protected:
            // don't use this directly, use getPlayer()->startControl(entity) (unless you know exactly what you do)
            inline void setControllableEntity(ControllableEntity* entity)
            {
                if (entity != this->controllableEntity_)
                {
                    this->controllableEntity_ = entity;
                    this->changedControllableEntity();
                }
            }

        protected:
            PlayerInfo* player_;
            ControllableEntity* controllableEntity_;
        private:
            bool bGodMode_;
    };
}

#endif /* _Controller_H__ */
