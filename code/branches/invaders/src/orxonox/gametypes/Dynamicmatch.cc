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
//TODO:
//pig punkte vergeben pro Zeit!
//killerfarbe schwarz; evtl. eigenes Raumfahrzeug;
//Low; Codeoptimierung und Dokumentation

/**
@brief
    Short Gaming Manual:
    There are three different parties a player can belong to: victim, chaser or killer
    Every player starts as chaser. As long as there are not enough victims and killers, you can change your and other player's parties by shooting them.
    In order to win you have to earn as much points as possible:
    - as victim by escaping the chasers
    - as chaser by shooting the victim
    - as killer by killing the chasers


    What you shouldn't do is shooting at players of your own party. By doing so your score will decrease.
    P.S: If you don't want to be a victim: Get rid of your part by shooting a chaser.
*/
#include "Dynamicmatch.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/command/Executor.h"
#include "chat/ChatManager.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/SpaceShip.h"
#include "core/config/ConfigValueIncludes.h"
#include "interfaces/TeamColourable.h"
#include "items/Engine.h"
#include "tools/Timer.h"
#include "worldentities/TeamSpawnPoint.h"

namespace orxonox
{
    RegisterUnloadableClass(Dynamicmatch);

    Dynamicmatch::Dynamicmatch(Context* context) : Gametype(context)
    {
        RegisterObject(Dynamicmatch);
        this->gameTime_ = 180;
        this->setConfigValues();
        this->chaser=0;
        this->piggy=1;
        this->killer=2;
        this->notEnoughPigs=false;
        this->notEnoughKillers=false;
        this->notEnoughChasers=false;
        this->gameEnded_ =false;
        this->timesequence_ = static_cast<int>(this->gameTime_);
        this->friendlyfire=true;
        this->numberOf[chaser]=0;
        this->numberOf[piggy]=0;
        this->numberOf[killer]=0;
        this->tutorial=true;
        this->pointsPerTime=1.0f;
        this->setHUDTemplate("DynamicmatchHUD");
    }

    void Dynamicmatch::setConfigValues()
    {
        SetConfigValue(gameTime_, 180);
        SetConfigValue(friendlyfire, true);
        SetConfigValue(tutorial, true);
        static ColourValue colours[] =
        {
            ColourValue(1.0f, 0.3f, 0.3f),  //chasercolour
            ColourValue(0.3f, 0.3f, 1.0f),  //piggycolour
            ColourValue(0.3f, 1.0f, 0.3f)   //killercolour  what about black: 0.0f, 0.0f, 0.0f
        };
        static std::vector<ColourValue> defaultcolours(colours, colours + sizeof(colours) / sizeof(ColourValue));

        SetConfigValue(partyColours_, defaultcolours);
    }

    bool Dynamicmatch::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        //TODO: static and fading message for the "human" player's
        if (!originator||!victim)
            return false;
        if (!originator->getPlayer()||!victim->getPlayer())
            return false;
        if (victim && victim->getPlayer()) //&& originator && originator->getPlayer() ??
        {
            int target = playerParty_[victim->getPlayer()];
            int source = playerParty_[originator->getPlayer()];

            //Case: Not Enough Pigs: party change (= party management)
            if (notEnoughPigs)
            {
                numberOf[target]--; //decrease numberof victims's party
                playerParty_[victim->getPlayer()]=piggy; //victim's new party: pig
                setPlayerColour(victim->getPlayer()); //victim's new colour
                numberOf[piggy]++; //party switch: number of players is not affected (decrease and increase)

                    if(tutorial) //announce party switch
                    {
                         std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(victim->getPlayer());
                         if (it2 != this->players_.end())
                         {
                              this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it2->first->getClientID(), partyColours_[chaser]);
                              this->gtinfo_->sendFadingMessage("You're now a victim.",it2->first->getClientID());
                         }
                    }
                    if (notEnoughKillers) //reward the originator
                    {
                        numberOf[source]--; //decrease numberof originator's party
                        playerParty_[originator->getPlayer()]=killer; //originator's new party: killer
                        setPlayerColour(originator->getPlayer()); //originator's new colour
                        numberOf[killer]++;

                        if(tutorial) //announce party switch
                        {
                             std::map<PlayerInfo*, Player>::iterator it3 = this->players_.find(originator->getPlayer());
                             if (it3 != this->players_.end())
                             {
                                  this->gtinfo_->sendStaticMessage("Take the chasers down.",it3->first->getClientID(), partyColours_[chaser]);
                                  this->gtinfo_->sendFadingMessage("You're now a killer.",it3->first->getClientID());
                             }
                        }
                    }
                evaluatePlayerParties(); //check if the party change has to trigger futher party changes

                //Give new pig boost
                SpaceShip* spaceship = orxonox_cast<SpaceShip*>(victim);
                this->grantPigBoost(spaceship);
            }

