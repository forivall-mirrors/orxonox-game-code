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
#include <boost/bind.hpp>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "objects/SpaceShip.h"
#include "util/Math.h"
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
  //boost::thread_group network_threads;
  
  ConnectionManager::ConnectionManager():receiverThread_(0){}
  boost::recursive_mutex ConnectionManager::enet_mutex_;
  
  ConnectionManager::ConnectionManager(ClientInformation *head) : receiverThread_(0) {
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }
  
  ConnectionManager::ConnectionManager(ClientInformation *head, int port){
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = port;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, std::string address, ClientInformation *head) :receiverThread_(0) {
    quit=false;
    enet_address_set_host (& bindAddress, address.c_str());
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, const char *address, ClientInformation *head) : receiverThread_(0) {
    quit=false;
    enet_address_set_host (& bindAddress, address);
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  /*ENetPacket *ConnectionManager::getPacket(ENetAddress &address) {
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
      return NULL;
  }*/
/**
This function only pops the first element in PacketBuffer (first in first out)
used by processQueue in Server.cc
*/
  /*ENetPacket *ConnectionManager::getPacket(int &clientID) {
    ENetAddress address;
    ENetPacket *packet=getPacket(address);
    ClientInformation *temp =head_->findClient(&address);
    if(!temp)
      return NULL;
    clientID=temp->getID();
    return packet;
  }*/
  
  ENetEvent *ConnectionManager::getEvent(){
    if(!buffer.isEmpty())
      return buffer.pop();
    else
      return NULL;
  }

  bool ConnectionManager::queueEmpty() {
    return buffer.isEmpty();
  }

  void ConnectionManager::createListener() {
    receiverThread_ = new boost::thread(boost::bind(&ConnectionManager::receiverThread, this));
    //network_threads.create_thread(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
         //boost::thread thr(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
    return;
  }

  bool ConnectionManager::quitListener() {
    quit=true;
    //network_threads.join_all();
    receiverThread_->join();
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, ENetPeer *peer) {
    ClientInformation *temp = head_->findClient(&(peer->address));
    if(!temp)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    if(enet_peer_send(peer, (enet_uint8)temp->getID() , packet)!=0)
      return false;
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, int clientID) {
    ClientInformation *temp = head_->findClient(clientID);
    if(!temp)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    if(enet_peer_send(temp->getPeer(), (enet_uint8)clientID, packet)!=0)
      return false;
    return true;
  }

  bool ConnectionManager::addPacketAll(ENetPacket *packet) {
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    for(ClientInformation *i=head_->next(); i!=0; i=i->next()){
      if(enet_peer_send(i->getPeer(), (enet_uint8)i->getID(), packet)!=0)
        return false;
    }
    return true;
  }

  bool ConnectionManager::sendPackets() {
    if(server==NULL)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    enet_host_flush(server);
    lock.unlock();
    return true;
  }

  void ConnectionManager::receiverThread() {
    // what about some error-handling here ?
    ENetEvent *event;
    atexit(enet_deinitialize);
    { //scope of the mutex
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_initialize();
      server = enet_host_create(&bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
      lock.unlock();
    }
    if(server==NULL){
      // add some error handling here ==========================
      quit=true;
      return;
    }

    while(!quit){
      event = new ENetEvent;
      { //mutex scope
        boost::recursive_mutex::scoped_lock lock(enet_mutex_);
        if(enet_host_service(server, event, NETWORK_WAIT_TIMEOUT)<0){
          // we should never reach this point
          quit=true;
          continue;
          // add some error handling here ========================
        }
        lock.unlock();
      }
      switch(event->type){
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
          COUT(3) << "adding event_type_connect to queue" << std::endl;
        case ENET_EVENT_TYPE_DISCONNECT:
          //addClient(event);
          //this is a workaround to ensure thread safety
          //COUT(5) << "Con.Man: connection event has occured" << std::endl;
          //break;
        case ENET_EVENT_TYPE_RECEIVE:
          //std::cout << "received data" << std::endl;
          COUT(5) << "Con.Man: receive event has occured" << std::endl;
          // only add, if client has connected yet and not been disconnected
          //if(head_->findClient(&event->peer->address))
            processData(event);
//           else
//             COUT(3) << "received a packet from a client we don't know" << std::endl;
          break;
        //case ENET_EVENT_TYPE_DISCONNECT:
          //clientDisconnect(event->peer);
          //break;
        case ENET_EVENT_TYPE_NONE:
          delete event;
          receiverThread_->yield();
          break;
      }
//       usleep(100);
      //receiverThread_->yield(); //TODO: find apropriate
    }
    disconnectClients();
    // if we're finishied, destroy server
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_host_destroy(server);
      lock.unlock();
    }
  }
  
  //### added some bugfixes here, but we cannot test them because
  //### the server crashes everytime because of some gamestates
  //### (trying to resolve that now)
  void ConnectionManager::disconnectClients() {
    ENetEvent event;
    ClientInformation *temp = head_->next();
    while(temp!=0){
      {
        boost::recursive_mutex::scoped_lock lock(enet_mutex_);
        enet_peer_disconnect(temp->getPeer(), 0);
        lock.unlock();
      }
      temp = temp->next();
    }
    //bugfix: might be the reason why server crashes when clients disconnects
    temp = head_->next();
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    while( temp!=0 && enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT) >= 0){
      switch (event.type)
      {
      case ENET_EVENT_TYPE_NONE: break;
      case ENET_EVENT_TYPE_CONNECT: break;
      case ENET_EVENT_TYPE_RECEIVE:
        enet_packet_destroy(event.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        COUT(4) << "disconnecting all clients" << std::endl;
        if(head_->findClient(&(event.peer->address)))
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

/**
This function adds a client that connects to the clientlist of the server
NOTE: if you change this, don't forget to change the test function
addClientTest in diffTest.cc since addClient is not good for testing because of syncClassid
*/
  /*bool ConnectionManager::addClient(ENetEvent *event) {
    ClientInformation *temp = head_->insertBack(new ClientInformation);
    if(!temp){
      COUT(2) << "Conn.Man. could not add client" << std::endl;
      return false;
    }
    if(temp->prev()->getHead()) { //not good if you use anything else than insertBack
      temp->prev()->setID(0); //bugfix: not necessary but usefull
      temp->setID(1);
    }
    else
      temp->setID(temp->prev()->getID()+1);
    temp->setPeer(event->peer);
    COUT(3) << "Con.Man: added client id: " << temp->getID() << std::endl;
    return true;
  }*/

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
    unsigned int network_id=0, failures=0;
    std::string classname;
    orxonox::Identifier *id;
    std::map<std::string, orxonox::Identifier*>::const_iterator it = orxonox::Factory::getFactoryBegin();
    while(it != orxonox::Factory::getFactoryEnd()){
      id = (*it).second;
      if(id == NULL)
        continue;
      classname = id->getName();
      network_id = id->getNetworkID();
      if(network_id==0)
        COUT(3) << "we got a null class id: " << id->getName() << std::endl;
      COUT(4) << "Con.Man:syncClassid:\tnetwork_id: " << network_id << ", classname: " << classname << std::endl;

      while(!addPacket(packet_gen.clid( (int)network_id, classname ), clientID) && failures < 10){
        failures++;
      }
      ++it;
    }
    sendPackets();
    COUT(4) << "syncClassid:\tall synchClassID packets have been sent" << std::endl;
  }

  /*bool ConnectionManager::createClient(int clientID){
    ClientInformation *temp = head_->findClient(clientID);
    if(!temp){
      COUT(2) << "Conn.Man. could not create client with id: " << clientID << std::endl;
      return false;
    }
    COUT(4) << "Con.Man: creating client id: " << temp->getID() << std::endl;
    syncClassid(temp->getID());
    COUT(4) << "creating spaceship for clientid: " << temp->getID() << std::endl;
    // TODO: this is only a hack, until we have a possibility to define default player-join actions
    if(!createShip(temp))
      COUT(2) << "Con.Man. could not create ship for clientid: " << clientID << std::endl;
    else
      COUT(3) << "created spaceship" << std::endl;
    temp->setSynched(true);
    COUT(3) << "sending welcome" << std::endl;
    sendWelcome(temp->getID(), temp->getShipID(), true);
    return true;
  }*/
  
  /*bool ConnectionManager::removeClient(int clientID){
    boost::recursive_mutex::scoped_lock lock(head_->mutex_);
    orxonox::Iterator<orxonox::SpaceShip> it = orxonox::ObjectList<orxonox::SpaceShip>::start();
    ClientInformation *client = head_->findClient(clientID);
    if(!client)
      return false;
    while(it){
      if(it->objectID!=client->getShipID()){
        ++it;
        continue;
      }
      orxonox::Iterator<orxonox::SpaceShip> temp=it;
      ++it;
      delete  *temp;
      return head_->removeClient(clientID);
    }
    return false;
  }*/
  
/*  bool ConnectionManager::createShip(ClientInformation *client){
    if(!client)
      return false;
    orxonox::Identifier* id = ID("SpaceShip");
    if(!id){
      COUT(4) << "We could not create the SpaceShip for client: " << client->getID() << std::endl;
      return false;
    }
    orxonox::SpaceShip *no = dynamic_cast<orxonox::SpaceShip *>(id->fabricate());
    no->setPosition(orxonox::Vector3(0,0,80));
    no->setScale(10);
    //no->setYawPitchRoll(orxonox::Degree(-90),orxonox::Degree(-90),orxonox::Degree(0));
    no->setMesh("assff.mesh");
    no->setMaxSpeed(500);
    no->setMaxSideAndBackSpeed(50);
    no->setMaxRotation(1.0);
    no->setTransAcc(200);
    no->setRotAcc(3.0);
    no->setTransDamp(75);
    no->setRotDamp(1.0);
    no->setCamera("cam_"+client->getID());
    no->classID = id->getNetworkID();
    no->create();
    
    client->setShipID(no->objectID);
    return true;
  }*/
  
  bool ConnectionManager::removeShip(ClientInformation *client){
    int id=client->getShipID();
    orxonox::Iterator<orxonox::SpaceShip> it;
    for(it = orxonox::ObjectList<orxonox::SpaceShip>::start(); it; ++it){
      if(it->objectID!=id)
        continue;
      delete *it;
    }
    return true;
  }
  
  bool ConnectionManager::sendWelcome(int clientID, int shipID, bool allowed){
    if(addPacket(packet_gen.generateWelcome(clientID, shipID, allowed),clientID)){
      sendPackets();
      return true;
    }else
      return false;
  }
  
  void ConnectionManager::disconnectClient(ClientInformation *client){
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_peer_disconnect(client->getPeer(), 0);
      lock.unlock();
    }
    removeShip(client);
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
