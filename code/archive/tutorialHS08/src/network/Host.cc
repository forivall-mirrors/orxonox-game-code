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

#include <assert.h>

#include "Host.h"
#include "packet/Packet.h"

namespace network {

Host *Host::instance_=0;
  
Host::Host()
{
  assert(instance_==0);
  instance_=this;
}


Host::~Host()
{
  instance_=0;
}

bool Host::addPacket(ENetPacket *packet, int clientID){
  if(instance_)
    return instance_->queuePacket(packet, clientID);
  else
    return false;
}


// bool Host::chat(std::string& message){
//   if(!instance_)
//     return false;
//   packet::Chat *c = new packet::Chat(message, getPlayerID());
//   return instance_->sendChat(c);
// }

// bool Host::receiveChat(network::packet::Chat *message, unsigned int clientID){
//   if(instance_)
//     return instance_->processChat(message, clientID);
//   else
//     return false;
// }

int Host::getPlayerID(){ 
  if(!instance_)
    return 0;
  return instance_->playerID();
}

// unsigned int Host::getShipID(){ 
//   if(!instance_)
//     assert(0);
//   return instance_->shipID();
// }

}//namespace network
