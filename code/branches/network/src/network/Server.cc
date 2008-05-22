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


namespace network
{
  
  
#define MAX_FAILURES 20;
  
  
  /**
  * Constructor for default values (bindaddress is set to ENET_HOST_ANY
  *
  */
  Server::Server() {
    packet_gen = PacketGenerator();
    clients = new ClientInformation(true);
    connection = new ConnectionManager(clients);
    gamestates = new GameStateManager(clients);
  }
  
  Server::Server(int port){
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
    updateGamestate();
//     usleep(500000); // TODO remove
    return;
  }

  /**
  * processes all the packets waiting in the queue
  */
  void Server::processQueue() {
    ENetPacket *packet;
    int clientID=-1;
    while(!connection->queueEmpty()){
      //std::cout << "Client " << clientID << " sent: " << std::endl;
      //clientID here is a reference to grab clientID from ClientInformation
      packet = connection->getPacket(clientID);
      if(!packet)
        continue;
      //if statement to catch case that packetbuffer is empty
      if( !elaborate(packet, clientID) ) 
        COUT(3) << "Server: could not elaborate" << std::endl;
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
    boost::recursive_mutex::scoped_lock lock(clients->mutex_);
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
        return false;
      }
      //std::cout << "adding gamestate" << std::endl;
      if ( !(connection->addPacket(packet_gen.gstate(gs), cid)) ){
        COUT(3) << "Server: packet with client id (cid): " << cid << " not sended: " << temp->getFailures() << std::endl; 
        temp->addFailure();
        if(temp->getFailures() > 20 )
          disconnectClient(temp);
      //std::cout << "added gamestate" << std::endl;
      }
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
    COUT(4) << "\b\b\b\n\n\n\n\nServer: processing ack from client: " << clientID << "; ack-id: " << data->a << std::endl;
    gamestates->ackGameState(clientID, data->a);
    delete data;
  }
  
  bool Server::processConnectRequest( connectRequest *con, int clientID ){
    COUT(3) << "processing connectRequest " << std::endl;
    //connection->addPacket(packet_gen.gstate(gamestates->popGameState(clientID)) , clientID);
    connection->createClient(clientID);
    delete con;
    return true;
  }
  
  void Server::processGamestate( GameStateCompressed *data, int clientID){
    COUT(4) << "processing partial gamestate from client " << clientID << std::endl;
    if(!gamestates->pushGameState(data, clientID))
        COUT(3) << "Could not push gamestate\t\t\t\t=====" << std::endl;
    else
      if(clients->findClient(clientID))
        clients->findClient(clientID)->resetFailures();
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
