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

namespace network{

  /**
   * Constructor for default values (bindaddress is set to ENET_HOST_ANY
   *
   */
  Server::Server(){
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
  Server::Server(int port, std::string bindAddress){
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
  Server::Server(int port, const char *bindAddress){
    packet_gen = PacketGenerator();
    clients = new ClientInformation();
    connection = new ConnectionManager(port, bindAddress, clients);
    gamestates = new GameStateManager(clients);
  }
  
  /**
   * This function opens the server by creating the listener thread
   */
  void Server::open(){
    connection->createListener();
    return;
  }
  
  /**
   * This function closes the server
   */
  void Server::close(){
    connection->quitListener();
    return;
  }
  
  /**
   * This function sends out a message to all clients
   * @param msg message
   * @return true/false
   */
  bool Server::sendMSG(std::string msg){
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
  bool Server::sendMSG(const char *msg){
    ENetPacket *packet = packet_gen.chatMessage(msg);
    std::cout <<"adding PPPPPackets" << std::endl;
    connection->addPacketAll(packet);
    //std::cout <<"added packets" << std::endl;
    if (connection->sendPackets()){
	std::cout << "SSSSSucessfully" << std::endl;
	return true;
    }
    return false;
  }
  
  /**
   * Run this function once every tick
   * calls processQueue and updateGamestate
   * @param time time since last tick
   */
  void Server::tick(float time){
    processQueue();
    updateGamestate();
    return;
  }
  
  /**
   * processes all the packets waiting in the queue
   */
  void Server::processQueue(){
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
  void Server::updateGamestate(){
    gamestates->update();
    std::cout << "updated gamestate, sending it" << std::endl;
    sendGameState();
    std::cout << "sent gamestate" << std::endl;
  }
  
  /**
   * sends the gamestate
   */
  bool Server::sendGameState(){
    for(ClientInformation *temp = clients; temp!=0; temp=temp->next()){
      connection->addPacket(packet_gen.gstate(&(gamestates->popGameState(temp->getID()))),temp->getID());
    }
    return true;
  }
  
  
}
