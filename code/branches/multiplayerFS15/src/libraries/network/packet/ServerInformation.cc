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

#include "ServerInformation.h"

#include <enet/enet.h>
#include <cassert>
#include <cstring>

#include "util/Serialise.h"

namespace orxonox
{
  namespace packet
  {

    ServerInformation::ServerInformation()
    {
    }

    ServerInformation::ServerInformation(ENetEvent* event)
    {
      char serverIP[64];

      // Save Server Round Trip Time
      this->serverRTT_ = event->peer->roundTripTime;
      // Save Server Address, leave some space for scope ID
      enet_address_get_host_ip(&event->peer->address, serverIP, 64);
      this->serverIP_ = std::string(serverIP);
      // Save ACK
      uint8_t* temp = event->packet->data;
      char* ack = new char[strlen(LAN_DISCOVERY_ACK)+1];
      loadAndIncrease((char*&)ack, temp);

      /* Fabian, what is this used for? it crashes the masterserver, hence commenting it */
      // written by Oli: this is just to make sure that loadAndIncrease really writes the whole ACK string into char* ack
//       assert(strcmp(ack, (const char*)LAN_DISCOVERY_ACK)==0);

      // Save Server Name
      loadAndIncrease(this->serverName_, temp);
    }

    ServerInformation::~ServerInformation()
    {

    }

    void ServerInformation::send(ENetPeer* peer)
    {
      std::string payload = this->serverName_ + Ogre::StringConverter::toString(this->clientNumber_);
      uint32_t size = returnSize((char*&)LAN_DISCOVERY_ACK) + returnSize(payload);
      uint8_t* temp = new uint8_t[size];
      uint8_t* temp2 = temp;
      saveAndIncrease((char*&)LAN_DISCOVERY_ACK, temp2);
      saveAndIncrease(payload, temp2);
      ENetPacket* packet = enet_packet_create( temp, size, 0 );
      enet_peer_send(peer, 0, packet);

      delete[] temp;
    }

  } // namespace packet

  std::ostream& operator<<(std::ostream& out, const ENetAddress& address)
  {
      char addr[64];
      if (!enet_address_get_host_ip(&address, addr, 64))
          out << addr;
      return out;
  }
} // namespace orxonox
