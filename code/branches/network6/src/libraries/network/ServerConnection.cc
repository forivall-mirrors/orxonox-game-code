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

#include "ServerConnection.h"

#include <cassert>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h>

#include "util/Debug.h"
#include <util/Sleep.h>
// #include "ClientInformation.h"

namespace orxonox
{

  ServerConnection::ServerConnection():
    bListening_(false)
  {
    this->bindAddress_ = new ENetAddress();
//     memset(this->bindAddress_, 0, sizeof(ENetAddress));
    this->bindAddress_->host = ENET_HOST_ANY;
    this->bindAddress_->port = NETWORK_PORT;
    this->bindAddress_->scopeID = 0;
  }

  ServerConnection::~ServerConnection()
  {
    if ( this->bListening_ )
      closeListener();
    delete this->bindAddress_;
  }

  void ServerConnection::setBindAddress( const std::string& bindAddress )
  {
    if (enet_address_set_host (this->bindAddress_, bindAddress.c_str()) < 0)
        COUT(1) << "Error: Could not resolve \"" << bindAddress << "\"." << std::endl;
  }

  void ServerConnection::setPort( unsigned int port ) {
      this->bindAddress_->port = port;
  }

  bool ServerConnection::openListener()
  {
    // create host
    this->host_ = enet_host_create(this->bindAddress_, NETWORK_MAX_CONNECTIONS, NETWORK_CHANNEL_COUNT, 0, 0);
    
    if ( this->host_ == NULL )
    {
        COUT(1) << "ServerConnection: host_ == NULL" << std::endl;
        return false;
    }
    
    // enable compression
    this->enableCompression();
    assert( this->host_->socket4 != ENET_SOCKET_NULL || this->host_->socket6 != ENET_SOCKET_NULL );
    if (this->host_->socket4 == ENET_SOCKET_NULL)
        COUT(2) << "Warning: IPv4 Socket failed." << std::endl;
    else if (this->host_->socket6 == ENET_SOCKET_NULL)
        COUT(2) << "Warning: IPv6 Socket failed." << std::endl;
    else
        COUT(3) << "Info: Using IPv4 and IPv6 Sockets." << std::endl;
    
    // start communication thread
    Connection::startCommunicationThread();

    return true;
  }

  bool ServerConnection::closeListener()
  {
    this->bListening_=false;
    disconnectClients();
    Connection::stopCommunicationThread();
    enet_host_destroy(this->host_);
    return true;
  }

  void ServerConnection::addPacket(ENetPacket *packet, unsigned int clientID, uint8_t channelID)
  {
    if ( clientID == NETWORK_PEER_ID_BROADCAST )
    {
      broadcastPacket(packet, channelID);
    }
    else
    {
//       ClientInformation *temp = ClientInformation::findClient(clientID);
//       if(!temp){
//         COUT(3) << "C.Man: addPacket findClient failed" << std::endl;
//       }
      Connection::addPacket(packet, clientID, channelID);
    }
  }

//   void ServerConnection::disconnectClient(ClientInformation *client)
//   {
//     Connection::disconnectPeer( client->getPeer() );
//   }

  void ServerConnection::disconnectClient(int clientID)
  {
//     ClientInformation *client = ClientInformation::findClient(clientID);
//     if(client)
    ServerConnection::disconnectClient(clientID);
  }

  void ServerConnection::disconnectClients()
  {
    Connection::disconnectPeers();
    Connection::waitOutgoingQueue();
    return;
  }


//   int ServerConnection::getClientID(ENetPeer* peer)
//   {
//     return getClientID(&(peer->address));
//   }

//   int ServerConnection::getClientID(ENetAddress* address)
//   {
//     return ClientInformation::findClient(address)->getID();
//   }
// 
//   ENetPeer *ServerConnection::getClientPeer(int clientID)
//   {
//     return ClientInformation::findClient(clientID)->getPeer();
//   }


}
