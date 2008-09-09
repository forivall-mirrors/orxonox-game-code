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
#include "GamestateManager.h"
#include "ClientInformation.h"
#include "util/Sleep.h"
#include "objects/SpaceShip.h"
#include "core/ConsoleCommand.h"
#include "core/Iterator.h"
#include "packet/Chat.h"
#include "packet/Packet.h"
#include "packet/Welcome.h"

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
    connection = new ConnectionManager();
    gamestates_ = new GamestateManager();
  }

  Server::Server(int port){
    timeSinceLastUpdate_=0;
    connection = new ConnectionManager(port);
    gamestates_ = new GamestateManager();
  }

  /**
  * Constructor
  * @param port Port to listen on
  * @param bindAddress Address to listen on
  */
  Server::Server(int port, std::string bindAddress) {
    timeSinceLastUpdate_=0;
    connection = new ConnectionManager(port, bindAddress);
    gamestates_ = new GamestateManager();
  }

  /**
  * Constructor
  * @param port Port to listen on
  * @param bindAddress Address to listen on
  */
  Server::Server(int port, const char *bindAddress) {
    timeSinceLastUpdate_=0;
    connection = new ConnectionManager(port, bindAddress);
    gamestates_ = new GamestateManager();
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

  bool Server::processChat(packet::Chat *message, unsigned int clientID){
    ClientInformation *temp = ClientInformation::getBegin();
    while(temp){
      message->setClientID(temp->getID());
      if(!message->send())
        COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
      temp = temp->next();
    }
    return message->process();
  }

  /**
  * This function sends out a message to all clients
  * @param msg message
  * @return true/false
  */
  bool Server::sendChat(packet::Chat *chat) {
    //TODO: change this (no informations about who wrote a message)
    assert(0);
    ClientInformation *temp = ClientInformation::getBegin();
    while(temp){
      chat->setClientID(temp->getID());
      if(!chat->send())
        COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
    }
    return chat->process();;
  }

  /**
  * This function sends out a message to all clients
  * @param msg message
  * @return true/false
  */
//   bool Server::sendChat(const char *msg) {
//     char *message = new char [strlen(msg)+10+1];
//     sprintf(message, "Player %d: %s", CLIENTID_SERVER, msg);
//     COUT(1) << message << std::endl;
//     ENetPacket *packet = packet_gen.chatMessage(message);
//     COUT(5) <<"Server: adding Packets" << std::endl;
//     return connection->addPacketAll(packet);
//   }

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
      timeSinceLastUpdate_=(float)((int)(timeSinceLastUpdate_*NETWORK_FREQUENCY))/timeSinceLastUpdate_;
//      timeSinceLastUpdate_-=1./NETWORK_FREQUENCY;
      gamestates_->processGamestates();
      updateGamestate();
    }
    /*while(timeSinceLastUpdate_>1./NETWORK_FREQUENCY)
      timeSinceLastUpdate_-=1./NETWORK_FREQUENCY;*/
