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
#include <map>
#include <enet/enet.h>
#include <boost/concept_check.hpp>

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
  
  namespace incomingEventType
  {
    enum Value
    {
      receivePacket   = 1,  // incoming packet
      peerConnect     = 2,  // incoming connect request
      peerDisconnect  = 3   // incoming disconnect request
    };
    
  }
  
  namespace outgoingEventType
  {
    enum Value
    {
      sendPacket      = 1,  // outgoing packet
      broadcastPacket = 2,  // outgoing broadcast packet
      disconnectPeer  = 3,  // outgoing disconnect request
      disconnectPeers = 4   // outgoing disconnect request
    };
    
  }
  
  struct _NetworkExport incomingEvent
  {
    uint32_t                  peerID;
    incomingEventType::Value  type;
    packet::Packet*           packet;
  };
  
  struct _NetworkExport outgoingEvent
  {
    uint32_t                  peerID;
    outgoingEventType::Value  type;
    ENetPacket*               packet;
    ENetChannelID             channelID;
  };
  
  class _NetworkExport Connection
  {
  public:
    virtual ~Connection();

  protected:
    Connection(uint32_t firstPeerID = NETWORK_PEER_ID_SERVER+1);
    
    void startCommunicationThread();
    void stopCommunicationThread();
    
    void addPacket(ENetPacket *packet, uint32_t peerID, uint8_t channelID);
    void broadcastPacket(ENetPacket* packet, uint8_t channelID);
    void disconnectPeer(uint32_t peerID);
    void disconnectPeers();
    
    void enableCompression();

    void processQueue();
    void waitOutgoingQueue();     // wait for the outgoing queue to become empty (everything processed by communication thread)
    virtual void addPeer(uint32_t peerID)=0;
    virtual void removePeer(uint32_t peerID)=0;
    virtual void processPacket( packet::Packet* packet)=0;
    
    incomingEvent preprocessConnectEvent(ENetEvent& event);
    incomingEvent preprocessDisconnectEvent(ENetEvent& event);
    incomingEvent preprocessReceiveEvent(ENetEvent& event);
    
    void processIncomingEvent(ENetEvent& event);
    void processOutgoingEvent(outgoingEvent& event);
    
    void disconnectPeersInternal();

    ENetHost*                     host_;
  private:
    void communicationThread();
    
    boost::thread*                communicationThread_;
    bool                          bCommunicationThreadRunning_;
    ENetAddress*                  bindAddress_;
    std::deque<incomingEvent>     incomingEvents_;
    std::deque<outgoingEvent>     outgoingEvents_;
    boost::mutex*                 incomingEventsMutex_;
    boost::mutex*                 outgoingEventsMutex_;
    boost::mutex*                 overallMutex_;
    std::map<uint32_t, ENetPeer*> peerMap_;
    std::map<ENetPeer*, uint32_t> peerIDMap_;
    uint32_t                      nextPeerID_;

  };

}

#endif /* _Connection_H__ */
