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
#include "Host.h"
#include "Synchronisable.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "packet/Packet.h"
#include "packet/Acknowledgement.h"

namespace network
{
//   SetConsoleCommandShortcut(Client, chat);


  /**
  * Constructor for the Client class
  * initializes the address and the port to default localhost:NETWORK_PORT
  */
  Client::Client(): client_connection(NETWORK_PORT,"127.0.0.1"){
    // set server address to localhost
    isConnected=false;
    isSynched_=false;
    gameStateFailure_=false;
    isServer_ = false;
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
    isServer_ = false;
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
    isServer_ = false;
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

  bool Client::queuePacket(ENetPacket *packet, int clientID){
    return client_connection.addPacket(packet);
  }

  bool Client::processChat(packet::Chat *message, unsigned int clientID){
    return message->process();
  }

  /*bool Client::sendChat(packet::Chat *chat){
    chat->process();
    packet::Packet *p = new packet::Packet(chat);
    return p->send();
  }*/


  /**
  * submits a chat message to the server
  * @param message message to send
  * @return true/false
  */
  bool Client::sendChat( std::string message ){
    // generate packet and add it to queue
    if(!isConnected)
      return false;
    packet::Chat chat(message, 0);
    return chat.send();
    // send packets
  }

  /**
  * Performs a GameState update
  */
  void Client::tick(float time){
//     COUT(3) << ".";
    if(client_connection.isConnected() && isSynched_){
      COUT(4) << "popping partial gamestate: " << std::endl;
      packet::Gamestate *gs = gamestate.getGamestate();
      if(gs){
        COUT(4) << "client tick: sending gs " << gs << std::endl;
        if( !gs->send() )
          COUT(3) << "Problem adding partial gamestate to queue" << std::endl;
        // gs gets automatically deleted by enet callback
      }
    }
    ENetEvent *event;
    // stop if the packet queue is empty
    while(!(client_connection.queueEmpty())){
      event = client_connection.getEvent();
      COUT(5) << "tick packet size " << event->packet->dataLength << std::endl;
      packet::Packet *packet = packet::Packet::createPacket(event->packet, event->peer);
      assert(packet->process());
    }
    int gameStateID = gamestate.processGamestates();
    if(gameStateID==GAMESTATEID_INITIAL)
      if(gameStateFailure_){
        packet::Acknowledgement *ack = new packet::Acknowledgement((unsigned int)GAMESTATEID_INITIAL, 0);
        if(!ack->send())
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
      packet::Acknowledgement *ack = new packet::Acknowledgement(gameStateID, 0);
      if(!ack->send())
        COUT(3) << "could not ack gamestate" << std::endl;
    }// otherwise we had no gamestate to load
    gamestate.cleanup();
    return;
  }

}
