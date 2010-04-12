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

#include <map>
#include <vector>
#include "OrxonoxPrereqs.h"
#include "Gametype.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport Dynamicmatch : public Gametype
    {
        public:
            Dynamicmatch(BaseObject* creator);
            virtual ~Dynamicmatch() {}
		
		bool onlyChasers;
		int getParty(PlayerInfo* player);
		void setPlayerColour(PlayerInfo* player);//own function
		void setConfigValues();//done
		virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0); //ok - score function and management of parties
            	virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0); //ok - simple
            virtual void start();

		  
            virtual void end(); //Wie geht das mit der Punkteausgabe? frags als Schnittstelle ausreichend?
            virtual void playerEntered(PlayerInfo* player);
		virtual void playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn);//is used to initialize the player's party and colour
            virtual bool playerLeft(PlayerInfo* player);//ToDo: extract the player's party record - done?
            virtual bool playerChangedName(PlayerInfo* player);//unchanged
	
            
		void tick (float dt);// used to end the game
		
		//inline const ColourValue& getPlayerColour(int teamnr) const
                //{ return this->partyColours_[teamnr]; }
		
		protected:
		  //the two different parties
		  int chaser;
		  int piggy;
		  std::map< PlayerInfo*, int > playerParty_; //player's parties are recorded here
		  std::vector<ColourValue> partyColours_; //aus TeamDeathmatch		  
		bool friendlyfire; //goal: player can switch it on/off
		float gameTime_;   // from UnderAttack
		bool gameEnded_; // true if game is over
		int timesequence_; //used for countdown
    };
}

#endif /* _Dynamicmatch_H__ */