            //Case: notEnoughKillers: party change
            else if (notEnoughKillers)
            {
                numberOf[source]--; //decrease numberof originator's party
                playerParty_[originator->getPlayer()]=killer; //originator's new party: killer
                setPlayerColour(originator->getPlayer()); //originator colour
                numberOf[killer]++; //party switch: number of players is not affected (decrease and increase)


                if(tutorial) //announce party switch
                {
                     std::map<PlayerInfo*, Player>::iterator it3 = this->players_.find(originator->getPlayer());
                     if (it3 != this->players_.end())
                     {
                          this->gtinfo_->sendStaticMessage("Take the chasers down.",it3->first->getClientID(),partyColours_[chaser]);
                          this->gtinfo_->sendFadingMessage("You're now a killer.",it3->first->getClientID());
                     }
                }
                evaluatePlayerParties(); //check if the party change has to trigger futher party changes
            }
            //Case: notEnoughChasers: party change
            else if (notEnoughChasers)
            {
                numberOf[target]--; //decrease numberof victims's party
                playerParty_[victim->getPlayer()]=chaser; //victim's new party: chaser
                setPlayerColour(victim->getPlayer()); //victim colour
                numberOf[chaser]++; //party switch: number of players is not affected (decrease and increase)

                if(tutorial) //announce party switch
                {
                     std::map<PlayerInfo*, Player>::iterator it3 = this->players_.find(originator->getPlayer());
                     if (it3 != this->players_.end())
                     {
                          if (numberOf[killer]>0)
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it3->first->getClientID(),partyColours_[piggy]);

                          else
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it3->first->getClientID(),partyColours_[piggy]);
                          this->gtinfo_->sendFadingMessage("You're now a chaser.",it3->first->getClientID());
                     }
                }
                evaluatePlayerParties(); //check if the party change has to trigger futher party changes
            }

            //Case: chaser vs. killer
            else if ((source == killer && target == chaser)||(target == killer && source == chaser ))
            {
                return true;
            }

            //Case: a chaser hits piggy
            else if ((source==chaser)&&(target==piggy))
            {
                std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                if (it != this->players_.end())
                    {
                        it->second.frags_++;
                    }
            }
            //Case: piggy hits chaser
            else if (source==piggy&&target==chaser)
            {
                //partyswitch: victim bcomes piggy and the originator(piggy) becomes chaser
                playerParty_[victim->getPlayer()]=piggy;
                playerParty_[originator->getPlayer()]=chaser;

                //party switch -> colour switch
                setPlayerColour(victim->getPlayer()); //victim colour
                setPlayerColour(originator->getPlayer());//originator colour

                //Announce pary switch
                if(tutorial)
                {
                     std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                     if (it != this->players_.end())
                     {
                          if (numberOf[killer]>0)
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID(), partyColours_[piggy]);
                          else
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID(),partyColours_[piggy]);
                          this->gtinfo_->sendFadingMessage("You're now a chaser.",it->first->getClientID());
                     }
                     std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(victim->getPlayer());
                     if (it2 != this->players_.end())
                     {
                          this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it2->first->getClientID(),partyColours_[chaser]);
                          this->gtinfo_->sendFadingMessage("You're now a victim.",it2->first->getClientID());
                     }
                }
                //Give new pig boost
                SpaceShip* spaceship = orxonox_cast<SpaceShip*>(victim);
                this->grantPigBoost(spaceship);
            }
            // killer vs piggy
            else if (source==killer &&target==piggy) //party and colour switch
            {
            playerParty_[victim->getPlayer()]=killer;
            playerParty_[originator->getPlayer()]=piggy;

            setPlayerColour(victim->getPlayer()); //victim colour
            setPlayerColour(originator->getPlayer()); //originator colour

            if(tutorial) //Announce pary switch
            {
                 std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                 if (it != this->players_.end())
                 {
                      this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID(),partyColours_[chaser]);
                      this->gtinfo_->sendFadingMessage("You're now a victim.",it->first->getClientID());
                 }
                 std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(victim->getPlayer());
                 if (it2 != this->players_.end())
                 {
                      this->gtinfo_->sendStaticMessage("Take the chasers down.",it2->first->getClientID(),partyColours_[chaser]);
                      this->gtinfo_->sendFadingMessage("You're now a killer.",it2->first->getClientID());
                 }
                }
            }
            //Case: friendly fire
            else if (friendlyfire && (source == target))
            {
           	    this->playerScored(originator->getPlayer(), -1);
            }
        }
        return false; //default: no damage
    }



    bool Dynamicmatch::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        //killers can kill chasers and killers can be killed by chasers
        if ((playerParty_[originator->getPlayer()] == killer && playerParty_[victim->getPlayer()] == chaser)||(playerParty_[victim->getPlayer()] == killer &&
        playerParty_[originator->getPlayer()] == chaser ))
        {
            if (playerParty_[originator->getPlayer()] == killer) //reward the killer
            {
           	    this->playerScored(originator->getPlayer(), 25);
            }
            return true;
        }
        else return false;
    }

    /**
    @brief
        Grant the victim a boost.
    @param spaceship
        The SpaceShip to give the boost.
    */
    void Dynamicmatch::grantPigBoost(SpaceShip* spaceship)
    {
        // Give victim boost
        if (spaceship)
        {
            WeakPtr<SpaceShip>* ptr = new WeakPtr<SpaceShip>(spaceship);
            if(ptr == NULL)
                return;
            spaceship->addSpeedFactor(5);
            ExecutorPtr executor = createExecutor(createFunctor(&Dynamicmatch::resetSpeedFactor, this));
            executor->setDefaultValue(0, ptr);
            new Timer(10, false, executor, true);
        }
    }

    void Dynamicmatch::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn) //set party + colouring
    {
        if (!player)
            return;

        Dynamicmatch::setPlayerColour(player); //Set playercolour
        evaluatePlayerParties();
    }

    void Dynamicmatch::playerEntered(PlayerInfo* player) //standardfunction
    {
        if (!player)// only for safety
            return;
        playerParty_[player]=chaser; //Set playerparty
        numberOf[chaser]++;
        Gametype::playerEntered(player);
        const std::string& message = player->getName() + " entered the game";
        ChatManager::message(message);
    }

    bool Dynamicmatch::playerLeft(PlayerInfo* player) //standardfunction
    {
        bool valid_player = Gametype::playerLeft(player);
        if (valid_player)
        {
            switch (playerParty_[player])
            {
            case 0: numberOf[chaser]--; break;
            case 1: numberOf[piggy]--; break;
            case 2: numberOf[killer]--; break;
            }
            const std::string& message = player->getName() + " left the game";
            ChatManager::message(message);
            //remove player from map
            playerParty_.erase (player);
            //adjust player parties
            evaluatePlayerParties();
        }

        return valid_player;
    }


    void Dynamicmatch::tick(float dt)
    {
        SUPER(Dynamicmatch, tick, dt);

        if (this->hasStarted() && !gameEnded_)
        {
            pointsPerTime = pointsPerTime + dt; //increase points
            gameTime_ = gameTime_ - dt; // decrease game time
            if (pointsPerTime > 2.0f) //hard coded points for victim! should be changed
            {
                pointsPerTime = 0.0f;
                rewardPig();
            }
            if (gameTime_<= 0)
            {
                this->gameEnded_ = true;
                this->end();
            }
            if ( gameTime_ <= timesequence_ && gameTime_ > 0)
            {
                const std::string& message = multi_cast<std::string>(timesequence_) + " seconds left!";

                this->gtinfo_->sendAnnounceMessage(message);

                if (timesequence_ >= 30 && timesequence_ <= 60)
                {
                    timesequence_ = timesequence_ - 10;
                }
                else if (timesequence_ <= 30)
                {
                    timesequence_ = timesequence_ - 5;
                }
                else
                {
                    timesequence_ = timesequence_ - 30;
                }
            }
        }
    }

