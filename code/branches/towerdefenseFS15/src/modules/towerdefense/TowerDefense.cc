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
 *  Author:
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
#include "TowerDefenseTower.h"
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
    static const std::string __CC_addTower_name  = "addTower";
    static const std::string __CC_upgradeTower_name = "upgradeTower";

    SetConsoleCommand("TowerDefense", __CC_addTower_name,  &TowerDefense::addTower ).addShortcut().defaultValues(1);
    SetConsoleCommand("TowerDefense", __CC_upgradeTower_name, &TowerDefense::upgradeTower).addShortcut().defaultValues(0);

    RegisterUnloadableClass(TowerDefense);

    TowerDefense::TowerDefense(Context* context) : TeamDeathmatch(context)
    {
        RegisterObject(TowerDefense);
/*
        for (int i=0; i < 16 ; i++){
            for (int j = 0; j< 16 ; j++){
                towermatrix[i][j] = NULL;
            }
        }*/

        selecter = NULL;


        this->setHUDTemplate("TowerDefenseHUD");
        this->nextwaveTimer_.setTimer(10, false, createExecutor(createFunctor(&TowerDefense::nextwave, this)));
        this->nextwaveTimer_.stopTimer();
        this->waves_ = 0;
        this->time = 0;
        this->credit_ = 0;
        this->lifes_ = 0;

        //this->stats_ = new TowerDefensePlayerStats();

        ModifyConsoleCommand(__CC_addTower_name).setObject(this);
        ModifyConsoleCommand(__CC_upgradeTower_name).setObject(this);
    }

    TowerDefense::~TowerDefense()
    {
        /* Part of a temporary hack to allow the player to add towers */
        if (this->isInitialized())
        {
            ModifyConsoleCommand(__CC_addTower_name).setObject(NULL);
            ModifyConsoleCommand(__CC_upgradeTower_name).setObject(NULL);
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

        TeamDeathmatch::start();

// Waypoints: [1,3] [10,3] [10,11] [13,11] -> add the points to a matrix so the player cant place towers on the path
        for (int i=0; i < 16 ; i++)
        {
            for (int j = 0; j< 16 ; j++)
            {
                towerModelMatrix[i][j] = NULL;
                towerTurretMatrix[i][j] = NULL;
            }
        }

        selecter = new TowerDefenseSelecter(this->center_->getContext());

        Model* dummyModel = new Model(this->center_->getContext());

        //the path of the spacehips has to be blocked, so that no towers can be build there
        for (int k=0; k<3; k++)
            towerModelMatrix[1][k]=dummyModel;
        for (int l=1; l<11; l++)
        	towerModelMatrix[l][3]=dummyModel;
        for (int m=3; m<12; m++)
        	towerModelMatrix[10][m]=dummyModel;
        for (int n=10; n<14; n++)
        	towerModelMatrix[n][11]=dummyModel;
        for (int o=13; o<16; o++)
        	towerModelMatrix[13][o]=dummyModel;


        //set initial credits, lifes and WaveNumber
        this->setCredit(1000);
        this->setLifes(100);
        this->setWaveNumber(0);
        time=0.0;

        /*
        //adds initial towers
        for (int i=0; i <7; i++){
            addTower(i+3,4);
        }
		*/
    }

    // Generates a TowerDefenseEnemy. Uses Template "enemytowerdefense". Sets position at first waypoint of path.
    void TowerDefense::addTowerDefenseEnemy(std::vector<TDCoordinate*> path, int templatenr){


        TowerDefenseEnemy* en1 = new TowerDefenseEnemy(this->center_->getContext());
        
        switch(templatenr)
        {
        case 1 :
            en1->addTemplate("enemytowerdefense1");
            en1->setScale(3);
            en1->setHealth(en1->getHealth() + this->getWaveNumber()*4);
            break;

        case 2 :
            en1->addTemplate("enemytowerdefense2");
            en1->setScale(2);
            en1->setHealth(en1->getHealth() + this->getWaveNumber()*4);
            //  en1->setShieldHealth(en1->getShield() = this->getWaveNumber()*2))
            break;

        case 3 :
            en1->addTemplate("enemytowerdefense3");
            en1->setScale(1);
            en1->setHealth(en1->getHealth() + this->getWaveNumber()*4);
            break;
        }

        en1->setTeam(2);
        en1->getController();
        en1->setPosition(path.at(0)->get3dcoordinate());
        TowerDefenseEnemyvector.push_back(en1);

        for(unsigned int i = 0; i < path.size(); ++i)
        {
            en1->addWaypoint((path.at(i)));
        }
    }


    void TowerDefense::end()
    {

        TeamDeathmatch::end();
        ChatManager::message("Match is over! Gameover!");

    }

    //not working yet
    void TowerDefense::upgradeTower(int x,int y)
    {
        const int upgradeCost = 20;

        if (!this->hasEnoughCreditForTower(upgradeCost))
        {
            orxout() << "not enough credit: " << (this->getCredit()) << " available, " << upgradeCost << " needed.";
            return;
        }


        Model* dummyModel2 = new Model(this->center_->getContext());

        if (towerModelMatrix [x][y] == NULL || (towerModelMatrix [x][y])->getMeshSource() == dummyModel2->getMeshSource())
        {
            orxout() << "no tower on this position" << endl;
            return;
        }

        else
        {
            (towerTurretMatrix [x][y])->upgradeTower();
            this->buyTower(upgradeCost);
        }
    }

    /*adds Tower at Position (x,y) and reduces credit and adds the point to the towermatrix. template ("towerturret")
    so towers have ability if the turrets

    */
    void TowerDefense::addTower(int x, int y)
    {
        const int towerCost = 20;

        if (!this->hasEnoughCreditForTower(towerCost))
        {
            orxout() << "not enough credit: " << (this->getCredit()) << " available, " << towerCost << " needed.";
            return;
        }

        if (towerModelMatrix [x][y]!=NULL)
        {
            orxout() << "not possible to put tower here!!" << endl;
            return;
        }

/*
        unsigned int width = this->center_->getWidth();
        unsigned int height = this->center_->getHeight();
*/

        int tileScale = (int) this->center_->getTileScale();

        if (x > 15 || y > 15 || x < 0 || y < 0)
        {
            //Hard coded: TODO: let this depend on the centerpoint's height, width and fieldsize (fieldsize doesn't exist yet)
            orxout() << "Can not add Tower: x and y should be between 0 and 15" << endl;
            return;
        }

        orxout() << "Will add tower at (" << (x-8) * tileScale << "," << (y-8) * tileScale << ")" << endl;



        //Create Model
        Model* newtowermodel = new Model(this->center_->getContext());
        newtowermodel->setMeshSource("Tower.mesh");
        newtowermodel->setScale(45);
        newtowermodel->setPosition(static_cast<float>((x-8) * tileScale), static_cast<float>((y-8) * tileScale), 50);



        //Creates tower
        TowerDefenseTower* towernew = new TowerDefenseTower(this->center_->getContext());
        towernew->setPosition(static_cast<float>((x-8) * tileScale), static_cast<float>((y-8) * tileScale), 275);
        towernew->setGame(this);
        towernew->setTeam(1);

        //Reduce credit
         this->buyTower(towerCost);
         towerModelMatrix [x][y]= newtowermodel;
         towerTurretMatrix [x][y]= towernew;
    }

    bool TowerDefense::hasEnoughCreditForTower(int towerCost)
    {
        return ((this->getCredit()) >= towerCost);
    }


    bool TowerDefense::hasEnoughCreditForUpgrade()
    {
        return true;
    }

 
    void TowerDefense::tick(float dt)
    {
        SUPER(TowerDefense, tick, dt);
        time +=dt;

        TDCoordinate* coord1 = new TDCoordinate(1,1);
        std::vector<TDCoordinate*> path;
        path.push_back(coord1);
        if(time>=TowerDefenseEnemyvector.size() && TowerDefenseEnemyvector.size() < 30)
        {
            //adds different types of enemys depending on the WaveNumber
            addTowerDefenseEnemy(path, this->getWaveNumber() % 3 +1 );
        }

        Vector3* endpoint = new Vector3(500, 700, 150);
        //if ships are at the end they get destroyed
        for(unsigned int i =0; i < TowerDefenseEnemyvector.size(); ++i)
        {
            if(TowerDefenseEnemyvector.at(i) != NULL && TowerDefenseEnemyvector.at(i)->isAlive())
            {
                //destroys enemys at the end of the path and reduces the life by 1. no credits gifted

                Vector3 ship = TowerDefenseEnemyvector.at(i)->getRVWorldPosition();
                float distance = ship.distance(*endpoint);

                if(distance <50){
                    TowerDefenseEnemyvector.at(i)->destroy();
                    this->reduceLifes(1);
                    this->buyTower(1);
                    if (this->getLifes()==0)
                    {
                        this->end();
                    }
                }
            }
        }

        //goes thorugh vector to see if an enemy is still alive. if not next wave is launched
        int count= 0;
        for(unsigned int i =0; i < TowerDefenseEnemyvector.size(); ++i)
        {
            if(TowerDefenseEnemyvector.at(i)!= NULL)
            {
                ++count;
            }
        }

        if (count == 0 && !this->nextwaveTimer_.isActive())
            this->nextwaveTimer_.startTimer();

/*            time2 +=dt;
        if(count== 0)
        {
            if(time2 > 10)
            {
                TowerDefenseEnemyvector.clear();
                this->nextwave();
                time=0;
                time2=0;
            }
        }
*/

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
        TeamDeathmatch::playerEntered(player);

        const std::string& message = player->getName() + " entered the game";
        ChatManager::message(message);
    }

    bool TowerDefense::playerLeft(PlayerInfo* player)
    {
        bool valid_player = TeamDeathmatch::playerLeft(player);

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

        TeamDeathmatch::pawnKilled(victim, killer);
    }

    void TowerDefense::playerScored(PlayerInfo* player, int score)
    {
        Gametype::playerScored(player, score);
    }*/
}
