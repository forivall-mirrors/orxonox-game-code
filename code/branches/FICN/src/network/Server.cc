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
    connection = ConnectionManager();
    gamestates = GameStateManager();
    packet_gen = PacketGenerator();
  }

  /**
   * Constructor
   * @param port Port to listen on
   * @param bindAddress Address to listen on
   */
  Server::Server(int port, std::string bindAddress){
    connection = ConnectionManager(port, bindAddress);
    gamestates = GameStateManager();
    packet_gen = PacketGenerator();
  }

  /**
   * Constructor
   * @param port Port to listen on
   * @param bindAddress Address to listen on
   */
  Server::Server(int port, const char *bindAddress){
    connection = ConnectionManager(port, bindAddress);
    gamestates = GameStateManager();
    packet_gen = PacketGenerator();
  }
  
  /**
   * This function opens the server by creating the listener thread
   */
  void Server::open(){
    connection.createListener();
    return;
  }
  
  /**
   * This function closes the server
   */
  void Server::close(){
    connection.quitListener();
    return;
  }
  
  /**
   * This function sends out a message to all clients
   * @param msg message
   * @return true/false
   */
  bool Server::sendMSG(std::string msg){
    ENetPacket *packet = packet_gen.chatMessage(msg.c_str());
    connection.addPacketAll(packet);
    return connection.sendPackets();
  }
  /**
   * This function sends out a message to all clients
   * @param msg message
   * @return true/false
   */
  bool Server::sendMSG(const char *msg){
    ENetPacket *packet = packet_gen.chatMessage(msg);
    connection.addPacketAll(packet);
    return connection.sendPackets();
  }
  
  /**
   * Run this function once every tick
   * calls processQueue and updateGamestate
   */
  void Server::tick(){
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
    while(!connection.queueEmpty()){
      packet = connection.getPacket(clientID);
    }
  }
  
  /**
   * takes a new snapshot of the gamestate and sends it to the clients
   */
  void Server::updateGamestate(){
  }
  
  
}
