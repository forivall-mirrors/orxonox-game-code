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

#include "LANDiscovery.h"

#include <enet/enet.h>
#include <cstring>

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"


namespace orxonox
{
  ManageScopedSingleton(LANDiscovery, ScopeID::Graphics, true);

  LANDiscovery::LANDiscovery()
  {
    this->host_ = enet_host_create(NULL, 10, 0, 0, 0 );
    if ( this->host_ == NULL )
        orxout(internal_error, context::network) << "LANDiscovery: host_ == NULL" << endl;
  }

  LANDiscovery::~LANDiscovery()
  {
    if (this->host_ != NULL)
      enet_host_destroy(this->host_);
  }

  void LANDiscovery::discover()
  {
    this->servers_.clear();
    ENetPeer* peer;
    ENetAddress address;
    memset(&address, 0, sizeof(ENetAddress));
    address.port = LAN_DISCOVERY_PORT;

    /* TODO: check for availability of each protocol */
    /* IPv4 */
    address.host = ENET_HOST_BROADCAST;
    peer = enet_host_connect(this->host_, &address, 0, 0);
    if (peer == NULL)
        orxout(internal_error, context::network) << "Could not send LAN discovery to IPv4 Broadcast." << endl;

    /* IPv6 */
    enet_address_set_host(&address, "ff02::1"); // TODO: use a multicast group
    peer = enet_host_connect(this->host_, &address, 0, 0);
    if (peer == NULL)
        orxout(internal_error, context::network) << "Could not send LAN discovery to IPv6 Multicast." << endl;

    ENetEvent event;
    while( enet_host_service(this->host_, &event, 1000 ) )
    {
      switch( event.type )
      {
        case ENET_EVENT_TYPE_CONNECT:
        {
          orxout(verbose, context::network) << "Received LAN discovery connect from server " << event.peer->host->receivedAddress << endl;
          ENetPacket* packet = enet_packet_create(LAN_DISCOVERY_MESSAGE, strlen(LAN_DISCOVERY_MESSAGE)+1, ENET_PACKET_FLAG_RELIABLE);
          enet_peer_send(event.peer, 0, packet);
          break;
        }
        case ENET_EVENT_TYPE_RECEIVE:
          {
            packet::ServerInformation info(&event);
            orxout(internal_info, context::network) << "Received LAN discovery server information; Name: " << info.getServerName() << ", Address: " << info.getServerIP() << ", RTT: " << info.getServerRTT() << endl;
            std::vector<packet::ServerInformation>::iterator it;
            for( it=this->servers_.begin(); it!=this->servers_.end(); ++it )
            {
              if( (*it).getServerIP() == info.getServerIP() )
                break;
            }
            if( it==this->servers_.end() )
              this->servers_.push_back(info);
          }
//           enet_address_get_host_ip(&event.peer->address, buffer, buflen );
//           serverIPs.push_back(std::string(buffer));
          break;
        default:
          break;
      }
    }
  }

  std::string LANDiscovery::getServerListItemName(unsigned int index)
  {
    if( index >= this->servers_.size() )
      return BLANKSTRING;
    else
      return this->servers_[index].getServerName();
  }

  std::string LANDiscovery::getServerListItemIP(unsigned int index)
  {
    if( index >= this->servers_.size() )
      return BLANKSTRING;
    else
      return this->servers_[index].getServerIP();
  }


} // namespace orxonox
