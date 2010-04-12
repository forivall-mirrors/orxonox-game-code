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

#include "Dynamicmatch.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "core/ConfigValueIncludes.h"
#include "interfaces/TeamColourable.h"

//toDO: maybe the has entered the game function is not enough... look at TeamDeathmath
namespace orxonox
{
    CreateUnloadableFactory(Dynamicmatch);

    Dynamicmatch::Dynamicmatch(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(Dynamicmatch);
	this->gameTime_ = 180;
	this->setConfigValues();
        this->chaser=0;
	this->piggy=1;
	this->onlyChasers=true;
	this->gameEnded_ =false;
	this->timesequence_ = static_cast<int>(this->gameTime_);
	this->friendlyfire=true;
    }

void Dynamicmatch::setPlayerColour(PlayerInfo* player) // not sure if this is the right place - helper function
{
	std::map<PlayerInfo*, int>::const_iterator it_player = this->playerParty_.find(player);
        //if (it_player != this->playerParty_.end() && it_player->second >= 0 && it_player->second < static_cast<int>(this->partyColours_.size()))
        // all players are the same colour at the beginning!!
            Pawn* pawn = dynamic_cast<Pawn*>(player->getControllableEntity());
            if (pawn)
            {
                pawn->setRadarObjectColour(this->partyColours_[it_player->second]); //does this work? //what about playerParty_[it_player] instead of it_player->second

                std::set<WorldEntity*> pawnAttachments = pawn->getAttachedObjects();
                for (std::set<WorldEntity*>::iterator it = pawnAttachments.begin(); it != pawnAttachments.end(); ++it)
                {
                    if ((*it)->isA(Class(TeamColourable)))//not sure if this works
                    {
                        TeamColourable* tc = orxonox_cast<TeamColourable*>(*it);
                        tc->setTeamColour(this->partyColours_[it_player->second]);
                    }
                }
            }
}


int Dynamicmatch::getParty(PlayerInfo* player) // helper function for ArtificialController
{
return this->playerParty_[player];
}



void Dynamicmatch::setConfigValues()
    {
        SetConfigValue(gameTime_, 180);//just for test cases
	SetConfigValue(friendlyfire, true);
	static ColourValue colours[] =
        {
	    ColourValue(0.3f, 0.3f, 1.0f),
            ColourValue(1.0f, 0.3f, 0.3f)
            
        };
        static std::vector<ColourValue> defaultcolours(colours, colours + sizeof(colours) / sizeof(ColourValue));

        SetConfigValue(partyColours_, defaultcolours);
    }

bool Dynamicmatch::allowPawnDamage(Pawn* victim, Pawn* originator)//
    {	

        if (victim && victim->getPlayer())
        { 	//TODO: evtl. ->getPlayer() zugriffe auslagern fuer mehr uebersicht
		//Case: 1. onlyChasers==false und victim ist chaser
		if ((!onlyChasers)&&(playerParty_[originator->getPlayer()]==chaser)) {
			std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                    if (it != this->players_.end())
                    {
                        it->second.frags_++;
                    }
		}
		//Case 2: onlyChasers==false und victim ist piggy
		else if ((!onlyChasers)&&(playerParty_[originator->getPlayer()]==piggy)){
			//partyswitch: victim bcomes piggy and the orginator(piggy) becomes chaser
			playerParty_[victim->getPlayer()]=piggy;
			playerParty_[originator->getPlayer()]=chaser;
			//announce
			const std::string& messageVictim = victim->getPlayer()->getName() + " is victim";
        		COUT(0) << messageVictim << std::endl;
			Host::Broadcast(messageVictim);
		//party switch -> colour switch
		
		setPlayerColour(victim->getPlayer()); //victim colour
		setPlayerColour(originator->getPlayer());//orginator colour
		}
		//Case 3: there are only chasers -> new piggy is needed
		else if (onlyChasers){
			//Rollenzuweisung victim wird piggy
			playerParty_[victim->getPlayer()]=piggy;
			onlyChasers=false;
			setPlayerColour(victim->getPlayer()); //victim colour
		std::string message("First victim.");
        	COUT(0) << message << std::endl;
		Host::Broadcast(message);
		}
		else if (friendlyfire)
		{
		std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
		 if (it != this->players_.end())
                    {
                        it->second.frags_--;
                    }
		}
        }
		
	return false;
    }

        
    

bool Dynamicmatch::allowPawnDeath(Pawn* victim, Pawn* originator)//
    {
        return false;
    }

void Dynamicmatch::start() 
    {	
        Gametype::start();

        std::string message("Don't be a victim!");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
	
    }

    void Dynamicmatch::end()
    {
        Gametype::end();

        std::string message("Time out. Press F2 to see the soreboard");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
	/*for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
                {
                    if (it->first->getClientID() == CLIENTID_UNKNOWN)
                        continue;

                    if (it->second == 1)
                        this->gtinfo_->sendAnnounceMessage("You have won the match!", it->first->getClientID());
                    else
                        this->gtinfo_->sendAnnounceMessage("You have lost the match!", it->first->getClientID());
                }*/
    }

    
void Dynamicmatch::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        if (!player)
            return;
	playerParty_[player]=chaser;//playerparty
	// Set the playercolour
 	Dynamicmatch::setPlayerColour(player);

	
    }

    void Dynamicmatch::playerEntered(PlayerInfo* player) //standardfunction + party + colouring
    {

	if (!player)// only for safety
            return;

        Gametype::playerEntered(player);
	
	//playerParty_[player]=chaser;//playerparty
	
        // Set the playercolour
        //Dynamicmatch::setPlayerColour(player);
	
	
        const std::string& message = player->getName() + " entered the game";
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    bool Dynamicmatch::playerLeft(PlayerInfo* player) //standardfunction
    {
        bool valid_player = Gametype::playerLeft(player);
	
	
        if (valid_player)
        {
            const std::string& message = player->getName() + " left the game";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
		//remove player from map
	     playerParty_.erase (player);  
        }

        return valid_player;
    }

    bool Dynamicmatch::playerChangedName(PlayerInfo* player) //standardfunction
    {
        bool valid_player = Gametype::playerChangedName(player);

        if (valid_player)
        {
            const std::string& message = player->getOldName() + " changed name to " + player->getName();
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return valid_player;
    }

    

 void Dynamicmatch::tick(float dt)
    {
        SUPER(Dynamicmatch, tick, dt);//TODO - was bedeutet diese Zeile

        if (this->hasStarted() && !gameEnded_)
        {
            gameTime_ = gameTime_ - dt;
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
}
