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

#include "Connection.h"

#include <cassert>
#include <deque>
#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time.hpp>

#include "packet/Packet.h"
#include <util/Sleep.h>

namespace orxonox
{
  const boost::posix_time::millisec NETWORK_COMMUNICATION_THREAD_WAIT_TIME(200);
  const unsigned int                NETWORK_DISCONNECT_TIMEOUT = 500;

  Connection::Connection(uint32_t firstPeerID):
    host_(0), bCommunicationThreadRunning_(false), nextPeerID_(firstPeerID)
  {
    enet_initialize();
    atexit(enet_deinitialize);
    this->incomingEventsMutex_ = new boost::mutex;
    this->outgoingEventsMutex_ = new boost::mutex;
//     this->overallMutex_ = new boost::mutex;
  }

  Connection::~Connection()
  {
    delete this->incomingEventsMutex_;
    delete this->outgoingEventsMutex_;
  }

  void Connection::startCommunicationThread()
  {
    this->bCommunicationThreadRunning_ = true;
    this->communicationThread_ = new boost::thread(&Connection::communicationThread, this);
  }
  
  void Connection::stopCommunicationThread()
  {
    this->bCommunicationThreadRunning_ = false;
    if( !this->communicationThread_->timed_join(NETWORK_COMMUNICATION_THREAD_WAIT_TIME) )
    {
      // force thread to stop
      this->communicationThread_->interrupt();
    }
    delete this->communicationThread_;
  }

  void Connection::disconnectPeer(uint32_t peerID)
  {
//     this->overallMutex_->lock();
    outgoingEvent outEvent = { peerID, outgoingEventType::disconnectPeer, 0, 0 };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
//     this->overallMutex_->unlock();
  }
  
  void Connection::disconnectPeers()
  {
    outgoingEvent outEvent = { 0, outgoingEventType::disconnectPeers, 0, 0 };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
  }

  void Connection::addPacket(ENetPacket* packet, uint32_t peerID, uint8_t channelID)
  {
//     this->overallMutex_->lock();
    outgoingEvent outEvent = { peerID, outgoingEventType::sendPacket, packet, channelID };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
//     this->overallMutex_->unlock();
  }
  
  void Connection::broadcastPacket(ENetPacket* packet, uint8_t channelID)
  {
//     this->overallMutex_->lock();
    outgoingEvent outEvent = { 0, outgoingEventType::broadcastPacket, packet, channelID };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
//     this->overallMutex_->unlock();
  }

  
  void Connection::communicationThread()
  {
    ENetEvent event;
    
//     this->overallMutex_->lock();
    while( bCommunicationThreadRunning_ )
    {
      // Receive all pending incoming Events (such as packets, connects and disconnects)
      while( enet_host_check_events( this->host_, &event ) > 0 )
      {
        processIncomingEvent(event);
      }
      
//       this->overallMutex_->unlock();
      msleep(1);
//       this->overallMutex_->lock();
      
      // Send all waiting outgoing packets
      this->outgoingEventsMutex_->lock();
      uint32_t outgoingEventsCount = this->outgoingEvents_.size();
      this->outgoingEventsMutex_->unlock();
      while( outgoingEventsCount > 0 )
      {
//         COUT(0) << "outgoing event" << endl;
        this->outgoingEventsMutex_->lock();
        outgoingEvent outEvent = this->outgoingEvents_.front();
        this->outgoingEvents_.pop_front();
        this->outgoingEventsMutex_->unlock();
        
        processOutgoingEvent(outEvent);
        
        this->outgoingEventsMutex_->lock();
        outgoingEventsCount = this->outgoingEvents_.size();
        this->outgoingEventsMutex_->unlock();
      }
      
      // Wait for incoming events (at most NETWORK_WAIT_TIMEOUT ms)
      if( enet_host_service( this->host_, &event, NETWORK_WAIT_TIMEOUT ) > 0 )
      {
        processIncomingEvent(event);
      }
    }
//     this->overallMutex_->unlock();
  }
  
