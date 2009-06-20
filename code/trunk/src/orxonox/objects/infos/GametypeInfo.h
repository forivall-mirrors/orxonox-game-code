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

#ifndef _GametypeInfo_H__
#define _GametypeInfo_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "Info.h"

namespace orxonox
{
    class _OrxonoxExport GametypeInfo : public Info
    {
        friend class Gametype;

        public:
            GametypeInfo(BaseObject* creator);
            virtual ~GametypeInfo();

            void registerVariables();

            inline bool hasStarted() const
                { return this->bStarted_; }
            inline bool hasEnded() const
                { return this->bEnded_; }

            inline bool isStartCountdownRunning() const
                { return this->bStartCountdownRunning_; }
            inline float getStartCountdown() const
                { return this->startCountdown_; }

            inline const std::string& getHUDTemplate() const
                { return this->hudtemplate_; }

            void sendAnnounceMessage(const std::string& message);
            void sendAnnounceMessage(const std::string& message, unsigned int clientID);
            void sendKillMessage(const std::string& message, unsigned int clientID);
            void sendDeathMessage(const std::string& message, unsigned int clientID);

            void dispatchAnnounceMessage(const std::string& message);
            void dispatchKillMessage(const std::string& message);
            void dispatchDeathMessage(const std::string& message);

        private:
            bool bStarted_;
            bool bEnded_;
            bool bStartCountdownRunning_;
            float startCountdown_;
            std::string hudtemplate_;
    };
}

#endif /* _GametypeInfo_H__ */
