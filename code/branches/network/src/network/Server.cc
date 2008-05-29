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
// C++ Implementation: Server
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "Server.h"

#include <iostream>


#include "ConnectionManager.h"
#include "PacketTypes.h"
#include "GameStateManager.h"
#include "ClientInformation.h"
//#include "NetworkFrameListener.h"
#include "util/Sleep.h"
#include "objects/SpaceShip.h"


namespace network
{
  
  
#define MAX_FAILURES 20;
#define NETWORK_FREQUENCY 30
  
  /**
  * Constructor for default values (bindaddress is set to ENET_HOST_ANY
  *
  */
  Server::Server() {
    timeSinceLastUpdate_=0;
    packet_gen = PacketGenerator();
    clients = new ClientInformation(true);
    connection = new ConnectionManager(clients);
    gamestates = new GameStateManager(clients);
  }
  
  Server::Server(int port){
    timeSinceLastUpdate_=0;
    packet_gen = PacketGenerator();
    clients = new ClientInformation(true);
    connection = new ConnectionManager(clients, port);
    gamestates = new GameStateManager(clients);
  }

  /**
  * Constructor
  * @param port Port to listen on
  * @param bindAddress Address to listen on
  */
  Server::Server(int port, std::string bindAddress) {
    timeSinceLastUpdate_=0;
    packet_gen = PacketGenerator();
    clients = new ClientInformation();
    connection = new ConnectionManager(port, bindAddress, clients);
    gamestates = new GameStateManager(clients);
  }

  /**
  * Constructor
  * @param port Port to listen on
  * @param bindAddress Address to listen on
  */
  Server::Server(int port, const char *bindAddress) {
    timeSinceLastUpdate_=0;
    packet_gen = PacketGenerator();
    clients = new ClientInformation();
    connection = new ConnectionManager(port, bindAddress, clients);
    gamestates = new GameStateManager(clients);
  }

  /**
  * This function opens the server by creating the listener thread
  */
  void Server::open() {
    connection->createListener();
    return;
  }

  /**
  * This function closes the server
  */
  void Server::close() {
    connection->quitListener();
    return;
  }

  /**
  * This function sends out a message to all clients
  * @param msg message
  * @return true/false
  */
  bool Server::sendMSG(std::string msg) {
    ENetPacket *packet = packet_gen.chatMessage(msg.c_str());
    //std::cout <<"adding packets" << std::endl;
    if(connection->addPacketAll(packet))
    //std::cout <<"added packets" << std::endl;
      return connection->sendPackets();
    else
      return false;
  }

  /**
  * This function sends out a message to all clients
  * @param msg message
  * @return true/false
  */
  bool Server::sendMSG(const char *msg) {
    ENetPacket *packet = packet_gen.chatMessage(msg);
    COUT(4) <<"Server: adding Packets" << std::endl;
    connection->addPacketAll(packet);
    //std::cout <<"added packets" << std::endl;
    if (connection->sendPackets()){
      COUT(4) << "Server: Sucessfully" << std::endl;
      return true;
    }
    return false;
  }

  /**
  * Run this function once every tick
  * calls processQueue and updateGamestate
  * @param time time since last tick
  */
  void Server::tick(float time) {
    processQueue();
    //this steers our network frequency
    timeSinceLastUpdate_+=time;
    if(timeSinceLastUpdate_>=(1./NETWORK_FREQUENCY)){
      timeSinceLastUpdate_-=(1./NETWORK_FREQUENCY);
      gamestates->processGameStates();
      updateGamestate();
    }
//     usleep(5000); // TODO remove
    return;
  }

