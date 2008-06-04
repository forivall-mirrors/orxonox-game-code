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
// C++ Interface: Server
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _Server_H__
#define _Server_H__

#include "NetworkPrereqs.h"

#include <string>

#include "core/Tickable.h"
#include "PacketManager.h"



namespace network
{
  #define CLIENTID_SERVER 0
  
  /**
  * This class is the root class of the network module for a server.
  * It implements all functions necessary for a Server
  */
  class _NetworkExport Server : public PacketDecoder, public orxonox::Tickable{
  public:
    static Server *createSingleton();
    static Server *createSingleton(int port);
    static Server *createSingleton(int port, std::string bindAddress);
    static Server *createSingleton(int port, const char *bindAddress);
    
    static Server *getSingleton();
    
    void open();
    void close();
    bool sendChat(std::string msg);
    bool sendChat(const char *msg);
    void tick(float time);
  protected:
    void processQueue();
    void updateGamestate();
  private:
    Server();
    Server(int port);
    Server(int port, std::string bindAddress);
    Server(int port, const char *bindAddress);
    
    static Server *instance_;
    
    bool addClient(ENetEvent *event);
    bool createClient(int clientID);
    bool createShip(ClientInformation *client);
    bool disconnectClient(ENetEvent *event);
    void disconnectClient(int clientID);
    void disconnectClient( ClientInformation *client);
    bool sendGameState();
    void processAck( ack *data, int clientID);
    bool processConnectRequest( connectRequest *con, int clientID );
    void processGamestate( GameStateCompressed *data, int clientID);
    void processChat( chat *data, int clientId);
    ConnectionManager *connection;
    GameStateManager *gamestates;
    PacketGenerator packet_gen;

    ClientInformation *clients;
    float timeSinceLastUpdate_;
  };




}

#endif /* _Server_H__ */
