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
#include <cassert>


#include "ConnectionManager.h"
#include "ClientConnectionListener.h"
#include "GamestateManager.h"
#include "ClientInformation.h"
#include "util/Sleep.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/Iterator.h"
#include "packet/Chat.h"
#include "packet/Packet.h"
#include "packet/Welcome.h"
#include "packet/DeleteObjects.h"
#include <util/Convert.h>

namespace network
{
  const int MAX_FAILURES = 20;
  const int NETWORK_FREQUENCY = 30;

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
  * @brief Destructor
  */
  Server::~Server(){
    if(connection)
      delete connection;
    if(gamestates_)
      delete gamestates_;
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

  bool Server::processChat(std::string message, unsigned int playerID){
    ClientInformation *temp = ClientInformation::getBegin();
    packet::Chat *chat;
    while(temp){
      chat = new packet::Chat(message, playerID);
      chat->setClientID(temp->getID());
      if(!chat->send())
        COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
      temp = temp->next();
    }
    COUT(1) << "Player " << playerID << ": " << message << std::endl;
    return true;
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
      timeSinceLastUpdate_=(float)((int)(timeSinceLastUpdate_*NETWORK_FREQUENCY))/timeSinceLastUpdate_;
      gamestates_->processGamestates();
      updateGamestate();
    }
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
    sendObjectDeletes();
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
        temp = temp->next();
        continue;
      }
      //std::cout << "adding gamestate" << std::endl;
      gs->setClientID(cid);
      if ( !gs->send() ){
        COUT(3) << "Server: packet with client id (cid): " << cid << " not sended: " << temp->getFailures() << std::endl;
        temp->addFailure();
      }else
        temp->resetFailures();
      added=true;
      temp=temp->next();
      // gs gets automatically deleted by enet callback
    }
    return true;
  }

  bool Server::sendObjectDeletes(){
    ClientInformation *temp = ClientInformation::getBegin();
    packet::DeleteObjects *del = new packet::DeleteObjects();
    if(!del->fetchIDs())
      return true;  //everything ok (no deletes this tick)
//     COUT(3) << "sending DeleteObjects" << std::endl;
    while(temp != NULL){
      if( !(temp->getSynched()) ){
        COUT(5) << "Server: not sending gamestate" << std::endl;
        temp=temp->next();
        continue;
      }
      int cid = temp->getID(); //get client id
      packet::DeleteObjects *cd = new packet::DeleteObjects(*del);
      assert(cd);
      cd->setClientID(cid);
      if ( !cd->send() )
        COUT(3) << "Server: packet with client id (cid): " << cid << " not sended: " << temp->getFailures() << std::endl;
      temp=temp->next();
      // gs gets automatically deleted by enet callback
    }
    return true;
  }


  bool Server::addClient(ENetEvent *event){
    unsigned int newid=0;

    ClientInformation *temp = ClientInformation::insertBack(new ClientInformation);
    if(!temp){
      COUT(2) << "Server: could not add client" << std::endl;
      return false;
    }
    if(temp==ClientInformation::getBegin()) { //not good if you use anything else than insertBack
      newid=1;
    }
    else
      newid=temp->prev()->getID()+1;
    temp->setID(newid);
    temp->setPeer(event->peer);

    // inform all the listeners
    orxonox::ObjectList<ClientConnectionListener>::iterator listener = orxonox::ObjectList<ClientConnectionListener>::begin();
    while(listener){
      listener->clientConnected(newid);
      listener++;
    }

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
    temp->setSynched(true);
    COUT(3) << "sending welcome" << std::endl;
    packet::Welcome *w = new packet::Welcome(temp->getID(), temp->getShipID());
    w->setClientID(temp->getID());
    bool b = w->send();
    assert(b);
    packet::Gamestate *g = new packet::Gamestate();
    g->setClientID(temp->getID());
    b = g->collectData(0);
    if(!b)
      return false; //no data for the client
    b = g->compressData();
    assert(b);
    b = g->send();
    assert(b);
    return true;
  }

  bool Server::disconnectClient(ENetEvent *event){
    COUT(4) << "removing client from list" << std::endl;
    //return removeClient(head_->findClient(&(peer->address))->getID());

    //boost::recursive_mutex::scoped_lock lock(head_->mutex_);
    ClientInformation *client = ClientInformation::findClient(&event->peer->address);
    if(!client)
      return false;
    gamestates_->removeClient(client);

// inform all the listeners
    orxonox::ObjectList<ClientConnectionListener>::iterator listener = orxonox::ObjectList<ClientConnectionListener>::begin();
    while(listener){
      listener->clientDisconnected(client->getID());
      listener++;
    }

    return ClientInformation::removeClient(event->peer);
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

  bool Server::chat(std::string message){
    ClientInformation *temp = ClientInformation::getBegin();
    packet::Chat *chat;
    while(temp){
      chat = new packet::Chat(message, Host::getPlayerID());
      chat->setClientID(temp->getID());
      if(!chat->send())
        COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
      temp = temp->next();
    }
    COUT(1) << "Player " << Host::getPlayerID() << ": " << message << std::endl;
    return true;
  }

}