  void Connection::processIncomingEvent(ENetEvent& event)
  {
    incomingEvent inEvent;
    // preprocess event
    switch( event.type )
    {
      case ENET_EVENT_TYPE_CONNECT:
        inEvent = preprocessConnectEvent(event);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        inEvent = preprocessReceiveEvent(event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        inEvent = preprocessDisconnectEvent(event);
        break;
      case ENET_EVENT_TYPE_NONE:
      default:
        return;
    }
    
    // pushing event to queue
    this->incomingEventsMutex_->lock();
    this->incomingEvents_.push_back(inEvent);
    this->incomingEventsMutex_->unlock();
  }
  
  void Connection::processOutgoingEvent(outgoingEvent& event)
  {
    ENetPeer* peer;
    switch( event.type )
    {
      case outgoingEventType::sendPacket:
        // check whether the peer is still/already in the peer list
        if( this->peerMap_.find(event.peerID) != this->peerMap_.end() )
        {
          peer = this->peerMap_[event.peerID];
          enet_peer_send( peer, event.channelID, event.packet );
        }
        else
        {
          // peer probably already disconnected so just discard packet
          assert(event.peerID<this->nextPeerID_);
          enet_packet_destroy(event.packet);
        }
        break;
      case outgoingEventType::disconnectPeer:
        if( this->peerMap_.find(event.peerID) != this->peerMap_.end() )
        {
          peer = this->peerMap_[event.peerID];
          enet_peer_disconnect(peer, 0);
        }
        else
        {
          // peer probably already disconnected so just discard disconnect event
          assert(event.peerID<this->nextPeerID_);
        }
        break;
      case outgoingEventType::disconnectPeers:
        disconnectPeersInternal();
        break;
      case outgoingEventType::broadcastPacket:
        enet_host_broadcast( this->host_, event.channelID, event.packet );
        break;
      default:
        assert(0);
    }
  }


  void Connection::disconnectPeersInternal()
  {
    std::map<uint32_t, ENetPeer*>::iterator it;
    for( it=this->peerMap_.begin(); it!=this->peerMap_.end(); ++it )
    {
      enet_peer_disconnect(it->second, 0);
    }
    uint32_t iterations = NETWORK_DISCONNECT_TIMEOUT/NETWORK_WAIT_TIMEOUT;
    uint32_t i = 0;
    while( this->peerMap_.size() && i++ < iterations )
    {
      ENetEvent event;
      if( enet_host_service( this->host_, &event, NETWORK_WAIT_TIMEOUT ) > 0 )
      {
        processIncomingEvent(event);
      }
    }
  }

  void Connection::processQueue()
  {
    incomingEvent inEvent;

    this->incomingEventsMutex_->lock();
    uint32_t incomingEventsCount = this->incomingEvents_.size();
    this->incomingEventsMutex_->unlock();
    while( incomingEventsCount > 0 )
    {
      // pop event from queue
      this->incomingEventsMutex_->lock();
      inEvent = this->incomingEvents_.front();
      this->incomingEvents_.pop_front();
      this->incomingEventsMutex_->unlock();
      
      // process event
      switch( inEvent.type )
      {
        case incomingEventType::peerConnect:
          addPeer(inEvent.peerID);
          break;
        case incomingEventType::peerDisconnect:
          removePeer(inEvent.peerID);
          break;
        case incomingEventType::receivePacket:
          processPacket(inEvent.packet);
          break;
        default:
          break;
      }
      
      // check whether there are still events in the queue
      this->incomingEventsMutex_->lock();
      incomingEventsCount = this->incomingEvents_.size();
      this->incomingEventsMutex_->unlock();
    }
  }
  
  void Connection::waitOutgoingQueue()
  {
    uint32_t outgoingEventsCount;
    this->outgoingEventsMutex_->lock();
    outgoingEventsCount = this->outgoingEvents_.size();
    this->outgoingEventsMutex_->unlock();
    while( outgoingEventsCount )
    {
      msleep(1);
      this->outgoingEventsMutex_->lock();
      outgoingEventsCount = this->outgoingEvents_.size();
      this->outgoingEventsMutex_->unlock();
    }
  }


  incomingEvent Connection::preprocessConnectEvent(ENetEvent& event)
  {
    // make sure this peer doesn't exist
    assert( this->peerMap_.find(this->nextPeerID_) == this->peerMap_.end() );
    assert( this->peerIDMap_.find(event.peer) == this->peerIDMap_.end() );
    
    // give peer a new id and increase peerID for next peer
    uint32_t peerID = this->nextPeerID_;
    ++this->nextPeerID_;
    
    // add peer/peerID into peerMap_ and peerIDMap_
    this->peerMap_[peerID] = event.peer;
    this->peerIDMap_[event.peer] = peerID;
    
    // create new peerEvent and return it
    incomingEvent inEvent = { peerID, incomingEventType::peerConnect, 0 };
    return inEvent;
  }
  
  incomingEvent Connection::preprocessDisconnectEvent(ENetEvent& event)
  {
    // assert that the peer exists and get peerID
    assert( this->peerIDMap_.find(event.peer) != this->peerIDMap_.end() );
    uint32_t peerID = this->peerIDMap_[event.peer];
    
    // remove peer/peerID from maps
    this->peerIDMap_.erase(this->peerIDMap_.find(event.peer));
    this->peerMap_.erase(this->peerMap_.find(peerID));
    
    // create new peerEvent and return it
    incomingEvent inEvent = { peerID, incomingEventType::peerDisconnect, 0 };
    return inEvent;
  }
  
  incomingEvent Connection::preprocessReceiveEvent(ENetEvent& event)
  {
    // assert that the peer exists and get peerID
    assert( this->peerIDMap_.find(event.peer) != this->peerIDMap_.end() );
    uint32_t peerID = this->peerIDMap_[event.peer];
    
    // create new Packet from ENetPacket
    packet::Packet* p = packet::Packet::createPacket(event.packet, peerID);
    
    // create new peerEvent and return it
    incomingEvent inEvent = { peerID, incomingEventType::receivePacket, p };
    return inEvent;
  }

  
  void Connection::enableCompression()
  {
    enet_host_compress_with_range_coder( this->host_ );
  }


}
