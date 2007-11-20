//
// C++ Implementation: Client
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "network/Client.h"

namespace network{
  
  /**
   * Constructor for the Client class
   * initializes the address and the port to default localhost:NETWORK_PORT
   */
  Client::Client(): client_connection(NETWORK_PORT,"127.0.0.1"){
    // set server address to localhost
    //server_address="127.0.0.1";
    //port = NETWORK_PORT;
    isConnected=false;
  }
  
  /**
   * Constructor for the Client class
   * @param address the server address
   * @param port port of the application on the server
   */
  Client::Client(std::string address, int port) : client_connection(port, address){
    //server_address=address.c_str();
    //this->port = port;
    isConnected=false;
  }
  
  /**
   * Constructor for the Client class
   * @param address the server address
   * @param port port of the application on the server
   */
  Client::Client(const char *address, int port) : client_connection(port, address){
    //server_address = address;
    //this->port = port;
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
    ENetEvent event;
    // generate packet and add it to the queue
    if(!client_connection.addPacket(pck_gen.mousem(x, y)))
        return false;
    // send packets
    client_connection.sendPackets(&event);
    if(event.type==ENET_EVENT_TYPE_NONE)
      return true;
    else
      return false;
  }
  
  /**
   * submits a Keystrike to the server
   * @param key_code code to submit
   * @return true/false
   */
  bool Client::sendKeyboard(char key_code){
    ENetEvent event;
    // generate packet and add it to queue
    if(!client_connection.addPacket(pck_gen.keystrike(key_code)))
        return false;
    // send packets
    client_connection.sendPackets(&event);
    if(event.type==ENET_EVENT_TYPE_NONE)
      return true;
    else
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
    ENetEvent event;
    // generate packet and add it to queue
    if(client_connection.addPacket(pck_gen.keystrike(key_code)))
      return true;
    else
      return false;
  }
  
  /**
   * Performs a GameState update
   */
  void Client::update(){
    // to be implemented ==================
    
    
  }
  
}
