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
#include "util/Output.h"
#include "synchronisable/Synchronisable.h"
#include "packet/Chat.h"
#include "packet/Gamestate.h"
#include "FunctionCallManager.h"
#include "core/CoreIncludes.h"
#include "core/Game.h"
#include "core/commandline/CommandLineParser.h"
#include "core/singleton/ScopedSingletonIncludes.h"

namespace orxonox
{

  ManageScopedSingleton( Client, ScopeID::ROOT, false );

  /**
  * Constructor for the Client class
  * initializes the address and the port to default localhost:NETWORK_PORT
  */
  Client::Client():
      isSynched_(false),
      gameStateFailure_(false),
      timeSinceLastUpdate_(0)
  {
    this->setDestination( CommandLineParser::getValue("dest").get<std::string>(), CommandLineParser::getValue("port") );
  }

  Client::~Client()
  {
    if ( ClientConnection::isConnected() )
      closeConnection();
  }

  /**
  * Establish the Connection to the Server
  * @return true/false
  */
  bool Client::establishConnection()
  {
    Synchronisable::setClient(true);
    if( ClientConnection::establishConnection() )
    {
      Host::setActive(true);
      GamestateManager::addPeer(NETWORK_PEER_ID_SERVER);
      return true;
    }
    else
      return false;
  }

  /**
  * closes the Connection to the Server
  * @return true/false
  */
  bool Client::closeConnection()
  {
    Host::setActive(false);
    GamestateManager::removePeer(NETWORK_PEER_ID_SERVER);
    return ClientConnection::closeConnection();
  }

  void Client::setDestination(const std::string& serverAddress, unsigned int port)
  {
    ClientConnection::setServerAddress(serverAddress);
    ClientConnection::setPort(port);
  }

  void Client::queuePacket(ENetPacket *packet, int clientID, uint8_t channelID)
  {
    ClientConnection::addPacket(packet, channelID);
  }

  void Client::printRTT()
  {
    orxout(message) << "Round trip time to server is " << ClientConnection::getRTT() << " ms" << endl;
  }

  /**
   * @brief Sends a chat message to the server.
   * @param message message to be sent
   * @param sourceID the ID of the sender
   * @param targetID the ID of the receiver
   */
  void Client::doSendChat(const std::string& message, unsigned int sourceID, unsigned int targetID)
  {
    // send the message to the server
    packet::Chat* packet = new packet::Chat(message, sourceID, targetID);
    packet->send(static_cast<Host*>(this));
  }

  /**
   * @brief Gets called if a packet::Chat packet is received. Calls the parent function which passes the message to the listeners.
   */
  void Client::doReceiveChat(const std::string& message, unsigned int sourceID, unsigned int targetID)
  {
    // call the parent function which passes the message to the listeners
    Host::doReceiveChat(message, sourceID, targetID);
  }

  /**
   * Processes incoming packets, sends a gamestate to the server and does the cleanup
   * @param time
   */
  void Client::update(const Clock& time)
  {
    //this steers our network frequency
    timeSinceLastUpdate_+=time.getDeltaTime();
    if(timeSinceLastUpdate_>=NETWORK_PERIOD)
    {
      timeSinceLastUpdate_ -= static_cast<unsigned int>( timeSinceLastUpdate_ / NETWORK_PERIOD ) * NETWORK_PERIOD;
      if ( isConnected() && isSynched_ )
      {
        orxout(verbose, context::network) << "popping partial gamestate: " << endl;
//         packet::Gamestate *gs = GamestateClient::getGamestate();
        if( GamestateManager::update() )
        {
          std::vector<packet::Gamestate*> gamestates = GamestateManager::getGamestates();
          std::vector<packet::Gamestate*>::iterator it;
          for( it = gamestates.begin(); it != gamestates.end(); ++it )
          {
            (*it)->send( static_cast<Host*>(this) );
          }
        }
        //assert(gs); <--- there might be the case that no data has to be sent, so its commented out now
//         if(gs){
//           orxout(verbose, context::network) << "client tick: sending gs " << gs << endl;
//           if( !gs->send() )
//             orxout(internal_warning, context::network) << "Problem adding partial gamestate to queue" << endl;
//         // gs gets automatically deleted by enet callback
//         }
        FunctionCallManager::sendCalls(static_cast<Host*>(this));
      }
    }
//     sendPackets(); // flush the enet queue

    Connection::processQueue();
    if(GamestateManager::processGamestates())
    {
      FunctionCallManager::processBufferedFunctionCalls();
      if(!isSynched_)
        isSynched_=true;
    }
//     GamestateManager::cleanup();;
//     Connection::sendPackets();

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

  void Client::processPacket(packet::Packet* packet)
  {
    if( packet->isReliable() )
    {
      if( this->getLastReceivedGamestateID(packet->getPeerID()) >= packet->getRequiredGamestateID() )
        packet->process(static_cast<Host*>(this));
      else
        this->packetQueue_.push_back(packet);
    }
    else
      packet->process(static_cast<Host*>(this));
  }
}
