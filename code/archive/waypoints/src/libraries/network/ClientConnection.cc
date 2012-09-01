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
#include "util/Output.h"

namespace orxonox
{
  const unsigned int NETWORK_CLIENT_WAIT_TIME = 1;
  const unsigned int NETWORK_CLIENT_CONNECTION_TIMEOUT = 3000; //millisecs
  const unsigned int NETWORK_CLIENT_MAX_CONNECTIONS = 1;


  ClientConnection::ClientConnection():
    Connection(NETWORK_PEER_ID_SERVER),
    established_(false),
    server_(NULL)
  {
    this->serverAddress_ = new ENetAddress();
    //set standard address and port
    enet_address_set_host(this->serverAddress_, "127.0.0.1"); // TODO: check for IPv6 and connect to ::1 instead
    serverAddress_->port = NETWORK_PORT;
  }

  ClientConnection::~ClientConnection(){
    if(this->established_)
      closeConnection();
    delete this->serverAddress_; // surely was created
  }

  void ClientConnection::setServerAddress( const std::string& serverAddress ) {
    if (enet_address_set_host (this->serverAddress_, serverAddress.c_str()) < 0)
        orxout(internal_error, context::network) << "Could not resolve \"" << serverAddress << "\"." << endl;
  }

  void ClientConnection::setPort( unsigned int port ) {
    this->serverAddress_->port = port;
  }

  bool ClientConnection::establishConnection()
  {
    ENetEvent event;

    // create host
    this->host_ = enet_host_create(NULL, NETWORK_CLIENT_MAX_CONNECTIONS, NETWORK_CHANNEL_COUNT, 0, 0);
    
    if ( this->host_ == NULL )
    {
      orxout(internal_error, context::network) << "ClientConnection: host_ == NULL" << endl;
      // error handling
      return false;
    }
    
    // enable compression
    this->enableCompression();
    
    assert( this->host_->socket4 != ENET_SOCKET_NULL || this->host_->socket6 != ENET_SOCKET_NULL );
    if (this->host_->socket4 == ENET_SOCKET_NULL)
        orxout(internal_warning, context::network) << "IPv4 Socket failed." << endl;
    else if (this->host_->socket6 == ENET_SOCKET_NULL)
        orxout(internal_warning, context::network) << "IPv6 Socket failed." << endl;
    else
        orxout(internal_info, context::network) << "Using IPv4 and IPv6 Sockets." << endl;

    this->server_ = enet_host_connect(this->host_, serverAddress_, NETWORK_CHANNEL_COUNT, 0);
    if ( this->server_==NULL )
    {
      orxout(internal_error, context::network) << "ClientConnection: server_ == NULL" << endl;
      // error handling
      return false;
    }
    // handshake
    for( unsigned int i=0; i<NETWORK_CLIENT_CONNECTION_TIMEOUT/NETWORK_CLIENT_WAIT_TIME; i++ )
    {
      if( enet_host_service(this->host_, &event, NETWORK_CLIENT_WAIT_TIME)>=0 && event.type == ENET_EVENT_TYPE_CONNECT )
      {
        // manually add server to list of peers
        /*incomingEvent inEvent = */Connection::preprocessConnectEvent(event);
//         addPeer(inEvent.peerID);
        // start communication thread
        this->established_=true;
        Connection::startCommunicationThread();
        return true;
      }
    }
    orxout(user_error, context::network) << "Could not connect to server" << endl;
    return false;
  }

  bool ClientConnection::closeConnection() {
    ENetEvent event;

    if ( !this->established_ )
      return true;
    this->established_ = false;
    
    // stop communication thread and disconnect server
    Connection::stopCommunicationThread();
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
            orxout(verbose, context::network) << "received disconnect confirmation from server" << endl;
            this->connectionClosed();
            return true;
        }
      }
    }
    enet_peer_reset( this->server_ );
    this->connectionClosed();
    return false;
  }


  void ClientConnection::addPacket(ENetPacket *packet, uint8_t channelID) {
    assert( this->server_ );
    assert( packet );
//     return Connection::addPacket( packet, NETWORK_PEER_ID_SERVER, channelID );
    // HACK: actually there should be a way to do this using addPacket and the correct peerID
    return Connection::broadcastPacket(packet, channelID);
  }

  void ClientConnection::addPeer(uint32_t peerID)
  {
    assert(0);
  }
  void ClientConnection::removePeer(uint32_t peerID)
  {
    this->established_=false;
    orxout(internal_error, context::network) << "Received disconnect Packet from Server!" << endl;
        // server closed the connection
    this->stopCommunicationThread();
    this->connectionClosed();
  }

  uint32_t ClientConnection::getRTT()
  {
    if (server_)
        return server_->roundTripTime;
    else
        return 0;
  }

}