//     usleep(5000); // TODO remove
    return;
  }

  bool Server::queuePacket(ENetPacket *packet, int clientID){
    return connection->addPacket(packet, clientID);
  }

  /**
  * processes all the packets waiting in the queue
  */
  void Server::processQueue() {
    ENetEvent *event;
    while(!connection->queueEmpty()){
      //std::cout << "Client " << clientID << " sent: " << std::endl;
      //clientID here is a reference to grab clientID from ClientInformation
      event = connection->getEvent();
      if(!event)
        continue;
      assert(event->type != ENET_EVENT_TYPE_NONE);
      switch( event->type ) {
      case ENET_EVENT_TYPE_CONNECT:
        COUT(3) << "processing event_Type_connect" << std::endl;
        addClient(event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        if(ClientInformation::findClient(&event->peer->address))
          disconnectClient(event);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        if(!processPacket(event->packet, event->peer))
          COUT(3) << "processing incoming packet failed" << std::endl;
        break;
      default:
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
    gamestates_->update();
    COUT(5) << "Server: one gamestate update complete, goig to sendGameState" << std::endl;
    //std::cout << "updated gamestate, sending it" << std::endl;
    //if(clients->getGamestateID()!=GAMESTATEID_INITIAL)
    sendGameState();
    COUT(5) << "Server: one sendGameState turn complete, repeat in next tick" << std::endl;
    //std::cout << "sent gamestate" << std::endl;
  }

  bool Server::processPacket( ENetPacket *packet, ENetPeer *peer ){
    packet::Packet *p = packet::Packet::createPacket(packet, peer);
    return p->process();
  }

  /**
  * sends the gamestate
  */
  bool Server::sendGameState() {
    COUT(5) << "Server: starting function sendGameState" << std::endl;
    ClientInformation *temp = ClientInformation::getBegin();
    bool added=false;
    while(temp != NULL){
      if( !(temp->getSynched()) ){
        COUT(5) << "Server: not sending gamestate" << std::endl;
        temp=temp->next();
        if(!temp)
          break;
        //think this works without continue
        continue;
      }
      COUT(4) << "client id: " << temp->getID() << " RTT: " << temp->getRTT() << " loss: " << temp->getPacketLoss() << std::endl;
      COUT(5) << "Server: doing gamestate gamestate preparation" << std::endl;
      int gid = temp->getGamestateID(); //get gamestate id
      int cid = temp->getID(); //get client id
      COUT(5) << "Server: got acked (gamestate) ID from clientlist: " << gid << std::endl;
      packet::Gamestate *gs = gamestates_->popGameState(cid);
      if(gs==NULL){
        COUT(2) << "Server: could not generate gamestate (NULL from compress)" << std::endl;
        continue;
      }
      //std::cout << "adding gamestate" << std::endl;
      gs->setClientID(cid);
      assert(gs->compressData());
      if ( !gs->send() ){
        COUT(3) << "Server: packet with client id (cid): " << cid << " not sended: " << temp->getFailures() << std::endl;
        temp->addFailure();
      }else
        temp->resetFailures();
      added=true;
      temp=temp->next();
      // gs gets automatically deleted by enet callback
    }
    /*if(added) {
      //std::cout << "send gamestates from server.cc in sendGameState" << std::endl;
      return connection->sendPackets();
    }*/
    //COUT(5) << "Server: had no gamestates to send" << std::endl;
    return true;
  }

//   void Server::processChat( chat *data, int clientId){
//     char *message = new char [strlen(data->message)+10+1];
//     sprintf(message, "Player %d: %s", clientId, data->message);
//     COUT(1) << message << std::endl;
//     ENetPacket *pck = packet_gen.chatMessage(message);
//     connection->addPacketAll(pck);
//     delete[] data->message;
//     delete data;
//   }

  bool Server::addClient(ENetEvent *event){
    ClientInformation *temp = ClientInformation::insertBack(new ClientInformation);
    if(!temp){
      COUT(2) << "Server: could not add client" << std::endl;
      return false;
    }
    if(temp->prev()->getBegin()) { //not good if you use anything else than insertBack
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
    ClientInformation *temp = ClientInformation::findClient(clientID);
    if(!temp){
      COUT(2) << "Conn.Man. could not create client with id: " << clientID << std::endl;
      return false;
    }
    COUT(4) << "Con.Man: creating client id: " << temp->getID() << std::endl;
    connection->syncClassid(temp->getID());
    COUT(5) << "creating spaceship for clientid: " << temp->getID() << std::endl;
    // TODO: this is only a hack, untill we have a possibility to define default player-join actions
    if(!createShip(temp))
      COUT(2) << "Con.Man. could not create ship for clientid: " << clientID << std::endl;
    else
      COUT(3) << "created spaceship" << std::endl;
    temp->setSynched(true);
    COUT(3) << "sending welcome" << std::endl;
    packet::Welcome *w = new packet::Welcome(temp->getID(), temp->getShipID());
    w->setClientID(temp->getID());
    assert(w->send());
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
    no->classID = id->getNetworkID();
    client->setShipID(no->objectID);
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
    no->create();
    no->setBacksync(true);

    return true;
  }

  bool Server::disconnectClient(ENetEvent *event){
    COUT(4) << "removing client from list" << std::endl;
    //return removeClient(head_->findClient(&(peer->address))->getID());

    //boost::recursive_mutex::scoped_lock lock(head_->mutex_);
    orxonox::ObjectList<orxonox::SpaceShip>::iterator it = orxonox::ObjectList<orxonox::SpaceShip>::begin();
    ClientInformation *client = ClientInformation::findClient(&event->peer->address);
    if(!client)
      return false;
    while(it){
      if(it->objectID!=client->getShipID()){
        ++it;
        continue;
      }
      orxonox::ObjectList<orxonox::SpaceShip>::iterator temp=it;
      ++it;
      delete  *temp;
      return ClientInformation::removeClient(event->peer);
    }
    return false;
  }

  void Server::disconnectClient(int clientID){
    ClientInformation *client = ClientInformation::findClient(clientID);
    if(client)
      disconnectClient(client);
  }
  void Server::disconnectClient( ClientInformation *client){
    connection->disconnectClient(client);
    gamestates_->removeClient(client);
  }

}
