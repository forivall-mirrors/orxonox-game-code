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
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

//
// Dummy client to test ConnectionManager, PacketBuffer, ClientConnection and other classes
//
// Author: Oliver Scheuss

#include <iostream>
#include <string>
#include <enet/enet.h>

#include "util/Sleep.h"
#include "PacketManager.h"
#include "PacketTypes.h"
#include "ClientConnection.h"

using namespace network;

int main(){
  network::PacketGenerator pck;

  std::cout << "Enter address of the server xxx.xxx.xxx.xxx (enter for localhost)" << std::endl;
  std::string str;
  std::getline(std::cin, str);
  std::cout << "You entered: " << str << std::endl;
  if(str.compare("")==0)
    str="127.0.0.1";

  ClientConnection client = ClientConnection(55556, str);

  client.createConnection();

  if(client.waitEstablished(10000))
    std::cout << "Connection established" << std::endl;
  else
    std::cout << "Connection failed" << std::endl;

  ENetPacket *packet;  //FIXME: unused var!
  ENetEvent event;



  for(int i=0; i<10; i++){
    // weihnachtsmann bringt packete
    // extend the packet and append the string foo to it
    // send packet to peer on channel id 0
    client.addPacket(pck.chatMessage("test"));
    // keep the timeout very small for low delay
    if(client.sendPackets(&event)){
      std::cout << "successfully sent: " << event.type << std::endl;
    }else{
      std::cout << "failed sending" << std::endl;
    }
    //    usleep(1000000);
  }
  usleep(1000000);
  // now disconnect
  if(client.closeConnection())
    std::cout << "Connection successfully closed" << std::endl;
  else
    std::cout << "Connection closing failed" << std::endl;

  // 3 seconds timeout
  return 0;
}