  /**
  * processes all the packets waiting in the queue
  */
  void Server::processQueue() {
    ENetEvent *event;
    int clientID=-1;
    while(!connection->queueEmpty()){
      //std::cout << "Client " << clientID << " sent: " << std::endl;
      //clientID here is a reference to grab clientID from ClientInformation
      event = connection->getEvent();
      if(!event)
        continue;
      switch( event->type ) {
      case ENET_EVENT_TYPE_CONNECT:
        COUT(3) << "processing event_Type_connect" << std::endl;
        addClient(event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        if(clients->findClient(&event->peer->address))
          disconnectClient(event);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        if(clients->findClient(&event->peer->address)){
          clientID = clients->findClient(&event->peer->address)->getID();
          if( !elaborate(event->packet, clientID) ) 
            COUT(3) << "Server: could not elaborate" << std::endl;
        }
        break;
      }
      delete event;
      //if statement to catch case that packetbuffer is empty
    }
  }

  /**
  * takes a new snapshot of the gamestate and sends it to the clients
  */
  void Server::updateGamestate() {
    gamestates->update();
    COUT(4) << "Server: one gamestate update complete, goig to sendGameState" << std::endl;
    //std::cout << "updated gamestate, sending it" << std::endl;
    //if(clients->getGamestateID()!=GAMESTATEID_INITIAL)
    sendGameState();
    COUT(4) << "Server: one sendGameState turn complete, repeat in next tick" << std::endl;
    //std::cout << "sent gamestate" << std::endl;
  }

  /**
  * sends the gamestate
  */
  bool Server::sendGameState() {
    COUT(5) << "Server: starting function sendGameState" << std::endl;
    ClientInformation *temp = clients;
    bool added=false;
    while(temp != NULL){
      if(temp->getHead()){
        temp=temp->next();
        //think this works without continue
        continue;
      }
      if( !(temp->getSynched()) ){
        COUT(5) << "Server: not sending gamestate" << std::endl;
        temp=temp->next();
        //think this works without continue
        continue;
      }
      COUT(5) << "Server: doing gamestate gamestate preparation" << std::endl;
      int gid = temp->getGamestateID(); //get gamestate id
      int cid = temp->getID(); //get client id
      COUT(5) << "Server: got acked (gamestate) ID from clientlist: " << gid << std::endl;
      GameStateCompressed *gs = gamestates->popGameState(cid);
      if(gs==NULL){
        COUT(2) << "Server: could not generate gamestate (NULL from compress)" << std::endl;
        continue;
      }
      //std::cout << "adding gamestate" << std::endl;
      ENetPacket *packet = packet_gen.gstate(gs);
      if(!packet)
	continue;
      if ( !(connection->addPacket(packet, cid)) ){
        COUT(3) << "Server: packet with client id (cid): " << cid << " not sended: " << temp->getFailures() << std::endl; 
        temp->addFailure();
        /*if(temp->getFailures() > 0 )
          disconnectClient(temp);*/
      //std::cout << "added gamestate" << std::endl;
      }else
        temp->resetFailures();
      added=true;
      temp=temp->next();
      // now delete gamestate
      delete[] gs->data;
      delete gs;
    }
    if(added) {
      //std::cout << "send gamestates from server.cc in sendGameState" << std::endl;
      return connection->sendPackets();
    }
    COUT(5) << "Server: had no gamestates to send" << std::endl;
    return false;
  }

  void Server::processAck( ack *data, int clientID) {
    COUT(4) << "Server: processing ack from client: " << clientID << "; ack-id: " << data->a << std::endl;
    gamestates->ackGameState(clientID, data->a);
    delete data;
  }
  
  bool Server::processConnectRequest( connectRequest *con, int clientID ){
    //(COUT(3) << "processing connectRequest " << std::endl;
    //connection->addPacket(packet_gen.gstate(gamestates->popGameState(clientID)) , clientID);
    //createClient(clientID);
    delete con;
    return true;
  }
  
  void Server::processGamestate( GameStateCompressed *data, int clientID){
    COUT(4) << "processing partial gamestate from client " << clientID << std::endl;
    gamestates->addGameState(data, clientID);
        /*COUT(3) << "Could not push gamestate\t\t\t\t=====" << std::endl;
    else
      if(clients->findClient(clientID))
        clients->findClient(clientID)->resetFailures();*/
  }
  
  bool Server::addClient(ENetEvent *event){
    ClientInformation *temp = clients->insertBack(new ClientInformation);
    if(!temp){
      COUT(2) << "Server: could not add client" << std::endl;
      return false;
    }
    if(temp->prev()->getHead()) { //not good if you use anything else than insertBack
      temp->prev()->setID(0); //bugfix: not necessary but usefull
      temp->setID(1);
    }
    else
      temp->setID(temp->prev()->getID()+1);
    temp->setPeer(event->peer);
    COUT(3) << "Server: added client id: " << temp->getID() << std::endl;
    return createClient(temp->getID());
  }
  
  bool Server::createClient(int clientID){
    ClientInformation *temp = clients->findClient(clientID);
    if(!temp){
      COUT(2) << "Conn.Man. could not create client with id: " << clientID << std::endl;
      return false;
    }
    COUT(4) << "Con.Man: creating client id: " << temp->getID() << std::endl;
    connection->syncClassid(temp->getID());
    COUT(4) << "creating spaceship for clientid: " << temp->getID() << std::endl;
    // TODO: this is only a hack, untill we have a possibility to define default player-join actions
    if(!createShip(temp))
      COUT(2) << "Con.Man. could not create ship for clientid: " << clientID << std::endl;
    else
      COUT(3) << "created spaceship" << std::endl;
    temp->setSynched(true);
    COUT(3) << "sending welcome" << std::endl;
    connection->sendWelcome(temp->getID(), temp->getShipID(), true);
    return true;
  }
  
  bool Server::createShip(ClientInformation *client){
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
  }
  
  bool Server::disconnectClient(ENetEvent *event){
    COUT(4) << "removing client from list" << std::endl;
    //return removeClient(head_->findClient(&(peer->address))->getID());
    
    //boost::recursive_mutex::scoped_lock lock(head_->mutex_);
    orxonox::Iterator<orxonox::SpaceShip> it = orxonox::ObjectList<orxonox::SpaceShip>::start();
    ClientInformation *client = clients->findClient(&event->peer->address);
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
      return clients->removeClient(event->peer);
    }
    return false;
  }

  void Server::disconnectClient(int clientID){
    ClientInformation *client = clients->findClient(clientID);
    if(client)
      disconnectClient(client);
  }
  void Server::disconnectClient( ClientInformation *client){
    connection->disconnectClient(client);
    gamestates->removeClient(client);
  }
  
}
