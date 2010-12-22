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

#include <deque>
#include <enet/enet.h>

namespace boost
{
  class thread;
  class mutex;
}

namespace orxonox
{
  const unsigned int NETWORK_PORT                   = 55556;
  const unsigned int NETWORK_MAX_CONNECTIONS        = 50;
  const unsigned int NETWORK_WAIT_TIMEOUT           = 1;
  const unsigned int NETWORK_MAX_QUEUE_PROCESS_TIME = 5;
  
  namespace outgoingEventType
  {
    enum Value
    {
      sendPacket      = 1,
      disconnectPeer  = 2,
      broadcastPacket = 3
    };
    
  }
  
  struct _NetworkExport outgoingEvent
  {
    ENetPeer*                 peer;
    outgoingEventType::Value  type;
    ENetPacket*               packet;
    ENetChannelID             channelID;
  };
  
  class _NetworkExport Connection
  {
  public:
    virtual ~Connection();

    void addPacket(ENetPacket *packet, ENetPeer *peer, uint8_t channelID);
    void broadcastPacket(ENetPacket* packet, uint8_t channelID);
//     ENetHost* getHost(){ return this->host_; }

  protected:
    Connection();
//     static Connection* getInstance(){ return Connection::instance_; }

//     int service(ENetEvent* event);
    void startCommunicationThread();
    void stopCommunicationThread();
    void communicationThread();
    virtual void disconnectPeer(ENetPeer *peer);
    
    void enableCompression();

    void processQueue();
    virtual void addPeer(ENetEvent* event)=0;
    virtual void removePeer(ENetEvent* event)=0;
    virtual void processPacket( packet::Packet* packet)=0;
    virtual packet::Packet* createPacket(ENetEvent* event);

    ENetHost*                   host_;
  private:
    boost::thread*              communicationThread_;
    bool                        bCommunicationThreadRunning_;
    ENetAddress*                bindAddress_;
    std::deque<ENetEvent>       incomingEvents_;
    std::deque<outgoingEvent>   outgoingEvents_;
    boost::mutex*               incomingEventsMutex_;
    boost::mutex*               outgoingEventsMutex_;

//     static Connection *instance_;

  };

}

#endif /* _Connection_H__ */
