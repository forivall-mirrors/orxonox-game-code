/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
*      Oliver Scheuss, (C) 2007
*   Co-authors:
*      ...
*
*/

//
// Dummy server to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss

#include <iostream>
#include <enet/enet.h>

#include "util/Sleep.h"
#include "ConnectionManager.h"
#include "PacketManager.h"
#include "ClientInformation.h"

using namespace network;

int main(){
  bool quit=false;
  ENetPacket *packet;
  ClientInformation clients;
  ConnectionManager server = ConnectionManager(&clients);
  server.createListener();

  PacketDecoder dec;

  while(!quit){
    if(server.queueEmpty())
      // Warning: usleep(100) is Sleep(100/1000) = Sleep(0), which is nothing!
      usleep(1);
    else{
      ENetAddress addr;
      packet=server.getPacket(addr);
      if(packet==NULL){
        // there was some error
        //std::cout << "null pointer" << std::endl;
        quit=true;
      }
      else{
        //std::cout << "We received: " << packet->data << std::endl;
        dec.elaborate(packet, 1);
      }
    }
  }
  server.quitListener();
  return 0;
}
