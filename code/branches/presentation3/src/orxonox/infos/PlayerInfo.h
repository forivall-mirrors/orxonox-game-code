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
#include "core/SubclassIdentifier.h"

namespace orxonox // tolua_export
{ // tolua_export
    class _OrxonoxExport PlayerInfo // tolua_export
        : public Info
    { // tolua_export
        public:
            PlayerInfo(BaseObject* creator);
            virtual ~PlayerInfo();

            virtual void changedName();
            virtual void changedGametype();

            virtual void changedController() {}
            virtual void changedControllableEntity() {}

            inline bool isHumanPlayer() const
                { return this->bHumanPlayer_; }
            inline bool isLocalPlayer() const
                { return this->bLocalPlayer_; }
            inline unsigned int getClientID() const
                { return this->clientID_; }

            virtual bool isInitialized() const = 0;
            virtual float getPing() const = 0;
            virtual float getPacketLossRatio() const = 0;

            inline void setReadyToSpawn(bool bReady)
                { this->bReadyToSpawn_ = bReady; }
            inline bool isReadyToSpawn() const
                { return this->bReadyToSpawn_; }

            void startControl(ControllableEntity* entity);
            void stopControl();
            void startTemporaryControl(ControllableEntity* entity);
            void stopTemporaryControl();

            inline ControllableEntity* getControllableEntity() const
                { return this->controllableEntity_; }

            inline Controller* getController() const
                { return this->controller_; }

            inline const GametypeInfo* getGametypeInfo() const
                { return this->gtinfo_; }

        protected:
            void createController();

            bool bHumanPlayer_;
            bool bLocalPlayer_;
            bool bSetUnreadyAfterSpawn_;
            SubclassIdentifier<Controller> defaultController_;
            unsigned int clientID_;

        private:
            void registerVariables();
            void networkcallback_changedcontrollableentityID();
            void networkcallback_changedgtinfoID();
            void updateGametypeInfo();

            bool bReadyToSpawn_;
            Controller* controller_;
            ControllableEntity* controllableEntity_;
            ControllableEntity* oldControllableEntity_;
            unsigned int controllableEntityID_;

            const GametypeInfo* gtinfo_;
            unsigned int gtinfoID_;
    }; // tolua_export
} // tolua_export

#endif /* _PlayerInfo_H__ */
