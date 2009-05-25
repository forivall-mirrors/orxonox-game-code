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

#include <cassert>

#include "Host.h"
#include "core/ConsoleCommand.h"
#include "packet/Packet.h"
#include "ChatListener.h"

namespace orxonox {

SetConsoleCommandShortcut(Host, Chat);

Host *Host::instance_=0;

/**
 * @brief Constructor: assures that only one reference will be created and sets the pointer
 */
Host::Host()
{
  clientID_=0;
  assert(instance_==0);
  instance_=this;
}


/**
 * @brief Destructor: resets the instance pointer to 0
 */
Host::~Host()
{
  instance_=0;
}

/**
 * This function is used to add an enetpacket to be sent to another peer
 * @param packet Packet to be added
 * @param clientID ID of the client the packet should be sent to
 * @return success?
 */
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

// bool Host::receiveChat(packet::Chat *message, unsigned int clientID){
//   if(instance_)
//     return instance_->processChat(message, clientID);
//   else
//     return false;
// }

/**
 * This function returns the ID of the player
 * @return playerID
 */
unsigned int Host::getPlayerID(){
  if(!instance_)
    return 0;
  return instance_->clientID_;
}

bool Host::Chat(const std::string& message){
  if(!instance_)
    return false;
  return instance_->chat(message);
}

bool Host::Broadcast(const std::string& message){
  if(!instance_)
  {
    for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
      it->incomingChat(message, 0);
    return true;
  }
  else
    return instance_->broadcast(message);
}

bool Host::incomingChat(const std::string& message, unsigned int playerID){
  for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
    it->incomingChat(message, playerID);

  return instance_->processChat(message, playerID);
}

}//namespace orxonox
