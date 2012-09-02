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
#include "util/Debug.h"
#include "core/ObjectList.h"
#include "core/command/Executor.h"
#include "packet/Chat.h"
#include "packet/ClassID.h"
#include "packet/DeleteObjects.h"
#include "packet/FunctionIDs.h"
#include "packet/Gamestate.h"
#include "packet/Welcome.h"
#include "ChatListener.h"
// #include "ClientInformation.h"
#include "FunctionCallManager.h"
#include "GamestateManager.h"
#include "WANDiscovery.h"

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


  /** helper that connects to the master server */
  void Server::helper_ConnectToMasterserver()
  {
//     WANDiscovery::getInstance().msc.sendRequest( MSPROTO_GAME_SERVER " " 
//       MSPROTO_REGISTER_SERVER );
  }

  /**
  * This function opens the server by creating the listener thread
  */
  void Server::open()
  {
    Host::setActive(true);
    COUT(4) << "opening server" << endl;
    this->openListener();
    
    /* make discoverable on LAN */
    LANDiscoverable::setActivity(true);

    /* make discoverable on WAN */
    WANDiscoverable::setActivity(true);
    /* TODO this needs to be optional, we need a switch from the UI to
     * enable/disable this 
     */
//     helper_ConnectToMasterserver();

    /* done */
    return;
  }

  /**
  * This function closes the server
  */
  void Server::close()
  {
    Host::setActive(false);
    COUT(4) << "closing server" << endl;
    this->disconnectClients();
    this->closeListener();

    /* tell master server we're closing */
    COUT(2) << "disconnecting." << endl;
    WANDiscoverable::setActivity(false);    
    COUT(2) << "disconnecting done" << endl;

    LANDiscoverable::setActivity(false);
    return;
  }

  bool Server::processChat(const std::string& message, unsigned int playerID)
  {
//     ClientInformation *temp = ClientInformation::getBegin();
    packet::Chat *chat;
//     while(temp){
    chat = new packet::Chat(message, playerID);
    chat->setPeerID(NETWORK_PEER_ID_BROADCAST);
    chat->send( static_cast<Host*>(this) );
//         COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
//       temp = temp->next();
//     }
//    COUT(1) << "Player " << playerID << ": " << message << std::endl;
    return true;
  }


  /* handle incoming data */
  int rephandler( char *addr, ENetEvent *ev )
  { 
    /* reply to pings */
    if( !strncmp( (char *)ev->packet->data, MSPROTO_PING_GAMESERVER, 
      MSPROTO_PING_GAMESERVER_LEN ) )
      //this->msc.sendRequest( MSPROTO_ACK );
      /* NOTE implement this after pollForReply
       * reimplementation 
       */
      return 0;

    /* done handling, return all ok code 0 */
    return 0;
  }

  void Server::helper_HandleMasterServerRequests()
  { 
    /* poll the master server for replies and see whether something 
     * has to be done or changed.
     */
    //WANDiscovery::getInstance().msc.pollForReply( rhandler, 10 );
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
    
    // receive and process requests from master server
    /* todo */
    //helper_HandleMasterServerRequests();

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
//       COUT(0) << "Round trip time to client with ID: " << temp->getID() << " is " << temp->getRTT() << " ms" << endl;
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
//     COUT(5) << "Server: one gamestate update complete, goig to sendGameState" << std::endl;
    //std::cout << "updated gamestate, sending it" << std::endl;
    //if(clients->getGamestateID()!=GAMESTATEID_INITIAL)
    sendGameStates();
    sendObjectDeletes();
//     COUT(5) << "Server: one sendGameState turn complete, repeat in next tick" << std::endl;
    //std::cout << "sent gamestate" << std::endl;
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
//     COUT(3) << "sending DeleteObjects" << std::endl;
//     while(temp != NULL){
//       if( !(temp->getSynched()) )
//       {
//         COUT(5) << "Server: not sending gamestate" << std::endl;
//         temp=temp->next();
//         continue;
//       }
//       int cid = temp->getID(); //get client id
//       packet::DeleteObjects *cd = new packet::DeleteObjects(*del);
//       assert(cd);
    del->setPeerID(NETWORK_PEER_ID_BROADCAST);
    if ( !del->send( static_cast<Host*>(this) ) )
      COUT(3) << "Server: could not broadcast deleteObjects packet" << std::endl;
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
//     COUT(2) << "Server: adding client" << std::endl;
//     ClientInformation *temp = ClientInformation::insertBack(new ClientInformation);
//     if(!temp)
//     {
//       COUT(2) << "Server: could not add client" << std::endl;
//     }
//     temp->setID(newid);
//     temp->setPeer(event->peer);

    // inform all the listeners
    this->clientIDs_.push_back(peerID);
    ClientConnectionListener::broadcastClientConnected(peerID);
    GamestateManager::addPeer(peerID);

//     ++newid;

    COUT(3) << "Server: added client id: " << peerID << std::endl;
    createClient(peerID);
}

  void Server::removePeer(uint32_t peerID)
  {
    COUT(4) << "removing client from list" << std::endl;
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
//       COUT(2) << "Server. could not create client with id: " << clientID << std::endl;
//       return false;
//     }
//     COUT(4) << "Con.Man: creating client id: " << temp->getID() << std::endl;

    // synchronise class ids
    syncClassid(clientID);

    // now synchronise functionIDs
    packet::FunctionIDs *fIDs = new packet::FunctionIDs();
    fIDs->setPeerID(clientID);
    bool b = fIDs->send( static_cast<Host*>(this) );
    assert(b);

//     temp->setSynched(true);
    GamestateManager::setSynched(clientID);
    
    COUT(4) << "sending welcome" << std::endl;
    packet::Welcome *w = new packet::Welcome(clientID, OBJECTID_UNKNOWN);
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

  bool Server::chat(const std::string& message)
  {
      return this->sendChat(message, Host::getPlayerID());
  }

  bool Server::broadcast(const std::string& message)
  {
      return this->sendChat(message, NETWORK_PEER_ID_BROADCAST);
  }

  bool Server::sendChat(const std::string& message, unsigned int clientID)
  {
//     ClientInformation *temp = ClientInformation::getBegin();
    packet::Chat *chat;
//     while(temp)
    {
      chat = new packet::Chat(message, clientID);
      chat->setPeerID(NETWORK_PEER_ID_BROADCAST);
      chat->send( static_cast<Host*>(this) );
//         COUT(3) << "could not send Chat message to client ID: " << temp->getID() << std::endl;
//       temp = temp->next();
    }
//    COUT(1) << "Player " << Host::getPlayerID() << ": " << message << std::endl;
    for (ObjectList<ChatListener>::iterator it = ObjectList<ChatListener>::begin(); it != ObjectList<ChatListener>::end(); ++it)
      it->incomingChat(message, clientID);

    return true;
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
    COUT(4) << "syncClassid:\tall synchClassID packets have been sent" << std::endl;
  }

}
