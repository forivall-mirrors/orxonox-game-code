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
 *
 *   Co-authors:
 *      ...
 *
 */
	 
 /**
	 @brief
	 GameType class for TowerDefense. See TowerDefenseReadme.txt for Information.
	 
	 @ingroup TowerDefense
 */
	 

#ifndef _TowerDefense_H__
#define _TowerDefense_H__

#include "towerdefense/TowerDefensePrereqs.h"
#include "gametypes/Deathmatch.h"

namespace orxonox
{
    class _OrxonoxExport TowerDefense : public Deathmatch
    {
	public:
		TowerDefense(BaseObject* creator);
		virtual ~TowerDefense();
		
		virtual void start(); //<! The function is called when the gametype starts
		virtual void end();
		virtual void tick(float dt);
		//virtual void playerEntered(PlayerInfo* player);
		//virtual bool playerLeft(PlayerInfo* player);
		
		//virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
		//virtual void playerScored(PlayerInfo* player);
		
		
		/*	Called by TowerDefenseCenterpoint upon game start 
			The centerpoint is used to create towers
		*/
		void setCenterpoint(TowerDefenseCenterpoint *centerpoint);
		
		/* Adds a tower at x, y in the playfield */
		void addTower(int x, int y);
		
		/* Part of a temporary hack to allow the player to add towers */
		ConsoleCommand* dedicatedAddTower_;
		
		//TODO: void spawnNewWave()
	    //TODO: create a timer which regularly calls the spawnNewWave function  (time driven)
		//      or spawn a new wave when the old wave has been killed           (event driven)


	private:
//		WeakPtr<TowerDefenseCenterpoint> center_;
		TowerDefenseCenterpoint *center_;
	
	
		bool hasTower(int x, int y);
		
		typedef struct {
			int x;
			int y;
		} coordinate; 
		
	
		
		std::vector<coordinate> addedTowersCoordinates_;
		std::vector<Tower*> towers_;
		
		void addWaypointsAndFirstEnemy();
    };
}

#endif /* _TowerDefense_H__ */
