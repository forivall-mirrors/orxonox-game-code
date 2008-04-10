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

#include <iostream>

#include "ConnectionManager.h"
#include "PacketTypes.h"
#include "GameStateManager.h"
#include "ClientInformation.h"
//#include "NetworkFrameListener.h"
#include "util/Sleep.h"
#include "Server.h"


namespace network
{
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
    connection->addPacketAll(packet);
    //std::cout <<"added packets" << std::endl;
    return connection->sendPackets();
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

    //sleep(1); // TODO remove
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
      //if statement to catch case that packetbuffer is empty
      if( !elaborate(packet, clientID) ) 
        COUT(4) << "Server: PacketBuffer empty" << std::endl;
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
      if(temp->head){
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
      if ( !(connection->addPacket(packet_gen.gstate(gs), cid)) )
        COUT(4) << "Server: packet with client id (cid): " << cid << " not sended" << std::endl; 
      //std::cout << "added gamestate" << std::endl;
      added=true;
      temp=temp->next();
    }
    if(added) {
      //std::cout << "send gamestates from server.cc in sendGameState" << std::endl;
      return connection->sendPackets();
    }
    COUT(5) << "Server: had no gamestates to send" << std::endl;
    return false;
  }

  void Server::processAck( ack *data, int clientID) {
    COUT(5) << "Server: processing ack from client: " << clientID << "; ack-id: " << data->id << std::endl;
    clients->findClient(clientID)->setGamestateID(data->a);
  }

}
