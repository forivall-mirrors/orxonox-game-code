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

#include "LANDiscoverable.h"

#include <enet/enet.h>
#include <cassert>
#include <cstring>

#include "util/Output.h"
#include "packet/ServerInformation.h"

namespace orxonox
{
  const char* LAN_DISCOVERY_MESSAGE = "Orxonox Client";
  const char* LAN_DISCOVERY_ACK     = "Orxonox Server Ack";

  LANDiscoverable::LANDiscoverable()
  {
    this->host_ = 0;
    this->bActive_ = false;
//     this->setActivity(true);
  }

  LANDiscoverable::~LANDiscoverable()
  {
    if( this->host_ )
    {
      enet_host_destroy( this->host_ );
      assert( this->bActive_ );
    }
  }

  void LANDiscoverable::setActivity(bool bActive)
  {
    if( bActive == this->bActive_ )        // no change
      return;
    
    if( bActive )
    {
      ENetAddress bindAddress;
      memset(& bindAddress, 0, sizeof(ENetAddress));
      bindAddress.host = ENET_HOST_ANY;
      bindAddress.port = LAN_DISCOVERY_PORT;
      assert( this->host_ == 0 );
      this->host_ = enet_host_create( &bindAddress, 10, 0, 0, 0 );
      if ( this->host_ == NULL )
          orxout(internal_error, context::network) << "LANDiscoverable: host_ == NULL" << endl;
    }
    else
    {
      enet_host_destroy( this->host_ );
      this->host_ = 0;
    }
      this->bActive_ = bActive;
  }

  void LANDiscoverable::update()
  {
    ENetEvent event;
    
    if( this->bActive_==false )
      return;
    assert(this->host_);
    
    while( enet_host_service( this->host_, &event, 0 ) > 0 )
    {
      switch(event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
            orxout(verbose, context::network) << "Received LAN discovery connect from client " << event.peer->host->receivedAddress << endl;
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
        case ENET_EVENT_TYPE_NONE:
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          if( strcmp( LAN_DISCOVERY_MESSAGE, (char*)event.packet->data ) == 0 )      // check for a suitable orxonox client
          {
            orxout(internal_info, context::network) << "Received LAN discovery message from client " << event.peer->host->receivedAddress << endl;
            packet::ServerInformation info;
            info.setServerName("Orxonox Server");
            info.send(event.peer);
//             ENetPacket* packet = enet_packet_create( LAN_DISCOVERY_ACK, strlen(LAN_DISCOVERY_ACK)+1, ENET_PACKET_FLAG_RELIABLE );
//             enet_peer_send(event.peer, 0, packet );
            enet_host_flush(this->host_);
          }
          break;
      }
    }
  }

}
