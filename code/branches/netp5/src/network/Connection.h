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
// C++ Interface: Connection
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _Connection_H__
#define _Connection_H__

#include "NetworkPrereqs.h"

#include <string>
#include <map>
#include <enet/enet.h>

#include "packet/Packet.h"

namespace orxonox
{
    const int NETWORK_PORT = 55556;
    const int NETWORK_MAX_CONNECTIONS = 50;
    const int NETWORK_WAIT_TIMEOUT = 1;
    const int NETWORK_DEFAULT_CHANNEL = 0;

//   struct _NetworkExport ClientList{
//     ENetEvent *event;
//     int ID;
//     ClientList *next;
//   };

  class _NetworkExport Connection{
  public:
    ~Connection();
    
    static bool addPacket(ENetPacket *packet, ENetPeer *peer);
    bool sendPackets();
    ENetHost* getHost(){ return this->host_; }

  protected:
    Connection();
    static Connection* getInstance(){ return Connection::instance_; }
    
    int service(ENetEvent* event){ return enet_host_service( this->host_, event, NETWORK_WAIT_TIMEOUT ); }
    void disconnectPeer(ENetPeer *peer){ enet_peer_disconnect(peer, 0); }
    
    void processQueue();
    virtual void addClient(ENetEvent* event)=0;
    virtual void disconnectClient(ENetEvent* event)=0;
    virtual bool processPacket(ENetEvent* event){ packet::Packet *p = packet::Packet::createPacket(event->packet, event->peer); return p->process(); }
    
    ENetHost *host_;
  private:
    ENetAddress *bindAddress_;

    static Connection *instance_;

  };

}

#endif /* _Connection_H__ */
