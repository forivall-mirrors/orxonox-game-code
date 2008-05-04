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
#include "Synchronisable.h"
#include "core/CoreIncludes.h"

namespace network
{
  Client* Client::_sClient = 0;
  
  Client* Client::createSingleton(){
    if(!_sClient){
      _sClient = new Client();
    }
    return _sClient;
  }
  
  Client* Client::createSingleton(std::string address, int port){
    if(!_sClient)
      _sClient = new Client(address, port);
    return _sClient;
  }
  
  Client* Client::createSingleton(const char *address, int port){
    if(!_sClient)
      _sClient = new Client(address, port);
    return _sClient;
  }
  
  void Client::destroySingleton(){
    if(_sClient){
      delete _sClient;
      _sClient = 0;
    }
  }
  
  Client* Client::getSingleton(){
    return _sClient; 
  }
  
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

  Client::~Client(){
    if(isConnected)
      closeConnection();
  }
  
  /**
  * Establish the Connection to the Server
  * @return true/false
  */
  bool Client::establishConnection(){
    Synchronisable::setClient(true);
    isConnected=client_connection.createConnection();
    /*if(isConnected){
      COUT(2) << "sending connectrequest" << std::endl;
      client_connection.addPacket(pck_gen.generateConnectRequest());
      client_connection.sendPackets();
    }else
      COUT(2) << "could not create connection" << std::endl;*/
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
    if(client_connection.addPacket(pck_gen.chatMessage( message.c_str() )))
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
    while(!(client_connection.queueEmpty())){
      packet = client_connection.getPacket();
      COUT(5) << "tick packet size " << packet->dataLength << std::endl;
      elaborate(packet, 0); // ================= i guess we got to change this .... (client_ID is always same = server)
    }
    return;
  }

  void Client::processGamestate( GameStateCompressed *data){
    int id = data->id;
    COUT(5) << "received gamestate id: " << data->id << std::endl;
    if(gamestate.pushGameState(data)){
      client_connection.addPacket(pck_gen.acknowledgement(id));
      if(!client_connection.sendPackets())
        COUT(2) << "Could not send acknowledgment" << std::endl;
    }
  }

  void Client::processClassid(classid *clid){
    orxonox::Identifier *id;
    id=ID(std::string(clid->message));
    if(id!=NULL)
      id->setNetworkID(clid->clid);
    COUT(4) << "Client: received and set network id: " << clid->clid << "; classname: " << clid->message << std::endl;
    return;
  }

  void Client::processChat( chat *data){
    COUT(0) << "Server: " << data->message << std::endl;
  }
  
  bool Client::processWelcome( welcome *w ){
    COUT(4) << "processing welcome message" << std::endl;
    clientID_ = w->clientID;
    shipID_ = w->shipID;
    
  }

}
