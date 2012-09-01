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
#include "ClientInformation.h"

namespace orxonox
{

  ServerConnection::ServerConnection():
    bListening_(false)
  {
    this->bindAddress_ = new ENetAddress();
    this->bindAddress_->host = ENET_HOST_ANY;
    this->bindAddress_->port = NETWORK_PORT;
  }

  ServerConnection::~ServerConnection(){
    if ( this->bListening_ )
      closeListener();
    delete this->bindAddress_;
  }

  void ServerConnection::setBindAddress( const std::string& bindAddress ) {
    enet_address_set_host (this->bindAddress_, bindAddress.c_str());
  }

  void ServerConnection::setPort( unsigned int port ) {
      this->bindAddress_->port = port;
  }

  bool ServerConnection::openListener() {
    this->host_ = enet_host_create(this->bindAddress_, NETWORK_MAX_CONNECTIONS, 0, 0);
    if ( this->host_ == NULL )
      return false;
    else
      return true;
  }

  bool ServerConnection::closeListener() {
    this->bListening_=false;
    disconnectClients();
    enet_host_destroy(this->host_);
    return true;
  }

  bool ServerConnection::addPacket(ENetPacket *packet, unsigned int clientID) {
    if ( clientID == CLIENTID_UNKNOWN )
    {
      return addPacketAll(packet);
    }
    else
    {
      ClientInformation *temp = ClientInformation::findClient(clientID);
      if(!temp){
        COUT(3) << "C.Man: addPacket findClient failed" << std::endl;
        return false;
      }
      return Connection::addPacket(packet, temp->getPeer());
    }
  }

  bool ServerConnection::addPacketAll(ENetPacket *packet) {
    if ( !Connection::getInstance() )
      return false;
    enet_host_broadcast( Connection::getInstance()->getHost(), 0, packet);
    return true;
  }

  void ServerConnection::disconnectClient(ClientInformation *client)
  {
    Connection::disconnectPeer( client->getPeer() );
  }

  void ServerConnection::disconnectClient(int clientID){
    ClientInformation *client = ClientInformation::findClient(clientID);
    if(client)
      ServerConnection::disconnectClient(client);
  }

  void ServerConnection::disconnectClients() {
    ENetEvent event;
    ClientInformation *temp = ClientInformation::getBegin();
    while(temp!=0){
      ServerConnection::disconnectClient( temp );
      temp = temp->next();
    }
    temp = ClientInformation::getBegin();
    while( temp!=0 ){
      if( service( &event ) )
      {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_NONE: break;
        case ENET_EVENT_TYPE_CONNECT: break;
        case ENET_EVENT_TYPE_RECEIVE:
          enet_packet_destroy(event.packet);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          removePeer( &event );
          temp = ClientInformation::getBegin();
          break;
        }
      }
    }
    return;
  }


  int ServerConnection::getClientID(ENetPeer* peer) {
    return getClientID(&(peer->address));
  }

  int ServerConnection::getClientID(ENetAddress* address) {
    return ClientInformation::findClient(address)->getID();
  }

  ENetPeer *ServerConnection::getClientPeer(int clientID) {
    return ClientInformation::findClient(clientID)->getPeer();
  }


}
