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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */
#ifndef NETWORKGAMESTATEHANDLER_H
#define NETWORKGAMESTATEHANDLER_H

#include <string>

#include "NetworkPrereqs.h"
#include "packet/Chat.h"

namespace network {

/**
	@author Oliver Scheuss
*/
class GamestateHandler{
  private:
    virtual bool add(packet::Gamestate *gs, int clientID)=0;
    virtual bool ack(int gamestateID, int clientID)=0;

    static GamestateHandler *instance_;


  protected:
    GamestateHandler();
    virtual ~GamestateHandler();

  public:
    static bool addGamestate(packet::Gamestate *gs, int clientID){ return instance_->add(gs, clientID); }
    static bool ackGamestate(int gamestateID, int clientID){ return instance_->ack(gamestateID, clientID); }
};

}

#endif