/**
    @brief The reward function is called every 2 seconds via the tick function and makes the victim score points.
*/
    void Dynamicmatch::rewardPig()
    {
        for (std::map< PlayerInfo*, int >::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it) //durch alle Spieler iterieren und alle piggys finden
        {
            if (it->second==piggy)//Spieler mit der Pig-party frags++
            {
            	 this->playerScored(it->first);
            }
        }
    }
    void Dynamicmatch::setPlayerColour(PlayerInfo* player) // sets a player's colour
    {
        std::map<PlayerInfo*, int>::const_iterator it_player = this->playerParty_.find(player);
        Pawn* pawn = orxonox_cast<Pawn*>(player->getControllableEntity());
            if (pawn)
            {
                pawn->setRadarObjectColour(this->partyColours_[it_player->second]);

                std::set<WorldEntity*> pawnAttachments = pawn->getAttachedObjects();
                for (std::set<WorldEntity*>::iterator it = pawnAttachments.begin(); it != pawnAttachments.end(); ++it)
                {
                    if ((*it)->isA(Class(TeamColourable)))
                    {
                        TeamColourable* tc = orxonox_cast<TeamColourable*>(*it);
                        tc->setTeamColour(this->partyColours_[it_player->second]);
                    }
                }
            }
    }

    void Dynamicmatch::evaluatePlayerParties() //manages the notEnough booleans (here the percentage of special players is implemented)
    {
        //pigs: 1 + every 6th player is a pig
        if ( (1 + getPlayerCount()/6) > numberOf[piggy])
        {
            notEnoughPigs=true;
            if (tutorial) // Announce selectionphase
            {
             for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (!it->first)//in order to catch nullpointer
                        continue;
                    if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                        continue;
                    this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
                }
            }
        }
        else
        {
             notEnoughPigs=false;
             if(tutorial&&(!notEnoughKillers)&&(!notEnoughChasers)) //Selection phase over
             {
                  for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                  {
                       if (!it->first)//in order to catch nullpointer
                           continue;
                       if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                           continue;
                       else if (it->second==chaser)
                       {
                           if (numberOf[killer]>0)
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID(),partyColours_[piggy]);
                           else
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID(),partyColours_[piggy]);
                           //this->gtinfo_->sendFadingMessage("You're now a chaser.",it->first->getClientID());
                       }
                       else if (it->second==piggy)
                       {
                           this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID(),partyColours_[chaser]);
                           //this->gtinfo_->sendFadingMessage("You're now a victim.",it->first->getClientID());
                       }
                       else if (it->second==killer)
                       {
                           this->gtinfo_->sendStaticMessage("Take the chasers down.",it->first->getClientID(),partyColours_[chaser]);
                           //this->gtinfo_->sendFadingMessage("You're now a killer.",it->first->getClientID());
                       }
                  }

             }
        }
        //killers: every 4th player is a killer
        if ( static_cast<unsigned int>(getPlayerCount()/4) > numberOf[killer])
        {
            notEnoughKillers=true;
            if (tutorial) // Announce selectionphase
            {
             for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (!it->first)//in order to catch nullpointer
                        continue;
                    if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                        continue;
                    this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
                }
            }
        }
        else
        {
            notEnoughKillers=false;
            if(tutorial&&(!notEnoughPigs)&&(!notEnoughChasers)) //Selection phase over
             {
                  for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                  {
                       if (!it->first)
                           continue;
                       if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                           continue;
                       else if (it->second==chaser)
                       {
                           if (numberOf[killer]>0)
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID(),partyColours_[piggy]);
                           else
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID(),partyColours_[piggy]);
                           //this->gtinfo_->sendFadingMessage("You're now a chaser.",it->first->getClientID());
                       }
                       else if (it->second==piggy)
                       {
                           this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID(),partyColours_[piggy]);
                           //this->gtinfo_->sendFadingMessage("You're now a victim.",it->first->getClientID());
                       }
                       else if (it->second==killer)
                       {
                           this->gtinfo_->sendStaticMessage("Take the chasers down.",it->first->getClientID(),partyColours_[piggy]);
                           //this->gtinfo_->sendFadingMessage("You're now a killer.",it->first->getClientID());
                       }
                  }

             }

        }
        //chasers: there are more chasers than killers + pigs
        if (numberOf[piggy]+numberOf[killer] > numberOf[chaser])
        {
            notEnoughChasers=true;
            if (tutorial) // Announce selectionphase
            {
             for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (!it->first)//in order to catch nullpointer
                        continue;
                    if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                        continue;
                    this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
                }
            }
        }
        else
        {
             notEnoughChasers=false;
             if(tutorial&&(!notEnoughPigs)&&(!notEnoughKillers)) //Selection phase over
             {
                  for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                  {
                       if (!it->first)
                           continue;
                       if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                           continue;
                       else if (it->second==chaser)
                       {
                           if (numberOf[killer]>0)
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID(),partyColours_[piggy]);
                           else
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID(),partyColours_[piggy]);
                           //this->gtinfo_->sendFadingMessage("You're now a chaser.",it->first->getClientID());
                       }
                       else if (it->second==piggy)
                       {
                           this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID(),partyColours_[chaser]);
                           //this->gtinfo_->sendFadingMessage("You're now a victim.",it->first->getClientID());
                       }
                       else if (it->second==killer)
                       {
                           this->gtinfo_->sendStaticMessage("Take the chasers down.",it->first->getClientID(),partyColours_[chaser]);
                           //this->gtinfo_->sendFadingMessage("You're now a killer.",it->first->getClientID());
                       }
                  }

             }
        }
    }

    int Dynamicmatch::getParty(PlayerInfo* player) // helper function for ArtificialController
    {
        return this->playerParty_[player];
    }

    void Dynamicmatch::resetSpeedFactor(WeakPtr<SpaceShip>* ptr)// helper function
    {
        if (*ptr)
        {
            (*ptr)->addSpeedFactor(1.0f/5.0f);
        }
        delete ptr;
    }

    bool Dynamicmatch::playerChangedName(PlayerInfo* player) //standardfunction
    {
        bool valid_player = Gametype::playerChangedName(player);
        if (valid_player)
        {
            const std::string& message = player->getOldName() + " changed name to " + player->getName();
            ChatManager::message(message);
        }

        return valid_player;
    }

    void Dynamicmatch::start()
    {
        Gametype::start();
        if(!tutorial)
        {
            std::string message("Dynamicmatch started!");
            ChatManager::message(message);
        }
        else if(tutorial) // Announce selectionphase
        {
            for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
            {
                if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                    continue;
                this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID(),ColourValue(1.0f, 1.0f, 0.5f));
            }
        }
    }

    /*void Dynamicmatch::instructions()
    {
        std::string message("Earn points:\n\n\n\tIf you're red: Chase the blue player!\n\n\tIf you're blue shoot at a red player or hide.\n\n\tIf you're green: You've got the licence to kill red players!");
        ChatManager::message(message);
        callInstructions_.setTimer(10, false, createExecutor(createFunctor(&Dynamicmatch::furtherInstructions, this)));
    }

    void Dynamicmatch::furtherInstructions()
    {
        std::string message("After 3 Minutes the game is over.");
        ChatManager::message(message);
    }*/
    void Dynamicmatch::end()
    {
        Gametype::end();

        std::string message("Time out. Press F2 to see the points you scored.");
        ChatManager::message(message);
    }
    SpawnPoint* Dynamicmatch::getBestSpawnPoint(PlayerInfo* player) const
    {
        int desiredTeamNr = -1;
        std::map<PlayerInfo*, int>::const_iterator it_player = this->playerParty_.find(player);
        if (it_player != this->playerParty_.end())
            desiredTeamNr = it_player->second;

        // Only use spawnpoints of the own team (or non-team-spawnpoints)
        std::set<SpawnPoint*> teamSpawnPoints = this->spawnpoints_;
        for (std::set<SpawnPoint*>::iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); )
        {
            if ((*it)->isA(Class(TeamSpawnPoint)))
            {
                TeamSpawnPoint* tsp = orxonox_cast<TeamSpawnPoint*>(*it);
                if (tsp && static_cast<int>(tsp->getTeamNumber()) != desiredTeamNr)//getTeamNumber!!
                {
                    teamSpawnPoints.erase(it++);
                    continue;
                }
            }

            ++it;
        }

        if (teamSpawnPoints.size() > 0)
        {
            unsigned int randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(teamSpawnPoints.size())));
            unsigned int index = 0;
            for (std::set<SpawnPoint*>::const_iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); ++it)
            {
                if (index == randomspawn)
                    return (*it);

                ++index;
            }
        }

        return 0;
    }

}
