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

#include "Host.h"

#include <cassert>
#include <string>

#include "core/ConsoleCommand.h"
#include "core/ObjectList.h"
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
  this->printRTTCC_ = createConsoleCommand( createFunctor(&Host::printRTT, this), "printRTT" );
  CommandExecutor::addConsoleCommandShortcut( this->printRTTCC_ );
}


/**
 * @brief Destructor: resets the instance pointer to 0
 */
Host::~Host()
{
  instance_=0;
  if( this->printRTTCC_ )
    delete this->printRTTCC_;
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
  {
    for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
      it->incomingChat(message, 0);
    return true;
  }
  return instance_->chat(message);
}

bool Host::Broadcast(const std::string& message){
  if(!instance_)
  {
    for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
      it->incomingChat(message, CLIENTID_UNKNOWN);
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
