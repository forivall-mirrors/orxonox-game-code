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
#include "worldentities/SpawnPoint.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateUnloadableFactory(TowerDefense);

    TowerDefense::TowerDefense(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(TowerDefense);

    }

    void TowerDefense::start()
    {
        Deathmatch::start();
        orxout()<< "This is a way to display output on the terminal." <<endl;
        //Tipp: Fenster-Modus über die Grafikeinstellungen einstellen.
        //(dazu den Bulletpoint 'Fullscreen' entfernen, eine kleine Auflösung auswählen und auf 'Apply' klicken.)
    }
    /*
    void TowerDefense::end()
    {
        Deathmatch::end();

        std::string message("The match has ended.");
        ChatManager::message(message);
    }
    */


	void TowerDefense::tick(float dt)
    {
        SUPER(TowerDefense, tick, dt);

        static bool test = false;
        if (!test)
			orxout()<< "First tick." <<endl;
            spawnEnemy();
        }
        test = true;
    }

	void TowerDefense::spawnEnemy()
	{
	//	SpawnPoint *sp = *this->spawnpoints_.begin();
		//Pawn* p = sp->spawn();
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
