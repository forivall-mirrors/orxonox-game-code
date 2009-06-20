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

#include "Connection.h"

#include <iostream>
#include <cassert>

#include "util/Debug.h"
#include "util/Math.h"
#include "util/Sleep.h"
#include "ClientInformation.h"
#include "synchronisable/Synchronisable.h"
#include "packet/ClassID.h"

namespace orxonox
{
  Connection *Connection::instance_=0;

  Connection::Connection():
    host_(0)
  {
    assert(instance_==0);
    Connection::instance_=this;
  }

  Connection::~Connection(){
    Connection::instance_=0;
  }

  bool Connection::addPacket(ENetPacket *packet, ENetPeer *peer) {
    if(enet_peer_send(peer, NETWORK_DEFAULT_CHANNEL, packet)!=0)
      return false;
    else
      return true;
  }

  bool Connection::sendPackets() {
    if ( !Connection::instance_ || this->host_==NULL )
      return false;
    enet_host_flush(this->host_);
    return true;
  }

  void Connection::processQueue() {
    ENetEvent event;
    
    assert(this->host_);

    if( enet_host_service( this->host_, &event, NETWORK_WAIT_TIMEOUT ) > 0 )
    {
      switch(event.type){
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
          addClient( &event );
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          disconnectClient( &event );
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          processPacket( &event );
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }

}
