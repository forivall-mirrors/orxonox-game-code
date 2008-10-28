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

#ifndef _PlayerInfo_H__
#define _PlayerInfo_H__

#include "OrxonoxPrereqs.h"

#include "Info.h"
#include "core/Identifier.h"
#include "objects/controllers/Controller.h"

namespace orxonox
{
    class _OrxonoxExport PlayerInfo : public Info
    {
        public:
            PlayerInfo(BaseObject* creator);
            virtual ~PlayerInfo();

            void registerVariables();

            virtual void changedName();
            virtual void changedGametype();

            inline bool isHumanPlayer() const
                { return this->bHumanPlayer_; }
            inline bool isLocalPlayer() const
                { return this->bLocalPlayer_; }
            inline unsigned int getClientID() const
                { return this->clientID_; }
            inline bool isReadyToSpawn() const
                { return this->bReadyToSpawn_; }

            virtual bool isReady() const = 0;
            virtual float getPing() const = 0;
            virtual float getPacketLossRatio() const = 0;

            inline void setReadyToSpawn(bool bReady)
                { this->bReadyToSpawn_ = bReady; }

            void startControl(ControllableEntity* entity);
            void stopControl(ControllableEntity* entity);

            inline ControllableEntity* getControllableEntity() const
                { return this->controllableEntity_; }

        protected:
            void createController();
            void networkcallback_changedcontrollableentityID();

            bool bHumanPlayer_;
            bool bLocalPlayer_;
            bool bReadyToSpawn_;
            SubclassIdentifier<Controller> defaultController_;
            Controller* controller_;
            ControllableEntity* controllableEntity_;
            unsigned int controllableEntityID_;
            unsigned int clientID_;
    };
}

#endif /* _PlayerInfo_H__ */
