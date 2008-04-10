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
    std::cout <<"adding Packets" << std::endl;
    connection->addPacketAll(packet);
    //std::cout <<"added packets" << std::endl;
    if (connection->sendPackets()){
      std::cout << "Sucessfully" << std::endl;
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
      packet = connection->getPacket(clientID);
      elaborate(packet, clientID);
    }
  }

  /**
  * takes a new snapshot of the gamestate and sends it to the clients
  */
  void Server::updateGamestate() {
    gamestates->update();
    //std::cout << "updated gamestate, sending it" << std::endl;
    //if(clients->getGamestateID()!=GAMESTATEID_INITIAL)
      sendGameState();
    //std::cout << "sent gamestate" << std::endl;
  }

  /**
  * sends the gamestate
  */
  bool Server::sendGameState() {
    COUT(5) << "starting sendGameState" << std::endl;
    ClientInformation *temp = clients;
    bool added=false;
    while(temp!=NULL){
      if(temp->head){
        temp=temp->next();
        continue;
      }
      if( !(temp->getSynched()) ){
        COUT(5) << "not sending gamestate" << std::endl;
        temp=temp->next();
        continue;
      }
      COUT(5) << "doing gamestate gamestate preparation" << std::endl;
      int gid = temp->getGamestateID();
      int cid = temp->getID();
      COUT(5) << "server, got acked (gamestate) ID: " << gid << std::endl;
      GameStateCompressed *gs = gamestates->popGameState(cid);
      if(gs==NULL){
        COUT(2) << "could not generate gamestate" << std::endl;
        return false;
      }
      //std::cout << "adding gamestate" << std::endl;
      connection->addPacket(packet_gen.gstate(gs), cid);
      //std::cout << "added gamestate" << std::endl;
      added=true;
      temp=temp->next();
    }
    if(added)
      return connection->sendPackets();
    COUT(5) << "had no gamestates to send" << std::endl;
    return false;
  }

  void Server::processAck( ack *data, int clientID) {
    COUT(5) << "processing ack from client: " << clientID << "; ack-id: " << data->id << std::endl;
    clients->findClient(clientID)->setGamestateID(data->a);
  }

}
