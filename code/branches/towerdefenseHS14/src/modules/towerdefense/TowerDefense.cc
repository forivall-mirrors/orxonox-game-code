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
 *
 *HUD:
 * Ein Gametype kann ein HUD (Head up Display haben.) Z.B: hat Pong eine Anzeige welcher Spieler wieviele Punkte hat.
 * Generell kann man a) Grafiken oder b) Zeichen in einer HUD anzeigen.
 * Fuer den ersten Schritt reicht reiner Text.
 *
 * a)
 * PongScore.cc uebernehmen und eigene Klasse draus machen.
 * Wenn ihr bloss anzeigen wollt wieviele Punkte der Spieler bereits erspielt hat (Punkte = Kapital fuer neue Tuerme) dann orientiert ihr euch an
 * TetrisScore.cc (im pCuts branch): http://www.orxonox.net/browser/code/branches/pCuts/src/modules/tetris/TetrisScore.cc
 * Ich habe TetrisScore lediglich dazu gebraucht, um eine Variable auf dem HUD auszugeben. Ein Objekt fuer statischen Text gibt es bereits.
 *
 * b)
 * Im naesten Schritt erstellt man die Vorlage fuer das HUD-Objekt: siehe /data/overlays/pongHUD
 * OverlayText ist eine Vorlage fuer statischen text zb: "Points Scored:". Aus mir nicht erklaerlichen Gruenden sollte man die OverlayText
 * Objekte immer erst nach dem PongScore anlegen.
 *
 * c)  Im TowerDefense gamtype muss im Constructor noch das HUD-Template gesetzt werden.
 *
 * d) in CMakeLists.txt noch das Module includen das fuer die Overlays zustaendig ist. Siehe das gleiche File im Pong module.
 *
 *
 *
 */

#include "TowerDefense.h"
//#include "Tower.h"
#include "TowerTurret.h"
#include "TowerDefenseCenterpoint.h"
//#include "TDCoordinate.h"
#include "worldentities/SpawnPoint.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/SpaceShip.h"
#include "controllers/WaypointController.h"

#include "graphics/Model.h"
#include "infos/PlayerInfo.h"
#include "chat/ChatManager.h"
#include "core/CoreIncludes.h"

/* Part of a temporary hack to allow the player to add towers */
#include "core/command/ConsoleCommand.h"

namespace orxonox
{
    RegisterUnloadableClass(TowerDefense);

    TowerDefense::TowerDefense(Context* context) : Deathmatch(context)
    {
        RegisterObject(TowerDefense);

        this->setHUDTemplate("TowerDefenseHUD");

        this->stats_ = new TowerDefensePlayerStats();

        /* Temporary hack to allow the player to add towers */
        this->dedicatedAddTower_ = createConsoleCommand( "addTower", createExecutor( createFunctor(&TowerDefense::addTower, this) ) );
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
        orxout() << "Centerpoint now setting..." << endl;
        this->center_ = centerpoint;
        orxout() << "Centerpoint now set..." << endl;
    }

    void TowerDefense::start()
    {
    	orxout() << "test0" << endl;

        Deathmatch::start();

        const int kInitialTowerCount = 3;

        for (int i = 0; i < kInitialTowerCount; i++)
        {
        	//{{3,2}, {8,5}, {12,10}}; old coordinates
            TDCoordinate* coordinate = new TDCoordinate(i,(i*2));
            addTower(coordinate->x, coordinate->y);
        }

        orxout() << "test3" << endl;

        //add some TowerDefenseEnemys

        TDCoordinate* coord1 = new TDCoordinate(1,1);
        /*TDCoordinate* coord2 = new TDCoordinate(10,10);
        TDCoordinate* coord3 = new TDCoordinate(1,2);*/
        std::vector<TDCoordinate> path;
        path.push_back(*coord1);
        /*path.push_back(*coord2);
        path.push_back(*coord3);*/

        for(int i = 0 ; i <4 ; ++i)
        {
        	addTowerDefenseEnemy(path);
        }

        //ChatManager::message("Use the console command addTower x y to add towers");

        //TODO: let the player control his controllable entity && TODO: create a new ControllableEntity for the player
    }

