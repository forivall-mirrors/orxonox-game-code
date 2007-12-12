//
// C++ Implementation: Client
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "Client.h"

namespace network{

  /**
   * Constructor for the Client class
   * initializes the address and the port to default localhost:NETWORK_PORT
   */
  Client::Client(): client_connection(NETWORK_PORT,"127.0.0.1"){
    // set server address to localhost
    isConnected=false;
  }

  /**
   * Constructor for the Client class
   * @param address the server address
   * @param port port of the application on the server
   */
  Client::Client(std::string address, int port) : client_connection(port, address){
    isConnected=false;
  }

  /**
   * Constructor for the Client class
   * @param address the server address
   * @param port port of the application on the server
   */
  Client::Client(const char *address, int port) : client_connection(port, address){
    isConnected=false;
  }

  /**
   * Establish the Connection to the Server
   * @return true/false
   */
  bool Client::establishConnection(){
    isConnected=client_connection.createConnection();
    return isConnected;
  }

  /**
   * closes the Connection to the Server
   * @return true/false
   */
  bool Client::closeConnection(){
    isConnected=false;
    return client_connection.closeConnection();
  }

  /**
   * submits a MouseAction to the server
   * @param x x Coordinate
   * @param y y Coordinate
   * @return true/false
   */
  bool Client::sendMouse(double x, double y){
    // generate packet and add it to the queue
    if(!isConnected)
      return false;
    if(!client_connection.addPacket(pck_gen.mousem(x, y)))
        return false;
    // send packets
    return client_connection.sendPackets();
  }

  /**
   * submits a Keystrike to the server
   * @param key_code code to submit
   * @return true/false
   */
  bool Client::sendKeyboard(char key_code){
    // generate packet and add it to queue
    if(!isConnected)
      return false;
    if(!client_connection.addPacket(pck_gen.keystrike(key_code)))
        return false;
    // send packets
    return client_connection.sendPackets();
  }
  
  /**
   * submits a chat message to the server
   * @param message message to send
   * @return true/false
   */
  bool Client::sendChat( std::string message ){
    // generate packet and add it to queue
    if(!isConnected)
      return false;
    if(client_connection.addPacket(pck_gen.chatMessage( message.c_str() )));
      return client_connection.sendPackets();
    // send packets
    return false;
  }

  /**
   * Adds a MouseAction to the PacketQueue
   * @param x x Coordinate
   * @param y y Coordinate
   * @return true/false
   */
  bool Client::addMouse(double x, double y){
    // generate packet and add it to the queue
    if(client_connection.addPacket(pck_gen.mousem(x, y)))
      return true;
    else
      return false;
  }

  /**
   * Adds a Keystrike to the PacketQueue
   * @param key_code
   * @return true/false
   */
  bool Client::addKeyboard(char key_code){
    // generate packet and add it to queue
    if(client_connection.addPacket(pck_gen.keystrike(key_code)))
      return true;
    else
      return false;
  }

  /** 
   * Sends out all the packets queued by addXXX
   */
  bool Client::sendPackets(){
    if(!isConnected)
      return false;
    ENetEvent event;
    // send packets
    client_connection.sendPackets(&event);
    if(event.type==ENET_EVENT_TYPE_NONE)
      return true;
    else
      return false;
  }
  
  /**
   * Performs a GameState update
   */
  void Client::tick(float time){
    ENetPacket *packet;
    // stop if the packet queue is empty
    while(!client_connection.queueEmpty()){
      packet = client_connection.getPacket();
      elaborate(packet, 0); // ================= i guess we got to change this .... (client_ID is always same = server)
    }
    return;
  }
  
  void Client::processGamestate( GameStateCompressed *data){
    gamestate.pushGameState(*data);
    client_connection.addPacket(pck_gen.acknowledgement(data->id));
    client_connection.sendPackets();
    return;
  }
  
  void Client::processClassid(classid *clid){
    orxonox::Identifier *id;
    id=ID(std::string(clid->message));
    if(id!=NULL)
      id->setNetworkID(clid->clid);
    return;
  }
  
  void Client::processChat( chat *data){
    std::cout << "Server: " << data->message << std::endl;
  }
  
}
