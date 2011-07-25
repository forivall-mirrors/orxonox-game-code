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
// C++ Interface: ServerConnection
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _ServerConnection_H__
#define _ServerConnection_H__

#include "NetworkPrereqs.h"
#include "Connection.h"

namespace orxonox
{

  class _NetworkExport ServerConnection : public Connection{
  public:
    ~ServerConnection();

    void setBindAddress( const std::string& bindAddress );
    void setPort( unsigned int port );

    bool openListener();
    bool closeListener();
    static bool addPacket(ENetPacket *packet, unsigned int ID);
    static bool addPacketAll(ENetPacket *packet);
    virtual void disconnectClient(ClientInformation *client);
    void disconnectClient(int clientID);
  protected:
    ServerConnection();
    void disconnectClients();

  private:
    int getClientID(ENetPeer* peer);
    int getClientID(ENetAddress* address);
    ENetPeer* getClientPeer(int clientID);

    ENetAddress* bindAddress_;

    bool bListening_;

  };

}

#endif /* _ServerConnection_H__ */
