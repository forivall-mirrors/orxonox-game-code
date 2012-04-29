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
 *NACHRICHT:
 * Ich habe die Klasse Deathmatch einfach per Copy&Paste&Rename als Vorlage für euren Deathmatch genommen.
 * Ein Deathmatch erbt vom Gametype. Der einzige Unterschied zum Gametype ist, dass hier ein bisschen
 * Textausgabe stattfindet. Sollte das Später nicht erwünscht sein, könnt ihr einfach die Gametype-Klasse
 * an die Stelle von Deathmatch setzten.
 *
 * Hier empfehle ich euch die gesamte Spielogik unter zu bringen. Viele Funktionen werden automatisch
 * bei gewissen Ereignissen aufgerufen bzw. lösen Ereignisse aus
 *
 *Z.B:
 * start() //wird aufgerufen, bevor das Spiel losgeht
 * end() //wenn man diese Funktion aufruft wird
 * pawnKilled() // wird aufgerufen, wenn ein Pawn stirbt (z.B: wenn )
 * playerScored() // kann man aufrufen um dem Spieler Punkte zu vergeben.
 *
 *
 *
 *TIPP: Eclipse hilft euch schnell auf bereits vorhanden Funktionen zuzugreifen:
 * einfach "this->" eingeben und kurz warten. Dann tauch eine Liste mit Vorschlägen auf. Wenn ihr jetzt weiter
 * tippt, werden die Vorschläge entsprechend gefiltert.
 *
 *
 *TIPP: schaut euch mal Tetris::createStone() an. Dort wird ein TetrisStone-Objekt (ControllableEntity) erzeugt,
 * ihm ein Template zugewiesen (welches vorher im Level definiert wurde und dem CenterPoint übergeben wurde)
 * Ähnlich könnt ihr vorgehen, um einen Turm zu erzeugen. (Zusätzlich braucht ein Turm noch einen Controller)
 * z.B: WaypointPatrolController. Wenn kein Team zugewiesen wurde bekämpft ein WaypointPatrolController alles,
 * was in seiner Reichweite liegt.
 *
 */

#include "TowerDefense.h"
#include "Tower.h"
#include "TowerDefenseCenterpoint.h"

#include "worldentities/SpawnPoint.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/SpaceShip.h"
#include "controllers/WaypointController.h"
	 
#include "graphics/Model.h"
#include "infos/PlayerInfo.h"
	 
#include "chat/ChatManager.h"

/* Part of a temporary hack to allow the player to add towers */
#include "core/command/ConsoleCommand.h"

namespace orxonox
{
    CreateUnloadableFactory(TowerDefense);
	
	TowerDefense::TowerDefense(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(TowerDefense);
		
		/* Temporary hack to allow the player to add towers */
		this->dedicatedAddTower_ = createConsoleCommand( "addTower", createExecutor( createFunctor(&TowerDefense::addTower, this) ) );
	
		// Quick hack to test waypoints
		createConsoleCommand( "aw", createExecutor( createFunctor(&TowerDefense::addWaypointsAndFirstEnemy, this) ) );
    }
	
    TowerDefense::~TowerDefense()
    {
		/* Part of a temporary hack to allow the player to add towers */
        if (this->isInitialized())
        {
            if( this->dedicatedAddTower_ )
                delete this->dedicatedAddTower_;
        }
    }
	
	void TowerDefense::setCenterpoint(TowerDefenseCenterpoint *centerpoint)
	{
		orxout() << "Centerpoint now set..." << endl;
		this->center_ = centerpoint;
	}
	
    void TowerDefense::start()
    {
        Deathmatch::start();
		
        orxout() << "Adding towers for debug..." << endl;
		
		// Mark corners
		addTower(0,15); addTower(15,0);
		
		// Mark diagonal line
		for (int i = 0 ; i <= 15; i++)
			addTower(i,i);
		
		orxout() << "Done" << endl;
		
		ChatManager::message("Use the console command addTower x y to add towers");
	}
	
