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

#include "core/CoreIncludes.h"
#include "core/object/ObjectList.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "NetworkChatListener.h"

namespace orxonox {

  static const std::string __CC_printRTT_group = "Stats";
  static const std::string __CC_printRTT_name = "printRTT";

  SetConsoleCommand(__CC_printRTT_group, __CC_printRTT_name, &Host::printRTT);

  // Host*               Host::instance_=0;
  uint32_t            Host::clientID_s=0;
//   uint32_t            Host::shipID_s=-1;
  std::vector<Host*>  Host::instances_s;

  /**
  * @brief Constructor: assures that only one reference will be created and sets the pointer
  */
  Host::Host()
  {
  //   assert(instance_==0);
    instances_s.push_back(this);
    ModifyConsoleCommand(__CC_printRTT_group, __CC_printRTT_name).setObject(this);
    this->bIsActive_ = false;
  }


  /**
  * @brief Destructor: resets the instance pointer to 0
  */
  Host::~Host()
  {
    assert( std::find( instances_s.begin(), instances_s.end(), this )!=instances_s.end() );
    instances_s.erase(std::find( instances_s.begin(), instances_s.end(), this ));
    ModifyConsoleCommand(__CC_printRTT_group, __CC_printRTT_name).setObject(0);
  }

  /**
  * This function is used to add an enetpacket to be sent to another peer
  * @param packet Packet to be added
  * @param clientID ID of the client the packet should be sent to
  * @param channelID ID of the channel.
  * @return success?
  */
  void Host::addPacket(ENetPacket *packet, int clientID, uint8_t channelID)
  {
    for( std::vector<Host*>::iterator it = instances_s.begin(); it!=instances_s.end(); ++it )
    {
      if( (*it)->isActive() )
      {
        (*it)->queuePacket(packet, clientID, channelID);
      }
    }
  }

  /**
   * @brief Sends a chat message through the network.
   * @param message message to be sent
   * @param sourceID the ID of the sender
   * @param targetID the ID of the receiver
   */
  void Host::sendChat(const std::string& message, unsigned int sourceID, unsigned int targetID)
  {
    for( std::vector<Host*>::iterator it = instances_s.begin(); it!=instances_s.end(); ++it )
      if( (*it)->isActive() )
        (*it)->doSendChat(message, sourceID, targetID);
  }

  /**
   * @brief Gets called if a packet::Chat packet is received. Passes the message to the listeners.
   */
  void Host::doReceiveChat(const std::string& message, unsigned int sourceID, unsigned int targetID)
  {
    for (ObjectList<NetworkChatListener>::iterator it = ObjectList<NetworkChatListener>::begin(); it != ObjectList<NetworkChatListener>::end(); ++it)
      it->incomingChat(message, sourceID);
  }


  bool Host::isServer()
  {
    for (std::vector<Host*>::iterator it=instances_s.begin(); it!=instances_s.end(); ++it )
    {
      if( (*it)->isActive() )
      {
        if( (*it)->isServer_() )
          return true;
      }
    }
    return false;
  }

  Host* Host::getActiveInstance()
  {
    std::vector<Host*>::iterator it = Host::instances_s.begin();
    while( it != Host::instances_s.end() )
    {
      if( (*it)->isActive() )
        return *it;
      else
        ++it;
    }
    return 0;
  }


  //////////////////////////////////////////////////////////////////////////
  // NetworkChatListener                                                  //
  //////////////////////////////////////////////////////////////////////////

  RegisterAbstractClass(NetworkChatListener).inheritsFrom<Listable>();

  NetworkChatListener::NetworkChatListener()
  {
      RegisterObject(NetworkChatListener);
  }

}//namespace orxonox
