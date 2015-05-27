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

#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h>
#include <cassert>
#include <string>

#include "util/Clock.h"
#include "util/Output.h"
#include "core/command/Executor.h"
#include "packet/Chat.h"
#include "packet/ClassID.h"
#include "packet/DeleteObjects.h"
#include "packet/FunctionIDs.h"
#include "packet/Gamestate.h"
#include "packet/Welcome.h"
// #include "ClientInformation.h"
#include "FunctionCallManager.h"
#include "GamestateManager.h"

namespace orxonox
{
  const unsigned int MAX_FAILURES = 20;

  /**
  * Constructor for default values (bindaddress is set to ENET_HOST_ANY
  *
  */
  Server::Server()
  {
    this->timeSinceLastUpdate_=0;
  }

  Server::Server(int port)
  {
    this->setPort( port );
    this->timeSinceLastUpdate_=0;
  }
/*
  Server::Server(int port, const std::string name)
  {
    this->setPort( port );
    this->timeSinceLastUpdate_=0;
    this->serverName_=name;
  }*/
  /**
  * Constructor
  * @param port Port to listen on
  * @param bindAddress Address to listen on
  */
  Server::Server(int port, const std::string& bindAddress)
  {
    this->setPort( port );
    this->setBindAddress( bindAddress );
    this->timeSinceLastUpdate_=0;
  }

  /**
  * @brief Destructor
  */
  Server::~Server()
  {
  }

  /**
  * This function opens the server by creating the listener thread
  */
  void Server::open()
  {
    Host::setActive(true);
    orxout(verbose, context::network) << "opening server" << endl;
    this->openListener();

    /* make discoverable on LAN */
    LANDiscoverable::setActivity(true);
    LANDiscoverable::updateClientNumber(0);

    /* make discoverable on WAN */
    WANDiscoverable::setActivity(true);
    WANDiscoverable::updateClientNumber(0);

    /* done */
    return;
  }

  /**
  * This function closes the server
  */
  void Server::close()
  {
    Host::setActive(false);
    orxout(verbose, context::network) << "closing server" << endl;
    this->disconnectClients();
    this->closeListener();

    /* tell master server we're closing */
    orxout(internal_info, context::network) << "disconnecting." << endl;
    WANDiscoverable::setActivity(false);
    orxout(internal_info, context::network) << "disconnecting done" << endl;

    LANDiscoverable::setActivity(false);
    return;
  }

  /**
  * Run this function once every tick
  * calls processQueue and updateGamestate
  * @param time time since last tick
  */
  void Server::update(const Clock& time)
  {
    // receive incoming packets
    Connection::processQueue();

    // receive and process incoming discovery packets
    LANDiscoverable::update();

    if ( GamestateManager::hasPeers() )
    {
      // process incoming gamestates
      GamestateManager::processGamestates();
      FunctionCallManager::processBufferedFunctionCalls();

      // send function calls to clients
      FunctionCallManager::sendCalls( static_cast<Host*>(this) );

      //this steers our network frequency
      timeSinceLastUpdate_+=time.getDeltaTime();
      if(timeSinceLastUpdate_>=NETWORK_PERIOD)
      {
        timeSinceLastUpdate_ -= static_cast<unsigned int>( timeSinceLastUpdate_ / NETWORK_PERIOD ) * NETWORK_PERIOD;
        updateGamestate();
      }
//       sendPackets(); // flush the enet queue
    }
  }

  void Server::queuePacket(ENetPacket *packet, int clientID, uint8_t channelID)
  {
    ServerConnection::addPacket(packet, clientID, channelID);
  }

  /**
   * @brief: returns ping time to client in milliseconds
   */
  unsigned int Server::getRTT(unsigned int clientID)
  {
//     assert(ClientInformation::findClient(clientID));
//     return ClientInformation::findClient(clientID)->getRTT();
    // TODO: reimplement
    return 0;
  }

  void Server::printRTT()
  {
//     for( ClientInformation* temp=ClientInformation::getBegin(); temp!=0; temp=temp->next() )
//       orxout(message) << "Round trip time to client with ID: " << temp->getID() << " is " << temp->getRTT() << " ms" << endl;
  }