	void TowerDefense::end()
	{
		Deathmatch::end();
	 
		ChatManager::message("Match is over");
	}
	
	
	void TowerDefense::addTower(int x, int y)
	{
		if (x > 15 || y > 15 || x < 0 || y < 0)
		{
			orxout() << "Can not add Tower: x and y should be between 0 and 15" << endl;
			return;
		}
		
		orxout()<< "Will add tower at (" << x << "," << y << ")" << endl;
		
		Tower* newTower = new Tower(this->center_);
		newTower->addTemplate(this->center_->getTowerTemplate());
		
		this->center_->attach(newTower);
		
		newTower->setPosition(x-8,y-8,0);
		newTower->setGame(this);
		
		// TODO: create Tower mesh
		// TODO: load Tower mesh
	}
	
	void TowerDefense::tick(float dt)
    {
        SUPER(TowerDefense, tick, dt);
		
        static int test = 0;
        if (++test == 10)
        {
			orxout()<< "10th tick." <<endl;
			/*
			for (std::set<SpawnPoint*>::iterator it = this->spawnpoints_.begin(); it != this->spawnpoints_.end(); it++)
			{
				orxout() << "checking spawnpoint with name " << (*it)->getSpawnClass()->getName() << endl;
			}
			*/
			
			//addWaypointsAndFirstEnemy();
			
        }
    }
	
	// Function to test if we can add waypoints using code only. Doesn't work yet
	
	// THE PROBLEM: WaypointController's getControllableEntity() returns null, so it won't track. How do we get the controlableEntity to NOT BE NULL???
	
	void TowerDefense::addWaypointsAndFirstEnemy()
	{
		SpaceShip *newShip = new SpaceShip(this->center_);
		newShip->addTemplate("spaceshipassff");
		
		WaypointController *newController = new WaypointController(newShip);
		newController->setAccuracy(3);
		
		Model *wayPoint1 = new Model(newController);
		wayPoint1->setMeshSource("crate.mesh");	
		wayPoint1->setPosition(7,-7,5);
		wayPoint1->setScale(0.2);
			
		Model *wayPoint2 = new Model(newController);
		wayPoint2->setMeshSource("crate.mesh");
		wayPoint2->setPosition(7,7,5);
		wayPoint2->setScale(0.2);
			
		newController->addWaypoint(wayPoint1);
		newController->addWaypoint(wayPoint2);
			
		// The following line causes the game to crash
//		newController -> getPlayer() -> startControl(newShip);
		newShip->setController(newController);
		newShip->setPosition(-7,-7,5);
		newShip->setScale(0.1);
		newShip->addSpeed(1);
		
		
		
//		this->center_->attach(newShip);
	}
	
	/*
	 void TowerDefense::playerEntered(PlayerInfo* player)
	 {
	 Deathmatch::playerEntered(player);
	 
	 const std::string& message = player->getName() + " entered the game";
	 ChatManager::message(message);
	 }
	 
	 bool TowerDefense::playerLeft(PlayerInfo* player)
	 {
	 bool valid_player = Deathmatch::playerLeft(player);
	 
	 if (valid_player)
	 {
	 const std::string& message = player->getName() + " left the game";
	 ChatManager::message(message);
	 }
	 
	 return valid_player;
	 }
	 
	 
	 void TowerDefense::pawnKilled(Pawn* victim, Pawn* killer)
	 {
	 if (victim && victim->getPlayer())
	 {
	 std::string message;
	 if (killer)
	 {
	 if (killer->getPlayer())
	 message = victim->getPlayer()->getName() + " was killed by " + killer->getPlayer()->getName();
	 else
	 message = victim->getPlayer()->getName() + " was killed";
	 }
	 else
	 message = victim->getPlayer()->getName() + " died";
	 
	 ChatManager::message(message);
	 }
	 
	 Deathmatch::pawnKilled(victim, killer);
	 }
	 
	 void TowerDefense::playerScored(PlayerInfo* player)
	 {
	 Gametype::playerScored(player);
	 
	 }*/
}
