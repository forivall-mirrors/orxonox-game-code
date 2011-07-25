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
        public:
            Controller(BaseObject* creator);
            virtual ~Controller();

            inline void setPlayer(PlayerInfo* player)
                { this->player_ = player; }
            inline PlayerInfo* getPlayer() const
                { return this->player_; }

            inline void setControllableEntity(ControllableEntity* entity)
            {
                if (entity != this->controllableEntity_)
                {
                    this->controllableEntity_ = entity;
                    this->changedControllableEntity();
                }
            }
            inline ControllableEntity* getControllableEntity() const
                { return this->controllableEntity_; }
            virtual void changedControllableEntity();

            inline void setHUDTemplate(const std::string& name)
            {
                if (name != this->hudtemplate_)
                {
                    this->hudtemplate_ = name;
                    if (this->controllableEntity_)
                        this->updateHUD();
                    else
                        this->bUpdateHUD_ = true;
                }
            }
            inline const std::string& getHUDTemplate() const
                { return this->hudtemplate_; }

            inline OverlayGroup* getHUD() const
                { return this->hud_; }

        protected:
            void updateHUD();

            PlayerInfo* player_;
            ControllableEntity* controllableEntity_;
            std::string hudtemplate_;
            OverlayGroup* hud_;
            bool bUpdateHUD_;
    };
}

#endif /* _Controller_H__ */
