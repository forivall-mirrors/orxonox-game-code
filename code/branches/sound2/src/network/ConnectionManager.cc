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

#include "ConnectionManager.h"

#include <enet/enet.h>
#include <iostream>
#include <cassert>
// boost.thread library for multithreading support
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "util/Debug.h"
#include "util/Math.h"
#include "util/Sleep.h"
#include "ClientInformation.h"
#include "synchronisable/Synchronisable.h"
#include "packet/ClassID.h"

namespace std
{
  bool operator< (ENetAddress a, ENetAddress b) {
    if(a.host <= b.host)
      return true;
    else
      return false;
  }
}

namespace orxonox
{
  //boost::thread_group network_threads;
  static boost::recursive_mutex enet_mutex_g;

  ConnectionManager *ConnectionManager::instance_=0;

  ConnectionManager::ConnectionManager():receiverThread_(0){
    assert(instance_==0);
    instance_=this;
    quit=false;
    bindAddress = new ENetAddress();
    bindAddress->host = ENET_HOST_ANY;
    bindAddress->port = NETWORK_PORT;
  }

  ConnectionManager::ConnectionManager(int port){
    assert(instance_==0);
    instance_=this;
    quit=false;
    bindAddress = new ENetAddress();
    bindAddress->host = ENET_HOST_ANY;
    bindAddress->port = port;
  }

  ConnectionManager::ConnectionManager(int port, const std::string& address) :receiverThread_(0) {
    assert(instance_==0);
    instance_=this;
    quit=false;
    bindAddress = new ENetAddress();
    enet_address_set_host (bindAddress, address.c_str());
    bindAddress->port = NETWORK_PORT;
  }

  ConnectionManager::ConnectionManager(int port, const char *address) : receiverThread_(0) {
    assert(instance_==0);
    instance_=this;
    quit=false;
    bindAddress = new ENetAddress();
    enet_address_set_host (bindAddress, address);
    bindAddress->port = NETWORK_PORT;
  }

  ConnectionManager::~ConnectionManager(){
    if(!quit)
      quitListener();
    instance_=0;
    delete bindAddress;
  }


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
    return;
  }

  bool ConnectionManager::quitListener() {
    quit=true;
    receiverThread_->join();
    return true;
  }


  bool ConnectionManager::addPacket(ENetPacket *packet, ENetPeer *peer) {
    boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
    if(enet_peer_send(peer, NETWORK_DEFAULT_CHANNEL, packet)!=0)
      return false;
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, int clientID) {
    ClientInformation *temp = ClientInformation::findClient(clientID);
    if(!temp){
      COUT(3) << "C.Man: addPacket findClient failed" << std::endl;
      return false;
    }
    return addPacket(packet, temp->getPeer());
  }

  bool ConnectionManager::addPacketAll(ENetPacket *packet) {
    if(!instance_)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
    for(ClientInformation *i=ClientInformation::getBegin()->next(); i!=0; i=i->next()){
      COUT(3) << "adding broadcast packet for client: " << i->getID() << std::endl;
      if(enet_peer_send(i->getPeer(), 0, packet)!=0)
        return false;
    }
    return true;
  }

  // we actually dont need that function, because host_service does that for us
  bool ConnectionManager::sendPackets() {
    if(server==NULL || !instance_)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
    enet_host_flush(server);
    lock.unlock();
    return true;
  }

  void ConnectionManager::receiverThread() {
    // what about some error-handling here ?
    ENetEvent *event;
    atexit(enet_deinitialize);
    { //scope of the mutex
      boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
      enet_initialize();
      server = enet_host_create(bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
      lock.unlock();
    }
    if(server==NULL){
      // add some error handling here ==========================
      quit=true;
      return;
    }

    event = new ENetEvent;
    while(!quit){
      { //mutex scope
        boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
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
        case ENET_EVENT_TYPE_DISCONNECT:
        case ENET_EVENT_TYPE_RECEIVE:
            processData(event);
            event = new ENetEvent;
          break;
        case ENET_EVENT_TYPE_NONE:
          //receiverThread_->yield();
          msleep(1);
          break;
      }
//       usleep(100);
      //receiverThread_->yield(); //TODO: find apropriate
    }
    disconnectClients();
    // if we're finishied, destroy server
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
      enet_host_destroy(server);
      lock.unlock();
    }
  }

  //### added some bugfixes here, but we cannot test them because
  //### the server crashes everytime because of some gamestates
  //### (trying to resolve that now)
  void ConnectionManager::disconnectClients() {
    ENetEvent event;
    ClientInformation *temp = ClientInformation::getBegin()->next();
    while(temp!=0){
      {
        boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
        enet_peer_disconnect(temp->getPeer(), 0);
        lock.unlock();
      }
      temp = temp->next();
    }
    //bugfix: might be the reason why server crashes when clients disconnects
    temp = ClientInformation::getBegin()->next();
    boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
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
        if(ClientInformation::findClient(&(event.peer->address)))
          delete ClientInformation::findClient(&(event.peer->address));
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



  int ConnectionManager::getClientID(ENetPeer* peer) {
    return getClientID(&(peer->address));
  }

  int ConnectionManager::getClientID(ENetAddress* address) {
    return ClientInformation::findClient(address)->getID();
  }

  ENetPeer *ConnectionManager::getClientPeer(int clientID) {
    return ClientInformation::findClient(clientID)->getPeer();
  }


  void ConnectionManager::syncClassid(unsigned int clientID) {
    int failures=0;
    packet::ClassID *classid = new packet::ClassID();
    classid->setClientID(clientID);
    while(!classid->send() && failures < 10){
      failures++;
    }
    assert(failures<10);
    COUT(4) << "syncClassid:\tall synchClassID packets have been sent" << std::endl;
  }
  

  void ConnectionManager::disconnectClient(ClientInformation *client){
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_g);
      enet_peer_disconnect(client->getPeer(), 0);
      lock.unlock();
    }
  }


}
