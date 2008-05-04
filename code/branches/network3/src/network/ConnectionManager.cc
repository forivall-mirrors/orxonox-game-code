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
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

//
// C++ Interface: ConnectionManager
//
// Description: The Class ConnectionManager manages the servers conenctions to the clients.
// each connection is provided by a new process. communication between master process and
// connection processes is provided by ...
//
//
// Author:  Oliver Scheuss
//

#include <iostream>
// boost.thread library for multithreading support
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "util/Math.h"
#include "objects/SpaceShip.h"
#include "ClientInformation.h"
#include "ConnectionManager.h"
#include "Synchronisable.h"

namespace std
{
  bool operator< (ENetAddress a, ENetAddress b) {
    if(a.host <= b.host)
      return true;
    else
      return false;
  }
}

namespace network
{
  boost::thread_group network_threads;
  
  ConnectionManager::ConnectionManager(){}
  
  ConnectionManager::ConnectionManager(ClientInformation *head) {
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, std::string address, ClientInformation *head) {
    quit=false;
    enet_address_set_host (& bindAddress, address.c_str());
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, const char *address, ClientInformation *head) {
    quit=false;
    enet_address_set_host (& bindAddress, address);
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ENetPacket *ConnectionManager::getPacket(ENetAddress &address) {
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
      return NULL;
  }
/**
This function only pops the first element in PacketBuffer (first in first out)
used by processQueue in Server.cc
*/
  ENetPacket *ConnectionManager::getPacket(int &clientID) {
    ENetAddress address;
    ENetPacket *packet=getPacket(address);
    ClientInformation *temp =head_->findClient(&address);
    clientID=temp->getID();
    return packet;
  }

  bool ConnectionManager::queueEmpty() {
    return buffer.isEmpty();
  }

  void ConnectionManager::createListener() {
    network_threads.create_thread(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
    //     boost::thread thr(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
    return;
  }

  bool ConnectionManager::quitListener() {
    quit=true;
    network_threads.join_all();
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, ENetPeer *peer) {
    if(enet_peer_send(peer, (enet_uint8)head_->findClient(&(peer->address))->getID() , packet)!=0)
      return false;
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, int clientID) {
    if(enet_peer_send(head_->findClient(clientID)->getPeer(), (enet_uint8)clientID, packet)!=0)
      return false;
    return true;
  }

  bool ConnectionManager::addPacketAll(ENetPacket *packet) {
    for(ClientInformation *i=head_->next(); i!=0; i=i->next()){
      if(enet_peer_send(i->getPeer(), (enet_uint8)i->getID(), packet)!=0)
        return false;
    }
    return true;
  }

  bool ConnectionManager::sendPackets(ENetEvent *event) {
    if(server==NULL)
      return false;
    if(enet_host_service(server, event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }

  bool ConnectionManager::sendPackets() {
    ENetEvent event;
    if(server==NULL)
      return false;
    if(enet_host_service(server, &event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }

  void ConnectionManager::receiverThread() {
    // what about some error-handling here ?
    enet_initialize();
    atexit(enet_deinitialize);
    ENetEvent event;
    server = enet_host_create(&bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
    if(server==NULL){
      // add some error handling here ==========================
      quit=true;
      return;
    }

    while(!quit){
      if(enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT)<0){
        // we should never reach this point
        quit=true;
        // add some error handling here ========================
      }
      switch(event.type){
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
          addClient(&event);
          //this is a workaround to ensure thread safety
          if(!addFakeConnectRequest(&event))
            COUT(3) << "Problem pushing fakeconnectRequest to queue" << std::endl;
          COUT(5) << "Con.Man: connection event has occured" << std::endl;
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          //std::cout << "received data" << std::endl;
          COUT(5) << "Con.Man: receive event has occured" << std::endl;
          processData(&event);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          // add some error/log handling here
          clientDisconnect(event.peer);
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
    disconnectClients();
    // if we're finishied, destroy server
    enet_host_destroy(server);
  }
  
  //### added some bugfixes here, but we cannot test them because
  //### the server crashes everytime because of some gamestates
  //### (trying to resolve that now)
  void ConnectionManager::disconnectClients() {
    ENetEvent event;
    ClientInformation *temp = head_->next();
    while(temp!=0){
      enet_peer_disconnect(temp->getPeer(), 0);
      temp = temp->next();
    }
    //bugfix: might be the reason why server crashes when clients disconnects
    //temp = temp->next();
    temp = head_->next();
    while( temp!=0 && enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT) > 0){
      switch (event.type)
      {
      case ENET_EVENT_TYPE_NONE: break;
      case ENET_EVENT_TYPE_CONNECT: break;
      case ENET_EVENT_TYPE_RECEIVE:
        enet_packet_destroy(event.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        COUT(4) << "disconnecting all clients" << std::endl;
        delete head_->findClient(&(event.peer->address));
        //maybe needs bugfix: might also be a reason for the server to crash
        temp = temp->next();
        break;
      }
    }
    return;
  }

  bool ConnectionManager::processData(ENetEvent *event) {
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }

  bool ConnectionManager::clientDisconnect(ENetPeer *peer) {
    COUT(4) << "removing client from list" << std::endl;
    return head_->removeClient(peer);
  }
/**
This function adds a client that connects to the clientlist of the server
NOTE: if you change this, don't forget to change the test function
addClientTest in diffTest.cc since addClient is not good for testing because of syncClassid
*/
  bool ConnectionManager::addClient(ENetEvent *event) {
    ClientInformation *temp = head_->insertBack(new ClientInformation);
    if(temp->prev()->head) { //not good if you use anything else than insertBack
      temp->prev()->setID(0); //bugfix: not necessary but usefull
      temp->setID(1);
    }
    else
      temp->setID(temp->prev()->getID()+1);
    temp->setPeer(event->peer);
    COUT(4) << "Con.Man: added client id: " << temp->getID() << std::endl;
    return true;
  }

  int ConnectionManager::getClientID(ENetPeer peer) {
    return getClientID(peer.address);
  }

  int ConnectionManager::getClientID(ENetAddress address) {
    return head_->findClient(&address)->getID();
  }

  ENetPeer *ConnectionManager::getClientPeer(int clientID) {
    return head_->findClient(clientID)->getPeer();
  }

  void ConnectionManager::syncClassid(int clientID) {
    unsigned int network_id=0;
    std::string classname;
    orxonox::Identifier *id;
    std::map<std::string, orxonox::Identifier*>::const_iterator it = orxonox::Factory::getFactoryBegin();
    while(it != orxonox::Factory::getFactoryEnd()){
      id = (*it).second;
      if(id == NULL)
        continue;
      classname = id->getName();
      network_id = id->getNetworkID();
      COUT(4) << "Con.Man:syncClassid:\tnetwork_id: " << network_id << ", classname: " << classname << std::endl;

      addPacket(packet_gen.clid( (int)network_id, classname ), clientID);

      ++it;
    }
    sendPackets();
    COUT(4) << "syncClassid:\tall synchClassID packets have been sent" << std::endl;
  }

  bool ConnectionManager::createClient(int clientID){
    ClientInformation *temp = head_->findClient(clientID);
    COUT(4) << "Con.Man: creating client id: " << temp->getID() << std::endl;
    syncClassid(temp->getID());
    COUT(4) << "creating spaceship for clientid: " << temp->getID() << std::endl;
    // TODO: this is only a hack, untill we have a possibility to define default player-join actions
    createShip(temp);
    COUT(4) << "created spaceship" << std::endl;
    temp->setSynched(true);
    COUT(4) << "sending welcome" << std::endl;
    sendWelcome(temp->getID(), temp->getShipID(), true);
    return true;
  }
  
  bool ConnectionManager::createShip(ClientInformation *client){
    orxonox::Identifier* id = ID("SpaceShip");
    if(!id){
      COUT(4) << "We could not create the SpaceShip for client: " << client->getID() << std::endl;
      return false;
    }
    orxonox::SpaceShip *no = dynamic_cast<orxonox::SpaceShip *>(id->fabricate());
    no->setPosition(orxonox::Vector3(0,80,0));
    no->setScale(10);
    no->setYawPitchRoll(orxonox::Degree(-90),orxonox::Degree(-90),orxonox::Degree(0));
    no->setMesh("assf3.mesh");
    no->setMaxSpeed(500);
    no->setMaxSideAndBackSpeed(50);
    no->setMaxRotation(1.0);
    no->setTransAcc(200);
    no->setRotAcc(3.0);
    no->setTransDamp(75);
    no->setRotDamp(1.0);
    no->setCamera("cam_"+client->getID());
    no->create();
    
    client->setShipID(no->objectID);
    return true;
  }
  
  bool ConnectionManager::sendWelcome(int clientID, int shipID, bool allowed){
    addPacket(packet_gen.generateWelcome(clientID, shipID, allowed),clientID);
    sendPackets();
  }
  
  bool ConnectionManager::addFakeConnectRequest(ENetEvent *ev){
    ENetEvent event;
    event.peer=ev->peer;
    event.packet = packet_gen.generateConnectRequest();
    return buffer.push(&event);
  }
  
  
//   int ConnectionManager::getNumberOfClients() {
//     
//     return clientsShip.size();
//   }
  
  /*void ConnectionManager::addClientsObjectID( int clientID, int objectID ) {
  COUT(4) << "ship of client: " << clientID << ": " << objectID << " mapped" << std::endl;
  clientsShip.insert( std::make_pair( clientID, objectID ) );
}

  int ConnectionManager::getClientsShipID( int clientID ) {
  return clientsShip[clientID];
}

  int ConnectionManager::getObjectsClientID( int objectID ) {
  std::map<int, int>::iterator iter;
  for( iter = clientsShip.begin(); iter != clientsShip.end(); iter++ ) {
  if( iter->second == objectID ) return iter->first;
}
  return -99;
}

  void ConnectionManager::deleteClientIDReg( int clientID ) {
  clientsShip.erase( clientID );
}

  void ConnectionManager::deleteObjectIDReg( int objectID ) {
  std::map<int, int>::iterator iter = clientsShip.begin();
  for( iter = clientsShip.begin(); iter != clientsShip.end(); iter++ ) {
  if( iter->second == objectID ) break;
}
  clientsShip.erase( iter->first );
}*/
}
