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
 *      Oliver Scheuss
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

#include <cassert>

#include "util/Clock.h"
#include "util/Debug.h"
#include "synchronisable/Synchronisable.h"
#include "packet/Chat.h"
#include "packet/Gamestate.h"
#include "FunctionCallManager.h"
#include "core/CoreIncludes.h"
#include "core/Game.h"

namespace orxonox
{


  /**
  * Constructor for the Client class
  * initializes the address and the port to default localhost:NETWORK_PORT
  */
  Client::Client():
      isSynched_(false),
      gameStateFailure_(false),
      timeSinceLastUpdate_(0)
  {
  }

  /**
  * Constructor for the Client class
  * @param address the server address
  * @param port port of the application on the server
  */
  Client::Client(const std::string& address, int port):
      isSynched_(false),
      gameStateFailure_(false),
      timeSinceLastUpdate_(0)
  {
      setPort( port );
      setServerAddress( address );
  }

  Client::~Client(){
    if ( ClientConnection::isConnected() )
      closeConnection();
  }

  /**
  * Establish the Connection to the Server
  * @return true/false
  */
  bool Client::establishConnection(){
    Synchronisable::setClient(true);
    return ClientConnection::establishConnection();
  }

  /**
  * closes the Connection to the Server
  * @return true/false
  */
  bool Client::closeConnection(){
    return ClientConnection::closeConnection();
  }

  bool Client::queuePacket(ENetPacket *packet, int clientID){
    bool b = ClientConnection::addPacket(packet);
    assert(b);
    return b;
  }

  bool Client::processChat(const std::string& message, unsigned int playerID){
//    COUT(1) << "Player " << playerID << ": " << message << std::endl;
    return true;
  }

  void Client::printRTT(){
    COUT(0) << "Round trip time to server is " << ClientConnection::getRTT() << " ms" << endl;
  }

  /**
   * This function implements the method of sending a chat message to the server
   * @param message message to be sent
   * @return result(true/false)
   */
  bool Client::chat(const std::string& message){
    packet::Chat *m = new packet::Chat(message, Host::getPlayerID());
    return m->send();
  }


  /**
   * Processes incoming packets, sends a gamestate to the server and does the cleanup
   * @param time
   */
  void Client::update(const Clock& time){
    //this steers our network frequency
    timeSinceLastUpdate_+=time.getDeltaTime();
    if(timeSinceLastUpdate_>=NETWORK_PERIOD)
    {
      timeSinceLastUpdate_ -= static_cast<unsigned int>( timeSinceLastUpdate_ / NETWORK_PERIOD ) * NETWORK_PERIOD;
      //     COUT(3) << '.';
      if ( isConnected() && isSynched_ )
      {
        COUT(4) << "popping partial gamestate: " << std::endl;
        packet::Gamestate *gs = gamestate.getGamestate();
        //assert(gs); <--- there might be the case that no data has to be sent, so its commented out now
        if(gs){
          COUT(4) << "client tick: sending gs " << gs << std::endl;
          if( !gs->send() )
            COUT(3) << "Problem adding partial gamestate to queue" << std::endl;
        // gs gets automatically deleted by enet callback
        }
        FunctionCallManager::sendCalls();
      }
    }
    sendPackets(); // flush the enet queue

    Connection::processQueue();
    if(gamestate.processGamestates())
    {
      if(!isSynched_)
        isSynched_=true;
    }
    gamestate.cleanup();
    Connection::sendPackets();

    return;
  }

  void Client::connectionClosed()
  {
    ObjectList<Synchronisable>::iterator it;
    for(it = ObjectList<Synchronisable>::begin(); it; )
    {
      if( it->getSyncMode() != 0x0 )
        (it++)->destroy();
      else
      {
        ++it;
      }
    }
    Game::getInstance().popState();
    Game::getInstance().popState();
  }

}
