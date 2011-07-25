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

#ifndef _ClientConnection_H__
#define _ClientConnection_H__

#include "NetworkPrereqs.h"
#include "Connection.h"

namespace orxonox
{

  class _NetworkExport ClientConnection: public Connection{
  public:
    ClientConnection();
    virtual ~ClientConnection();

    void setServerAddress( const std::string& serverAddress );
    void setPort( unsigned int port );

    ENetEvent *getEvent();
    // check wheter the packet queue is empty
    bool queueEmpty();
    // create a new listener thread
    virtual bool establishConnection();
    virtual bool closeConnection();
    // add a packet to queue for the server
    bool addPacket(ENetPacket *packet);
    inline bool isConnected(){ return this->established_; }
  protected:
    virtual void connectionClosed()=0;
    uint32_t getRTT();
  private:
    virtual void addPeer(ENetEvent* event);
    virtual void removePeer(ENetEvent* event);

    bool disconnectConnection();
    // enet stuff
    ENetAddress *serverAddress_;
    bool established_;
    // clientlist
    ENetPeer *server_;
};


}

#endif /* _ClientConnection_H__ */
