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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Dynamicmatch_H__
#define _Dynamicmatch_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <vector>

#include "tools/Timer.h"

#include "Gametype.h"

namespace orxonox
{
    class _OrxonoxExport Dynamicmatch : public Gametype
    {
        public:
            Dynamicmatch(BaseObject* creator);
            virtual ~Dynamicmatch() {}

            bool notEnoughPigs;
            bool notEnoughKillers;
            bool notEnoughChasers;

            //three different parties
            int chaser;
            int piggy;
            int killer;

            virtual void evaluatePlayerParties();
            int getParty(PlayerInfo* player);
            void setPlayerColour(PlayerInfo* player);//own function
            void setConfigValues();//done

            bool friendlyfire; //goal: player can switch it on/off
            bool tutorial; //goal: new players receive messages how the new gametype works - later it can be switched off.

            virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0); //ok - score function and management of parties
            virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0); //ok - simple
            virtual void start();
            virtual void end(); //Wie geht das mit der Punkteausgabe aendern? Z.B: Persoenliche Nachricht?
            virtual void playerEntered(PlayerInfo* player);
            virtual void playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn);//is used to initialize the player's party and colour
            virtual bool playerLeft(PlayerInfo* player);
            virtual bool playerChangedName(PlayerInfo* player);//unchanged

            /*virtual void instructions();
            virtual void furtherInstructions();*/
            virtual void rewardPig();
            void grantPigBoost(SpaceShip* spaceship); // Grant the piggy a boost.
            void resetSpeedFactor(WeakPtr<SpaceShip>* ptr);
            void tick (float dt);// used to end the game
            SpawnPoint* getBestSpawnPoint(PlayerInfo* player) const;

        protected:

            std::map< PlayerInfo*, int > playerParty_; //player's parties are recorded here
            std::vector<ColourValue> partyColours_; //aus TeamDeathmatch
            unsigned int numberOf[3]; //array to count number of chasers, pigs, killers
            float pointsPerTime;
            float gameTime_;   // from UnderAttack
            bool gameEnded_; // true if game is over
            int timesequence_; //used for countdown
            //Timer callInstructions_;
    };
}

#endif /* _Dynamicmatch_H__ */
