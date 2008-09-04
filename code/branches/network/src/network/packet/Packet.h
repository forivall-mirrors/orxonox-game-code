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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */
#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H

#include "PacketContent.h"

#include <enet/enet.h>

namespace network {

namespace packet{
  
namespace ENUM{
  enum Direction{
    Incoming,
    Outgoing,
    Bidirectional
  };
  enum Type{
    Acknowledgement,
    Gamestate,
    ClassID,
    Chat,
    Welcome
  };
}
  
/**
	@author Oliver Scheuss <scheusso [at] ee.ethz.ch>
*/
class Packet{
  public:
    Packet();
    Packet(PacketContent *content);
    Packet(ENetPacket *packet, ENetPeer *peer);
    virtual ~Packet();
    
    int getClientID(){ return clientID_; }
    PacketContent *getPacketContent(){ return packetContent_; }
    
    void setClientID( int id ){ clientID_ = id; }
    void setPacketContent(PacketContent *content);
    
    bool send();
  protected:
  private:
    PacketContent *createPacketContent(ENetPacket *packet);
    
    int clientID_;
    PacketContent *packetContent_;
    ENetPacket *enetPacket_;
    ENUM::Direction packetDirection_;
};

} //namespace packet

} //namespace network

#endif
