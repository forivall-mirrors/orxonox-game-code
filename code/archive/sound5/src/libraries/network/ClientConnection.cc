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

#include "ClientConnection.h"

#include <cassert>
#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h>
#include "util/Debug.h"

namespace orxonox
{
  const unsigned int NETWORK_CLIENT_WAIT_TIME = 1;
  const unsigned int NETWORK_CLIENT_CONNECTION_TIMEOUT = 3000; //millisecs
  const unsigned int NETWORK_CLIENT_MAX_CONNECTIONS = 1;
  const unsigned int NETWORK_CLIENT_CHANNELS = 1;


  ClientConnection::ClientConnection():
    established_(false),
    server_(NULL)
  {
    this->serverAddress_ = new ENetAddress();
    //set standard address and port
    enet_address_set_host(this->serverAddress_, "127.0.0.1");
    serverAddress_->port = NETWORK_PORT;
  }

  ClientConnection::~ClientConnection(){
    if(this->established_)
      closeConnection();
    delete this->serverAddress_; // surely was created
  }

  void ClientConnection::setServerAddress( const std::string& serverAddress ) {
    enet_address_set_host (this->serverAddress_, serverAddress.c_str());
  }

  void ClientConnection::setPort( unsigned int port ) {
    this->serverAddress_->port = port;
  }

  bool ClientConnection::establishConnection()
  {
    ENetEvent event;

    this->host_ = enet_host_create(NULL, NETWORK_CLIENT_MAX_CONNECTIONS, 0, 0);
    if ( this->host_ == NULL )
    {
      COUT(2) << "ClientConnection: host_ == NULL" << std::endl;
      // error handling
      return false;
    }
    this->server_ = enet_host_connect(this->host_, serverAddress_, NETWORK_CLIENT_CHANNELS);
    if ( this->server_==NULL )
    {
      COUT(2) << "ClientConnection: server == NULL" << std::endl;
      // error handling
      return false;
    }
    // handshake
    for( unsigned int i=0; i<NETWORK_CLIENT_CONNECTION_TIMEOUT/NETWORK_CLIENT_WAIT_TIME; i++ )
    {
      if( enet_host_service(this->host_, &event, NETWORK_CLIENT_WAIT_TIME)>=0 && event.type == ENET_EVENT_TYPE_CONNECT )
      {
        this->established_=true;
        return true;
      }
    }
    COUT(1) << "Could not connect to server" << endl;
    return false;
  }

  bool ClientConnection::closeConnection() {
    ENetEvent event;

    if ( !this->established_ )
      return true;
    this->established_ = false;
    enet_peer_disconnect(this->server_, 0);
    for( unsigned int i=0; i<NETWORK_CLIENT_CONNECTION_TIMEOUT/NETWORK_CLIENT_WAIT_TIME; i++)
    {
      if ( enet_host_service(this->host_, &event, NETWORK_CLIENT_WAIT_TIME) >= 0 )
      {
        switch (event.type)
        {
          case ENET_EVENT_TYPE_NONE:
          case ENET_EVENT_TYPE_CONNECT:
            break;
          case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(event.packet);
            break;
          case ENET_EVENT_TYPE_DISCONNECT:
            COUT(4) << "received disconnect confirmation from server" << endl;
            this->connectionClosed();
            return true;
        }
      }
    }
    enet_peer_reset( this->server_ );
    this->connectionClosed();
    return false;
  }


  bool ClientConnection::addPacket(ENetPacket *packet) {
    assert( this->server_ );
    assert( packet );
    return Connection::addPacket( packet, this->server_ );
  }

  void ClientConnection::addPeer(ENetEvent* event)
  {
    assert(0);
  }
  void ClientConnection::removePeer(ENetEvent* event)
  {
    this->established_=false;
    COUT(1) << "Received disconnect Packet from Server!" << endl;
        // server closed the connection
    this->connectionClosed();
  }

  uint32_t ClientConnection::getRTT()
  {
    assert(server_);
    return server_->roundTripTime;
  }

}
