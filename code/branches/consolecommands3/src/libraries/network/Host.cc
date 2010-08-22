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

#include "core/ObjectList.h"
#include "core/command/ConsoleCommand.h"
#include "ChatListener.h"

namespace orxonox {

  SetConsoleCommandShortcut(Host, Chat);

  // Host*               Host::instance_=0;
  uint32_t            Host::clientID_s=0;
  uint32_t            Host::shipID_s=-1;
  std::vector<Host*>  Host::instances_s;

  /**
  * @brief Constructor: assures that only one reference will be created and sets the pointer
  */
  Host::Host()
  {
  //   assert(instance_==0);
    instances_s.push_back(this);
    this->printRTTCC_ = createConsoleCommand( createFunctor(&Host::printRTT, this), "printRTT" );
    CommandExecutor::addConsoleCommandShortcut( this->printRTTCC_ );
    this->bIsActive_ = false;
  }


  /**
  * @brief Destructor: resets the instance pointer to 0
  */
  Host::~Host()
  {
    assert( std::find( instances_s.begin(), instances_s.end(), this )!=instances_s.end() );
    instances_s.erase(std::find( instances_s.begin(), instances_s.end(), this ));
    if( this->printRTTCC_ )
      delete this->printRTTCC_;
  }

  /**
  * This function is used to add an enetpacket to be sent to another peer
  * @param packet Packet to be added
  * @param clientID ID of the client the packet should be sent to
  * @return success?
  */
  bool Host::addPacket(ENetPacket *packet, int clientID)
  {
    bool result = true;
    for( std::vector<Host*>::iterator it = instances_s.begin(); it!=instances_s.end(); ++it )
    {
      if( (*it)->isActive() )
      {
        if( !(*it)->queuePacket(packet, clientID) )
          result = false;
      }
    }
    return result;
  }

  void Host::Chat(const std::string& message)
  {
    if(instances_s.size()==0)
    {
      for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
        it->incomingChat(message, 0);
//      return true;
    }
    else
    {
      bool result = true;
      for( std::vector<Host*>::iterator it = instances_s.begin(); it!=instances_s.end(); ++it )
      {
        if( (*it)->isActive() )
        {
          if( !(*it)->chat(message) )
            result = false;
        }
      }
//      return result;
    }
  }

  bool Host::Broadcast(const std::string& message)
  {
    if(instances_s.size()==0)
    {
      for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
        it->incomingChat(message, CLIENTID_UNKNOWN);
      return true;
    }
    else
    {
      bool result = true;
      for( std::vector<Host*>::iterator it = instances_s.begin(); it!=instances_s.end(); ++it )
      {
        if( (*it)->isActive() )
        {
          if( !(*it)->broadcast(message) )
            result = false;
        }
      }
      return result;
    }
  }

  bool Host::incomingChat(const std::string& message, unsigned int playerID)
  {
    for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
      it->incomingChat(message, playerID);

    bool result = true;
    for( std::vector<Host*>::iterator it = instances_s.begin(); it!=instances_s.end(); ++it )
    {
      if( (*it)->isActive() )
      {
        if( !(*it)->processChat(message, playerID) )
          result = false;
      }
    }
    return result;
  }

  bool Host::isServer()
  {
    for (std::vector<Host*>::iterator it=instances_s.begin(); it!=instances_s.end(); ++it )
    {
      if( (*it)->isServer_() )
        return true;
    }
    return false;
  }

}//namespace orxonox
