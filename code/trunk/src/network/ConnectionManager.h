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
// C++ Interface: ConnectionManager
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _ConnectionManager_H__
#define _ConnectionManager_H__

#include "NetworkPrereqs.h"

#include <string>
#include <map>
// enet library for networking support
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX // required to stop windows.h screwing up std::min definition
#include <enet/enet.h>
#include <boost/thread/recursive_mutex.hpp>

#include "PacketBuffer.h"
#include "packet/Packet.h"

namespace boost { class thread; }

namespace std
{
  bool operator<(ENetAddress a, ENetAddress b);
}

namespace orxonox
{
    const int NETWORK_PORT = 55556;
    const int NETWORK_MAX_CONNECTIONS = 50;
    const int NETWORK_WAIT_TIMEOUT = 1;
    const int NETWORK_DEFAULT_CHANNEL = 0;

  struct _NetworkExport ClientList{
    ENetEvent *event;
    int ID;
    ClientList *next;
  };

  class _NetworkExport ConnectionManager{
    public:
    static boost::recursive_mutex enet_mutex;
    ConnectionManager();
    ConnectionManager(int port);
    ConnectionManager(int port, const char *address);
    ConnectionManager(int port, const std::string& address);
    ~ConnectionManager();
    ENetEvent *getEvent();
    bool queueEmpty();
    void createListener();
    bool quitListener();
    static bool addPacket(ENetPacket *packet, ENetPeer *peer);
    static bool addPacket(ENetPacket *packet, int ID);
    static bool addPacketAll(ENetPacket *packet);
    bool sendPackets();
    void disconnectClient(ClientInformation *client);
    void syncClassid(unsigned int clientID);

  private:
    bool processData(ENetEvent *event);
    void receiverThread();
    void disconnectClients();
    int getClientID(ENetPeer peer);
    int getClientID(ENetAddress address);
    ENetPeer *getClientPeer(int clientID);
    PacketBuffer buffer;

    ENetHost *server;
    ENetAddress bindAddress;

    bool quit; // quit-variable (communication with threads)

    boost::thread *receiverThread_;
    static ConnectionManager *instance_;

  };

}

#endif /* _ConnectionManager_H__ */
