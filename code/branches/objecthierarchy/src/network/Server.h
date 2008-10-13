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

#include "Host.h"
#include "GamestateManager.h"

namespace network
{
  const int CLIENTID_SERVER = 0;

  /**
  * This class is the root class of the network module for a server.
  * It implements all functions necessary for a Server
  */
  class _NetworkExport Server : public Host{
  public:
    Server();
    Server(int port);
    Server(int port, std::string bindAddress);
    Server(int port, const char *bindAddress);
    ~Server();

    void open();
    void close();
    bool processChat(std::string message, unsigned int playerID);
    bool queuePacket(ENetPacket *packet, int clientID);
    void tick(float time);
  protected:
    void processQueue();
    void updateGamestate();
  private:
    virtual bool isServer_(){return true;}
    unsigned int shipID(){return 0;}
    unsigned int playerID(){return 0;}

    bool addClient(ENetEvent *event);
    bool createClient(int clientID);
    bool disconnectClient(ENetEvent *event);
    void disconnectClient(int clientID);
    void disconnectClient( ClientInformation *client);
    bool processPacket( ENetPacket *packet, ENetPeer *peer );
    bool sendGameState();
    bool sendObjectDeletes();
    virtual bool chat(std::string message);

    //void processChat( chat *data, int clientId);
    ConnectionManager *connection;
    GamestateManager *gamestates_;


    float timeSinceLastUpdate_;
  };




}

#endif /* _Server_H__ */
