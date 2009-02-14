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

#ifndef _HumanPlayer_H__
#define _HumanPlayer_H__

#include "OrxonoxPrereqs.h"

#include "PlayerInfo.h"

namespace orxonox
{
    class _OrxonoxExport HumanPlayer : public PlayerInfo
    {
        public:
            HumanPlayer(BaseObject* creator);
            virtual ~HumanPlayer();

            void registerVariables();
            void setConfigValues();

            bool isInitialized() const;
            float getPing() const;
            float getPacketLossRatio() const;

            void setClientID(unsigned int clientID);

            virtual void changedController();

        protected:
            void configvaluecallback_changednick();
            void configvaluecallback_changedHUDTemplate();
            void networkcallback_changednick();
            void networkcallback_clientIDchanged();
            void networkcallback_server_initialized();
            void networkcallback_client_initialized();

            std::string nick_;
            std::string synchronize_nick_;
            std::string hudtemplate_;
            bool server_initialized_;
            bool client_initialized_;
    };
}

#endif /* _HumanPlayer_H__ */
