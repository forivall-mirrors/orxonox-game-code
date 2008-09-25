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

#include <map>
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
    Packet(const Packet &p);
    virtual ~Packet();
    static Packet *createPacket(ENetPacket *packet, ENetPeer *peer);
    static void deletePacket(ENetPacket *packet);
    
    virtual unsigned char *getData(){ return data_; };
    virtual unsigned int getSize() const =0;
    virtual bool process()=0;
    enet_uint32 getFlags()
      { return flags_; }
    int getClientID()
      { return clientID_; }
    void setClientID( int id )
      { clientID_ = id; }
    
    bool send();
  protected:
    Packet();
    Packet(unsigned char *data, int clientID);
//    Packet(ENetPacket *packet, ENetPeer *peer);
    enet_uint32 flags_;
    int clientID_;
    ENUM::Direction packetDirection_;
    unsigned char *data_;
  private:
    static std::map<ENetPacket *, Packet *> packetMap_;
    ENetPacket *enetPacket_;
};

} //namespace packet

} //namespace network

#endif
