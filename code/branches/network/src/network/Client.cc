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
    isSynched_=false;
    gameStateFailure_=false;
  }

  /**
  * Constructor for the Client class
  * @param address the server address
  * @param port port of the application on the server
  */
  Client::Client(std::string address, int port) : client_connection(port, address){
    isConnected=false;
    isSynched_=false;
    gameStateFailure_=false;
  }

  /**
  * Constructor for the Client class
  * @param address the server address
  * @param port port of the application on the server
  */
  Client::Client(const char *address, int port) : client_connection(port, address){
    isConnected=false;
    isSynched_=false;
    gameStateFailure_=false;
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
    if(isConnected){
//       COUT(3) << "sending connectrequest" << std::endl;
//       if(!client_connection.addPacket(pck_gen.generateConnectRequest()) || !client_connection.sendPackets())
//         COUT(1) << "could not send connection request !!!!!!!!!" << std::endl;
    }else
      COUT(1) << "could not create connection laber" << std::endl;
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
  * Sends out all the packets queued by addXXX
  */
  bool Client::sendPackets(){
    if(!isConnected)
      return false;
    // send packets
    client_connection.sendPackets();
    return true;
  }

  /**
  * Performs a GameState update
  */
  void Client::tick(float time){
//     COUT(3) << ".";
    if(client_connection.isConnected() && isSynched_){
      COUT(4) << "popping partial gamestate: " << std::endl;
      GameStateCompressed *gs = gamestate.popPartialGameState();
      if(gs){
        COUT(4) << "client tick: sending gs " << gs << std::endl;
        ENetPacket *packet = pck_gen.gstate(gs);
        if( packet == NULL || !client_connection.addPacket(packet))
          COUT(3) << "Problem adding partial gamestate to queue" << std::endl;
        // now delete it to save memory
        delete[] gs->data;
        delete gs;
      }
    }
    ENetEvent *event;
    // stop if the packet queue is empty
    while(!(client_connection.queueEmpty())){
      event = client_connection.getEvent();
      COUT(5) << "tick packet size " << event->packet->dataLength << std::endl;
      elaborate(event->packet, 0); // ================= i guess we got to change this .... (client_ID is always same = server)
    }
    int gameStateID = gamestate.processGameState();
    if(gameStateID==GAMESTATEID_INITIAL)
      if(gameStateFailure_){
        if(!client_connection.addPacket(pck_gen.acknowledgement(GAMESTATEID_INITIAL)))
          COUT(3) << "could not (negatively) ack gamestate" << std::endl;
        else 
          COUT(4) << "negatively acked a gamestate" << std::endl;
        }
      else
        gameStateFailure_=true;
    else if(gameStateID!=0){
      // ack gamestate and set synched
      if(!isSynched_)
        isSynched_=true;
      gameStateFailure_=false;
      if(!client_connection.addPacket(pck_gen.acknowledgement(gameStateID)))
        COUT(3) << "could not ack gamestate" << std::endl;
    }// otherwise we had no gamestate to load
    gamestate.cleanup();
    if(!client_connection.sendPackets())
      COUT(3) << "Problem sending packets to server" << std::endl;
    return;
  }

  void Client::processGamestate( GameStateCompressed *data, int clientID){
    COUT(5) << "received gamestate id: " << data->id << std::endl;
    gamestate.addGameState(data);
  }

  void Client::processClassid(classid *clid){
    orxonox::Identifier *id;
    id=ID(std::string(clid->message));
    if(id!=NULL)
      id->setNetworkID(clid->clid);
    COUT(4) << "Client: received and set network id: " << clid->clid << "; classname: " << clid->message << std::endl;
    COUT(4) << "id(classid)->getName " << ID((unsigned int)clid->clid)->getName() << std::endl;
    delete clid;
    return;
  }

  void Client::processChat( chat *data){
    COUT(0) << "Server: " << data->message << std::endl;
    delete[] data->message;
    delete data;
  }
  
  bool Client::processWelcome( welcome *w ){
    COUT(4) << "processing welcome message" << std::endl;
    clientID_ = w->clientID;
    shipID_ = w->shipID;
    delete w;
    return true;
  }

}
