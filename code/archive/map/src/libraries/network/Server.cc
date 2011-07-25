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

#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h>
#include <cassert>
#include <string>

#include "util/Clock.h"
#include "util/Debug.h"
#include "core/ObjectList.h"
#include "core/Executor.h"
#include "packet/Chat.h"
#include "packet/ClassID.h"
#include "packet/DeleteObjects.h"
#include "packet/FunctionIDs.h"
#include "packet/Gamestate.h"
#include "packet/Welcome.h"
#include "ChatListener.h"
#include "ClientInformation.h"
#include "FunctionCallManager.h"
#include "GamestateManager.h"

namespace orxonox
{
  const unsigned int MAX_FAILURES = 20;

  /**
  * Constructor for default values (bindaddress is set to ENET_HOST_ANY
  *
  */
  Server::Server() {
    this->timeSinceLastUpdate_=0;
  }

  Server::Server(int port){
    this->setPort( port );
    this->timeSinceLastUpdate_=0;
  }

  /**
  * Constructor
  * @param port Port to listen on
  * @param bindAddress Address to listen on
  */
  Server::Server(int port, const std::string& bindAddress) {
    this->setPort( port );
    this->setBindAddress( bindAddress );
    this->timeSinceLastUpdate_=0;
  }

  /**
  * @brief Destructor
  */
  Server::~Server(){
  }

  /**
  * This function opens the server by creating the listener thread
  */
  void Server::open() {
    COUT(4) << "opening server" << endl;
    this->openListener();
    return;
  }

  /**
  * This function closes the server
  */
  void Server::close() {
    COUT(4) << "closing server" << endl;
    this->disconnectClients();
    this->closeListener();
    return;
  }

  bool Server::processChat(const std::string& message, unsigned int playerID){
    ClientInformation *temp = ClientInformation::getBegin();
    packet::Chat *chat;
    while(temp){
      chat = new packet::Chat(message, playerID);
      chat->setClientID(temp->getID());
      if(!chat->send())
        COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
      temp = temp->next();
    }
//    COUT(1) << "Player " << playerID << ": " << message << std::endl;
    return true;
  }


  /**
  * Run this function once every tick
  * calls processQueue and updateGamestate
  * @param time time since last tick
  */
  void Server::update(const Clock& time) {
    // receive incoming packets
    Connection::processQueue();

    if ( ClientInformation::hasClients() )
    {
      // process incoming gamestates
      GamestateManager::processGamestates();

      // send function calls to clients
      FunctionCallManager::sendCalls();

      //this steers our network frequency
      timeSinceLastUpdate_+=time.getDeltaTime();
      if(timeSinceLastUpdate_>=NETWORK_PERIOD)
      {
        timeSinceLastUpdate_ -= static_cast<unsigned int>( timeSinceLastUpdate_ / NETWORK_PERIOD ) * NETWORK_PERIOD;
        updateGamestate();
      }
      sendPackets(); // flush the enet queue
    }
  }

  bool Server::queuePacket(ENetPacket *packet, int clientID){
    return ServerConnection::addPacket(packet, clientID);
  }

  /**
   * @brief: returns ping time to client in milliseconds
   */
  unsigned int Server::getRTT(unsigned int clientID){
    assert(ClientInformation::findClient(clientID));
    return ClientInformation::findClient(clientID)->getRTT();
  }

  void Server::printRTT()
  {
    for( ClientInformation* temp=ClientInformation::getBegin(); temp!=0; temp=temp->next() )
      COUT(0) << "Round trip time to client with ID: " << temp->getID() << " is " << temp->getRTT() << " ms" << endl;
  }

  /**
   * @brief: return packet loss ratio to client (scales from 0 to 1)
   */
  double Server::getPacketLoss(unsigned int clientID){
    assert(ClientInformation::findClient(clientID));
    return ClientInformation::findClient(clientID)->getPacketLoss();
  }

  /**
  * takes a new snapshot of the gamestate and sends it to the clients
  */
  void Server::updateGamestate() {
    if( ClientInformation::getBegin()==NULL )
      //no client connected
      return;
    GamestateManager::update();
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
//     COUT(5) << "Server: starting function sendGameState" << std::endl;
//     ClientInformation *temp = ClientInformation::getBegin();
//     bool added=false;
//     while(temp != NULL){
//       if( !(temp->getSynched()) ){
//         COUT(5) << "Server: not sending gamestate" << std::endl;
//         temp=temp->next();
//         if(!temp)
//           break;
//         continue;
//       }
//       COUT(4) << "client id: " << temp->getID() << " RTT: " << temp->getRTT() << " loss: " << temp->getPacketLoss() << std::endl;
//       COUT(5) << "Server: doing gamestate gamestate preparation" << std::endl;
//       int cid = temp->getID(); //get client id
//       packet::Gamestate *gs = GamestateManager::popGameState(cid);
//       if(gs==NULL){
//         COUT(2) << "Server: could not generate gamestate (NULL from compress)" << std::endl;
//         temp = temp->next();
//         continue;
//       }
//       //std::cout << "adding gamestate" << std::endl;
//       gs->setClientID(cid);
//       if ( !gs->send() ){
//         COUT(3) << "Server: packet with client id (cid): " << cid << " not sended: " << temp->getFailures() << std::endl;
//         temp->addFailure();
//       }else
//         temp->resetFailures();
//       added=true;
//       temp=temp->next();
//       // gs gets automatically deleted by enet callback
//     }
    GamestateManager::sendGamestates();
    return true;
  }

