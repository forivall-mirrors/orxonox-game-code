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
// C++ Interface: PacketBuffer
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _PacketBuffer_H__
#define _PacketBuffer_H__

#include <enet/enet.h>

#include "NetworkPrereqs.h"

namespace network
{
  struct PacketEnvelope{
    int length;
    int data;
  };

  struct QueueItem{
    ENetPacket *packet;
    ENetAddress address;
    QueueItem *next;
  };

  class PacketBuffer{
  public:
    PacketBuffer();
    bool isEmpty();
    bool isClosed();
    void setClosed(bool value);
    void print();
    // pops a packet from the queue
    ENetPacket *pop();
    ENetPacket *pop(ENetAddress &address);
    // pushs a packet to the queue
    bool push(ENetEvent *ev);

  private:
    QueueItem *first;
    QueueItem *last;
    bool closed;

  };

} //namespace
#endif /* _PacketBuffer_H__ */