    void TowerDefense::addTowerDefenseEnemy(std::vector<TDCoordinate> path){

    	orxout() << "test1" << endl;

    	TowerDefenseEnemy* en1 = new TowerDefenseEnemy(this->center_->getContext());
        // Model* TowerDefenseEnemymodel = new Model(this->center_->getContext());
        //TowerDefenseEnemymodel->setMeshSource("crate.mesh");
        //TowerDefenseEnemymodel->setPosition(0,0,0);
        en1->setPosition(path.at(0).get3dcoordinate());
        //TowerDefenseEnemymodel->setScale(0.2);

        //en1->attach(TowerDefenseEnemymodel);

        //TowerDefenseEnemyvector.push_back(en1);

        orxout() << "test2" << endl;

        /*for(unsigned int i = 0; i < path.size(); ++i)
        {
            en1->addWaypoint(path.at(i));
        }*/

        orxout() << "test6" << endl;

        /*WaypointController *newController = new WaypointController(en1);
        newController->setAccuracy(3);

        for(int i =0; i < path.size(); ++i)
        {

            Model *wayPoint = new Model(newController);
            wayPoint->setMeshSource("crate.mesh");
            wayPoint->setPosition(path.at(i).get3dcoordinate());
            wayPoint->setScale(0.2);

            newController->addWaypoint(wayPoint);

        }*/

    }


    void TowerDefense::end()
    {
        Deathmatch::end();

        ChatManager::message("Match is over");
    }



    void TowerDefense::addTower(int x, int y)
    {
        /*const TowerCost towerCost = TDDefaultTowerCost;

        if (!this->hasEnoughCreditForTower(towerCost))
        {
            orxout() << "not enough credit: " << (this->stats_->getCredit()) << " available, " << TDDefaultTowerCost << " needed.";
            return;
        }

        if (this->towerExists(x,y))
        {
            orxout() << "tower exists!!" << endl;
            return;
        }


        unsigned int width = this->center_->getWidth();
        unsigned int height = this->center_->getHeight();


        int tileScale = (int) this->center_->getTileScale();

        if (x > 15 || y > 15 || x < 0 || y < 0)
        {
            //Hard coded: TODO: let this depend on the centerpoint's height, width and fieldsize (fieldsize doesn't exist yet)
            orxout() << "Can not add Tower: x and y should be between 0 and 15" << endl;
            return;
        }

        orxout() << "Will add tower at (" << (x-8) * tileScale << "," << (y-8) * tileScale << ")" << endl;

        // Add tower to coordinatesStack
        TDCoordinate newTowerCoordinates;
        newTowerCoordinates.x=x;
        newTowerCoordinates.y=y;


        addedTowersCoordinates_.push_back(newTowerCoordinates);

        // Reduce credit
        this->stats_->buyTower(towerCost);

        // Create tower
        TowerTurret* newTower = new TowerTurret(this->center_->getContext());
        newTower->addTemplate(this->center_->getTowerTemplate());

        newTower->setPosition(static_cast<float>((x-8) * tileScale), static_cast<float>((y-8) * tileScale), 75);
        newTower->setGame(this);*/
    }

    bool TowerDefense::hasEnoughCreditForTower(TowerCost towerCost)
    {
        return ((this->stats_->getCredit()) >= towerCost);
    }

    bool TowerDefense::towerExists(int x, int y)
    {
        for(std::vector<TDCoordinate>::iterator it = addedTowersCoordinates_.begin(); it != addedTowersCoordinates_.end(); ++it)
        {
            TDCoordinate currentCoordinates = (TDCoordinate) (*it);
            if (currentCoordinates.x == x && currentCoordinates.y == y)
                return true;
        }

        return false;
    }


    void TowerDefense::tick(float dt)
    {
        //SUPER(TowerDefense, tick, dt);
    }

    // Function to test if we can add waypoints using code only. Doesn't work yet

    // THE PROBLEM: WaypointController's getControllableEntity() returns null, so it won't track. How do we get the controlableEntity to NOT BE NULL???
    /*
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

        newShip->setController(newController);
//        newController -> getPlayer() -> startControl(newShip);
        newShip->setPosition(-7,-7,5);
        newShip->setScale(0.1);
        //newShip->addSpeed(1);



//      this->center_->attach(newShip);
    }
    */
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

    void TowerDefense::playerScored(PlayerInfo* player, int score)
    {
        Gametype::playerScored(player, score);
    }*/
}