  bool Server::sendObjectDeletes(){
    ClientInformation *temp = ClientInformation::getBegin();
    if( temp == NULL )
      //no client connected
      return true;
    packet::DeleteObjects *del = new packet::DeleteObjects();
    if(!del->fetchIDs())
    {
      delete del;
      return true;  //everything ok (no deletes this tick)
    }
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
    delete del;
    return true;
  }


  void Server::addPeer(ENetEvent *event){
    static unsigned int newid=1;

    COUT(2) << "Server: adding client" << std::endl;
    ClientInformation *temp = ClientInformation::insertBack(new ClientInformation);
    if(!temp){
      COUT(2) << "Server: could not add client" << std::endl;
    }
    temp->setID(newid);
    temp->setPeer(event->peer);

    // inform all the listeners
    ClientConnectionListener::broadcastClientConnected(newid);

    ++newid;

    COUT(3) << "Server: added client id: " << temp->getID() << std::endl;
    createClient(temp->getID());
}

  void Server::removePeer(ENetEvent *event)
  {
    COUT(4) << "removing client from list" << std::endl;
    ClientInformation *client = ClientInformation::findClient(&event->peer->address);
    if(!client)
      return;
    else
    {
      //ServerConnection::disconnectClient( client );
      //ClientConnectionListener::broadcastClientDisconnected( client->getID() ); //this is done in ClientInformation now
      delete client;
    }
  }

  bool Server::createClient(int clientID){
    ClientInformation *temp = ClientInformation::findClient(clientID);
    if(!temp){
      COUT(2) << "Conn.Man. could not create client with id: " << clientID << std::endl;
      return false;
    }
    COUT(5) << "Con.Man: creating client id: " << temp->getID() << std::endl;

    // synchronise class ids
    syncClassid(temp->getID());

    // now synchronise functionIDs
    packet::FunctionIDs *fIDs = new packet::FunctionIDs();
    fIDs->setClientID(clientID);
    bool b = fIDs->send();
    assert(b);

    temp->setSynched(true);
    COUT(4) << "sending welcome" << std::endl;
    packet::Welcome *w = new packet::Welcome(temp->getID(), temp->getShipID());
    w->setClientID(temp->getID());
    b = w->send();
    assert(b);
    packet::Gamestate *g = new packet::Gamestate();
    g->setClientID(temp->getID());
    b = g->collectData(0,0x1);
    if(!b)
      return false; //no data for the client
    b = g->compressData();
    assert(b);
    b = g->send();
    assert(b);
    return true;
  }

  void Server::disconnectClient( ClientInformation *client ){
    ServerConnection::disconnectClient( client );
    GamestateManager::removeClient(client);
    // inform all the listeners
    // ClientConnectionListener::broadcastClientDisconnected(client->getID()); // this is done in ClientInformation now
  }

  bool Server::chat(const std::string& message){
      return this->sendChat(message, Host::getPlayerID());
  }

  bool Server::broadcast(const std::string& message){
      return this->sendChat(message, CLIENTID_UNKNOWN);
  }

  bool Server::sendChat(const std::string& message, unsigned int clientID){
    ClientInformation *temp = ClientInformation::getBegin();
    packet::Chat *chat;
    while(temp){
      chat = new packet::Chat(message, clientID);
      chat->setClientID(temp->getID());
      if(!chat->send())
        COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
      temp = temp->next();
    }
//    COUT(1) << "Player " << Host::getPlayerID() << ": " << message << std::endl;
    for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
      it->incomingChat(message, clientID);

    return true;
  }

  void Server::syncClassid(unsigned int clientID) {
    int failures=0;
    packet::ClassID *classid = new packet::ClassID();
    classid->setClientID(clientID);
    while(!classid->send() && failures < 10){
      failures++;
    }
    assert(failures<10);
    COUT(4) << "syncClassid:\tall synchClassID packets have been sent" << std::endl;
  }

}