  /**
   * @brief: return packet loss ratio to client (scales from 0 to 1)
   */
  float Server::getPacketLoss(unsigned int clientID)
  {
//     assert(ClientInformation::findClient(clientID));
//     return ClientInformation::findClient(clientID)->getPacketLoss();
    return 0.;
  }

  /**
  * takes a new snapshot of the gamestate and sends it to the clients
  */
  void Server::updateGamestate()
  {
    if( this->clientIDs_.size()==0 )
      //no client connected
      return;
    GamestateManager::update();
//     orxout(verbose_more, context::network) << "Server: one gamestate update complete, goig to sendGameState" << endl;
    //orxout(verbose_more, context::network) << "updated gamestate, sending it" << endl;
    //if(clients->getGamestateID()!=GAMESTATEID_INITIAL)
    sendGameStates();
    sendObjectDeletes();
//     orxout(verbose_more, context::network) << "Server: one sendGameState turn complete, repeat in next tick" << endl;
    //orxout(verbose_more, context::network) << "sent gamestate" << endl;
  }

  /**
  * sends the current gamestate to all peers
  */
  bool Server::sendGameStates()
  {
    std::vector<packet::Gamestate*> gamestates = GamestateManager::getGamestates();
    std::vector<packet::Gamestate*>::iterator it;
    for( it = gamestates.begin(); it != gamestates.end(); ++it )
    {
      (*it)->send(static_cast<Host*>(this));
    }
    return true;
  }


  bool Server::sendObjectDeletes()
  {
//     ClientInformation *temp = ClientInformation::getBegin();
//     if( temp == NULL )
      //no client connected
    if( this->clientIDs_.size()==0 )
      return true;
    packet::DeleteObjects *del = new packet::DeleteObjects();
    if(!del->fetchIDs())
    {
      delete del;
      return true;  //everything ok (no deletes this tick)
    }
//     orxout(verbose, context::network) << "sending DeleteObjects" << endl;
//     while(temp != NULL){
//       if( !(temp->getSynched()) )
//       {
//         orxout(verbose_more, context::network) << "Server: not sending gamestate" << endl;
//         temp=temp->next();
//         continue;
//       }
//       int cid = temp->getID(); //get client id
//       packet::DeleteObjects *cd = new packet::DeleteObjects(*del);
//       assert(cd);
    del->setPeerID(NETWORK_PEER_ID_BROADCAST);
    if ( !del->send( static_cast<Host*>(this) ) )
      orxout(internal_warning, context::network) << "Server: could not broadcast deleteObjects packet" << endl;
//       temp=temp->next();
      // gs gets automatically deleted by enet callback
//     }
//     delete del;
    return true;
  }


  void Server::addPeer(uint32_t peerID)
  {
//     static unsigned int newid=1;
//
//     orxout(internal_info, context::network) << "Server: adding client" << endl;
//     ClientInformation *temp = ClientInformation::insertBack(new ClientInformation);
//     if(!temp)
//     {
//       orxout(internal_warning, context::network) << "Server: could not add client" << endl;
//     }
//     temp->setID(newid);
//     temp->setPeer(event->peer);

    // inform all the listeners
    this->clientIDs_.push_back(peerID);
    WANDiscoverable::updateClientNumber(this->clientIDs_.size());
    LANDiscoverable::updateClientNumber(this->clientIDs_.size());

    ClientConnectionListener::broadcastClientConnected(peerID);
    GamestateManager::addPeer(peerID);

//     ++newid;

    orxout(internal_info, context::network) << "Server: added client id: " << peerID << endl;

    createClient(peerID);
}

  void Server::removePeer(uint32_t peerID)
  {
    orxout(verbose, context::network) << "removing client from list" << endl;
//     ClientInformation *client = ClientInformation::findClient(&event->peer->address);
//     if(!client)
//       return;
//     else
//     {
  std::vector<uint32_t>::iterator it;
  for( it=this->clientIDs_.begin(); it!=this->clientIDs_.end(); ++it )
  {
    if( *it == peerID )
    {
      this->clientIDs_.erase(it);
      break;
    }
  }
  WANDiscoverable::updateClientNumber(this->clientIDs_.size());
  LANDiscoverable::updateClientNumber(this->clientIDs_.size());

  ClientConnectionListener::broadcastClientDisconnected(peerID);
  GamestateManager::removePeer(peerID);
      //ServerConnection::disconnectClient( client );
      //ClientConnectionListener::broadcastClientDisconnected( client->getID() ); //this is done in ClientInformation now
//       delete client;
//     }
  }

