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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "ClientConnectionListener.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"

namespace orxonox{

  ClientConnectionListener::ClientConnectionListener()
  { 
    RegisterRootObject(ClientConnectionListener); 
  }

  void ClientConnectionListener::getConnectedClients(){
    if(GameMode::showsGraphics())
      this->clientConnected(0); //server client id
    ClientInformation *client = ClientInformation::getBegin();
    while(client){
      this->clientConnected(client->getID());
      client=client->next();
    }
  }

}


