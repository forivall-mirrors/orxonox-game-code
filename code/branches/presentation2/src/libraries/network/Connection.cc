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

namespace orxonox
{
  const boost::posix_time::millisec NETWORK_COMMUNICATION_THREAD_WAIT_TIME(20);

  Connection::Connection():
    host_(0), bCommunicationThreadRunning_(false)
  {
    enet_initialize();
    atexit(enet_deinitialize);
    this->incomingEventsMutex_ = new boost::mutex;
    this->outgoingEventsMutex_ = new boost::mutex;
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


//   int Connection::service(ENetEvent* event) {
//     return enet_host_service( this->host_, event, NETWORK_WAIT_TIMEOUT );
//   }

  void Connection::disconnectPeer(ENetPeer *peer)
  {
    assert(peer);
    outgoingEvent outEvent = { peer, outgoingEventType::disconnectPeer, (ENetPacket*)10, 15 };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
  }

  void Connection::addPacket(ENetPacket *packet, ENetPeer *peer, uint8_t channelID)
  {
    assert(peer);
    outgoingEvent outEvent = { peer, outgoingEventType::sendPacket, packet, channelID };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
  }
  
  void Connection::broadcastPacket(ENetPacket* packet, uint8_t channelID)
  {
    outgoingEvent outEvent = { (ENetPeer*)15, outgoingEventType::broadcastPacket, packet, channelID };
    
    this->outgoingEventsMutex_->lock();
    this->outgoingEvents_.push_back(outEvent);
    this->outgoingEventsMutex_->unlock();
  }

  
  void Connection::communicationThread()
  {
    COUT(0) << "starting communication thread" << endl;
    ENetEvent event;
    
    while( bCommunicationThreadRunning_ )
    {
      // Receive all pending incoming Events (such as packets, connects and disconnects)
      while( enet_host_check_events( this->host_, &event ) > 0 )
      {
//         COUT(0) << "incoming event" << endl;
        // received an event
        this->incomingEventsMutex_->lock();
        this->incomingEvents_.push_back(event);
        this->incomingEventsMutex_->unlock();
      }
      
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
        
        switch( outEvent.type )
        {
          case outgoingEventType::sendPacket:
            enet_peer_send( outEvent.peer, outEvent.channelID, outEvent.packet );
            break;
          case outgoingEventType::disconnectPeer:
            enet_peer_disconnect(outEvent.peer, 0);
            break;
          case outgoingEventType::broadcastPacket:
            enet_host_broadcast( this->host_, outEvent.channelID, outEvent.packet );
            break;
          default:
            assert(0);
        }
        this->outgoingEventsMutex_->lock();
        outgoingEventsCount = this->outgoingEvents_.size();
        this->outgoingEventsMutex_->unlock();
      }
      
      // Wait for incoming events (at most NETWORK_WAIT_TIMEOUT ms)
      if( enet_host_service( this->host_, &event, NETWORK_WAIT_TIMEOUT ) > 0 )
      {
//         COUT(0) << "incoming event after wait" << endl;
        //received an event
        this->incomingEventsMutex_->lock();
        this->incomingEvents_.push_back(event);
        this->incomingEventsMutex_->unlock();
      }
    }
  }

  void Connection::processQueue()
  {
    ENetEvent event;

    this->incomingEventsMutex_->lock();
    uint32_t incomingEventsCount = this->incomingEvents_.size();
    this->incomingEventsMutex_->unlock();
    while( incomingEventsCount > 0 )
    {
      packet::Packet* p;
      this->incomingEventsMutex_->lock();
      event = this->incomingEvents_.front();
      this->incomingEvents_.pop_front();
      this->incomingEventsMutex_->unlock();
      
      switch(event.type)
      {
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
          addPeer( &event );
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          removePeer( &event );
          break;
        case ENET_EVENT_TYPE_RECEIVE:
//           COUT(0) << "ENET_EVENT_TYPE_RECEIVE" << endl;
          p = createPacket( &event );
          processPacket(p);
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
      
      this->incomingEventsMutex_->lock();
      incomingEventsCount = this->incomingEvents_.size();
      this->incomingEventsMutex_->unlock();
    }
  }

  packet::Packet* Connection::createPacket(ENetEvent* event)
  {
    packet::Packet *p = packet::Packet::createPacket(event->packet, event->peer);
    return p;
//     return p->process();
  }
  
  void Connection::enableCompression()
  {
    enet_host_compress_with_range_coder( this->host_ );
  }


}
