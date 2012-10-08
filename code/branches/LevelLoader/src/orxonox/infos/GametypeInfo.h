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
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file GametypeInfo.h
    @brief Definition of the GametypeInfo class
*/

#ifndef _GametypeInfo_H__
#define _GametypeInfo_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include "Info.h"

namespace orxonox
{

    /**
    @brief
        The GametypeInfo class keeps track of the state of the game and provides facilities to inform the player about it.

    @author
        Fabian 'x3n' Landau
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport GametypeInfo : public Info
    {
        friend class Gametype;

        public:
            GametypeInfo(BaseObject* creator);
            virtual ~GametypeInfo();

            /**
            @brief Get whether the game has started yet.
            @return Returns true if the game has started, false if not.
            */
            inline bool hasStarted() const
                { return this->bStarted_; }
            void changedStarted(void); // Is called when the game has changed to started.

            /**
            @brief Get whether the game has ended yet.
            @return Returns true if the game has ended, false if not.
            */
            inline bool hasEnded() const
                { return this->bEnded_; }
            void changedEnded(void); // Is called when the game has changed to ended.

            /**
            @brief Get whether the start countdown is currently running.
            @return Returns true if the countdown is running, false if not.
            */
            inline bool isStartCountdownRunning() const
                { return this->bStartCountdownRunning_; }
            void changedStartCountdownRunning(void); // Is called when the start countdown has been either started or stopped.

            /**
            @brief Get the current value of the start countdown.
            @return Returns the current value of the start countdown.
            */
            inline float getStartCountdown() const
                { return this->startCountdown_; }

            /**
            @brief Get the current start countdown counter.
                   The start countdown counter only has integer values that correspond to the actually displayed countdown.
            @return Returns the current integer countdown counter.
            */
            inline unsigned int getStartCountdownCounter() const
                { return this->counter_; }
            void changedCountdownCounter(void); // Is called when the start countdown counter has changed.

            /**
            @brief Get whether the local player is ready to spawn.
            @return Returns true if the player is ready to spawn, false if not.
            */
            inline bool isReadyToSpawn() const
                { return this->readyToSpawn_; }
            void changedReadyToSpawn(bool ready); // Inform the GametypeInfo that the local player has changed its spawned status.

            /**
            @brief Get whether the local player is spawned.
            @return Returns true if the local player is currently spawned, false if not.
            */
            inline bool isSpawned() const
                { return this->spawned_; }
            void changedSpawned(bool spawned); // Inform the GametypeInfo that the local player has changed its spawned status.

            inline const std::string& getHUDTemplate() const
                { return this->hudtemplate_; }

            void sendAnnounceMessage(const std::string& message) const;
            void sendAnnounceMessage(const std::string& message, unsigned int clientID) const;
            void sendKillMessage(const std::string& message, unsigned int clientID) const;
            void sendDeathMessage(const std::string& message, unsigned int clientID) const;
            void sendStaticMessage(const std::string& message, unsigned int clientID, const ColourValue& colour) const;
            void sendFadingMessage(const std::string& message, unsigned int clientID) const;

            void dispatchAnnounceMessage(const std::string& message) const;
            void dispatchKillMessage(const std::string& message) const;
            void dispatchDeathMessage(const std::string& message) const;
            void dispatchStaticMessage(const std::string& message,const ColourValue& colour) const;
            void dispatchFadingMessage(const std::string& message) const;

        protected:
            void start(void); // Inform the GametypeInfo that the game has started.
            void end(void); // Inform the GametypeInfo that the game has ended.
            void setStartCountdown(float countdown); // Set the start countdown to the input value.
            void countdownStartCountdown(float countDown); // Count down the start countdown by the specified value.
            void countDown(); // Count down the start countdown counter.
            void startStartCountdown(void); // Inform the GametypeInfo about the start of the start countdown.
            void stopStartCountdown(void); // Inform the GametypeInfo about the stop of the start countdown.
            void playerReadyToSpawn(PlayerInfo* player); // Inform the GametypeInfo about a player that is ready to spawn.
            void pawnKilled(PlayerInfo* player); // Inform the GametypeInfo about a player whose Pawn has been killed.
            void playerSpawned(PlayerInfo* player); // Inform the GametypeInfo about a player that has spawned.
            void playerEntered(PlayerInfo* player); // Inform the GametypeInfo about a player that has entered,

            inline void setHUDTemplate(const std::string& templateName)
                { this->hudtemplate_ = templateName; };

        private:
            void registerVariables();
            void setSpawnedHelper(PlayerInfo* player, bool spawned); // Helper method. Sends changedReadyToSpawn notifiers over the network.
            void setReadyToSpawnHelper(PlayerInfo* player, bool ready); // Helper method. Sends changedSpawned notifiers over the network.

            static const std::string NOTIFICATION_SENDER; //!< The name of the sender for the sending of notifications.

            bool bStarted_; //!< Whether the game has started,
            bool bEnded_; //!< Whether the game has ended.
            bool bStartCountdownRunning_; //!< Whether the start countdown is running.
            float startCountdown_; //!< The current value of the start countdown.
            unsigned int counter_; //!< The current integer value of the start countdown, the start countdown counter.
            std::string hudtemplate_;

            std::set<PlayerInfo*> spawnedPlayers_; //!< A set of players that are currently spawned.
            bool spawned_; //!< Whether the local Player is currently spawned.
            bool readyToSpawn_; //!< Whether the local Player is ready to spawn.
    };
}

#endif /* _GametypeInfo_H__ */
