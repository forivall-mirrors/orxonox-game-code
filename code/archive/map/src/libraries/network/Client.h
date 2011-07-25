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
// C++ Interface: Client
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _Client_H__
#define _Client_H__

#include "NetworkPrereqs.h"

#include <string>

#include "util/UtilPrereqs.h"
#include "ClientConnection.h"
#include "GamestateClient.h"
#include "Host.h"

namespace orxonox
{
  /**
  Client *client;
  * The network/Client class
  * This class implements all necessary function for the network communication
  * It is the root class of the network module
  *
  */
  class _NetworkExport Client : public Host, public ClientConnection{
  public:
    Client();
    Client(const std::string& address, int port);
    ~Client();

    bool establishConnection();
    bool closeConnection();
    bool queuePacket(ENetPacket *packet, int clientID);
    bool processChat(const std::string& message, unsigned int playerID);
    virtual bool chat(const std::string& message);
    virtual bool broadcast(const std::string& message) { return false; }
    virtual void printRTT();

    void update(const Clock& time);
  protected:
    virtual void connectionClosed();
  private:
    Client(const Client& copy); // not used
    virtual bool isServer_(){return false;}

    GamestateClient gamestate;
    bool isSynched_;

    bool gameStateFailure_;
    float timeSinceLastUpdate_;
  };


}

#endif /* _Client_H__ */
