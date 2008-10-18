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

namespace orxonox
{
    class _OrxonoxExport PlayerInfo : public Info
    {
        public:
            PlayerInfo();
            virtual ~PlayerInfo();

            void setConfigValues();
            void registerVariables();

            inline void setClientID(unsigned int clientID)
                { this->clientID_ = clientID; this->checkClientID(); }
            inline unsigned int getClientID() const
                { return this->clientID_; }

            inline void setHumanPlayer(bool bHumanPlayer)
                { this->bHumanPlayer_ = bHumanPlayer; }
            inline bool isHumanPlayer() const
                { return this->bHumanPlayer_; }

        private:
            void checkClientID();
            void finishedSetup();
            void checkName();

            unsigned int clientID_;
            float ping_;
            bool bLocalPlayer_;
            bool bHumanPlayer_;
            bool bFinishedSetup_;

            std::string playerName_;
    };
}

#endif /* _PlayerInfo_H__ */
