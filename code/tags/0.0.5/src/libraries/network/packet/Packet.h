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
#ifndef _NETWORK_Packet_H__
#define _NETWORK_Packet_H__

#include "network/NetworkPrereqs.h"
#include <map>

namespace orxonox
{

namespace packet
{

namespace Direction
{
  enum Value
  {
    Incoming,
    Outgoing,
    Bidirectional
  };
}
namespace Type
{
  enum Value
  {
    Acknowledgement,
    Chat,
    ClassID,
    DeleteObjects,
    FunctionIDs,
    FunctionCalls,
    Gamestate,
    Welcome
  };
}

/**
    @author Oliver Scheuss <scheusso [at] ee.ethz.ch>
*/
class _NetworkExport Packet
{
  public:
    Packet(const Packet &p);
    virtual ~Packet();
    static Packet* createPacket(ENetPacket* packet, uint32_t peerID);
    static void deletePacket(ENetPacket* packet);

    virtual unsigned char* getData(){ return data_; };
    virtual unsigned int getSize() const =0;
    virtual bool process(orxonox::Host* host)=0;
    inline uint32_t getFlags()
      { return flags_; }
    inline int getPeerID()
      { return peerID_; }
    inline void setPeerID( int id )
      { peerID_ = id; }
    inline bool isReliable()
      { return this->flags_ & PacketFlag::Reliable; }
    inline uint32_t getRequiredGamestateID()
      { return this->requiredGamestateID_; }

    virtual bool send(orxonox::Host* host);
  protected:
    Packet();
    Packet(uint8_t *data, unsigned int peerID);
//    Packet(ENetPacket *packet, ENetPeer *peer);
    inline bool isDataENetAllocated() const
      { return bDataENetAllocated_; }

    uint32_t flags_;
    unsigned int peerID_;
    uint32_t requiredGamestateID_;
    Direction::Value packetDirection_;
    /** Pointer to the data. Be careful when deleting it because it might
        point to a location that was allocated by ENet.
        See bDataENetAllocated_ */
    uint8_t *data_;
    /** Tells whether data_ was allocated by ENet or ourselves.
        data_ might no correlate with enetPacket_->data. */
    bool bDataENetAllocated_;
  private:
    static std::map<size_t, Packet *> packetMap_;
    static boost::mutex               packetMapMutex_;
    ENetPacket *enetPacket_;
};

} //namespace packet

} //namespace orxonox

#endif /* _NETWORK_Packet_H__ */