  void Server::processPacket(packet::Packet* packet)
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


  bool Server::createClient(int clientID)
  {
//     ClientInformation *temp = ClientInformation::findClient(clientID);
//     if(!temp)
//     {
//       orxout(internal_error, context::network) << "Server. could not create client with id: " << clientID << endl;
//       return false;
//     }
//     orxout(verbose, context::network) << "Con.Man: creating client id: " << temp->getID() << endl;

    // synchronise class ids
    syncClassid(clientID);

    // now synchronise functionIDs
    packet::FunctionIDs *fIDs = new packet::FunctionIDs();
    fIDs->setPeerID(clientID);
    bool b = fIDs->send( static_cast<Host*>(this) );
    assert(b);

//     temp->setSynched(true);
    GamestateManager::setSynched(clientID);

    orxout(verbose, context::network) << "sending welcome" << endl;
    packet::Welcome *w = new packet::Welcome(clientID);
    w->setPeerID(clientID);
    b = w->send( static_cast<Host*>(this) );
    assert(b);
//     packet::Gamestate *g = new packet::Gamestate();
//     g->setPeerID(clientID);
//     b = g->collectData(0,packet::GAMESTATE_MODE_SERVER);
//     assert(b);
//     if(!b)
//       return false; //no data for the client
// //     b = g->compressData();
// //     assert(b);
//     b = g->send( static_cast<Host*>(this) );
//     assert(b);
    return true;
  }

  void Server::disconnectClient( uint32_t clientID )
  {
    ServerConnection::disconnectClient( clientID );
    GamestateManager::removePeer( clientID );
    // inform all the listeners
    // ClientConnectionListener::broadcastClientDisconnected(client->getID()); // this is done in ClientInformation now
  }

  /**
   * @brief Sends a chat message to the given target ID.
   * @param message message to be sent
   * @param sourceID the ID of the sender
   * @param targetID the ID of the receiver
   */
  void Server::doSendChat(const std::string& message, unsigned int sourceID, unsigned int targetID)
  {
    // check if the target exists. just ignore the message otherwise
    if (!this->isValidTarget(targetID)) // TODO: remove this if an invalid clientIDs don't trigger assertions anymore
      return;

    // send the message to the target
    packet::Chat* packet = new packet::Chat(message, sourceID, targetID);
    packet->setPeerID(targetID);
    packet->send( static_cast<Host*>(this) );

    // if the target is (or includes) this host as well, call the parent function which passes the message to the listeners
    if (targetID == NETWORK_PEER_ID_BROADCAST || targetID == Host::getPlayerID())
      Host::doReceiveChat(message, sourceID, targetID);
  }

  /**
   * @brief Gets called if a packet::Chat packet is received. Forwards the packet to the target
   * and calls the parent function if necessary.
   */
  void Server::doReceiveChat(const std::string& message, unsigned int sourceID, unsigned int targetID)
  {
      this->doSendChat(message, sourceID, targetID);
  }

  /**
   * @brief Returns true if the target ID is in the list of clients (or if it
   * corresponds to the broadcast or the server ID).
   */
  bool Server::isValidTarget(unsigned int targetID)
  {
    if (targetID == NETWORK_PEER_ID_BROADCAST || targetID == NETWORK_PEER_ID_SERVER)
      return true;

    std::vector<uint32_t>::iterator it;
    for( it=this->clientIDs_.begin(); it!=this->clientIDs_.end(); ++it )
      if( *it == targetID )
        return true;

    return false;
  }

  void Server::syncClassid(unsigned int clientID)
  {
    int failures=0;
    packet::ClassID *classid = new packet::ClassID();
    classid->setPeerID(clientID);
    while(!classid->send( static_cast<Host*>(this) ) && failures < 10){
      failures++;
    }
    assert(failures<10);
    orxout(verbose, context::network) << "syncClassid:\tall synchClassID packets have been sent" << endl;
  }

}
