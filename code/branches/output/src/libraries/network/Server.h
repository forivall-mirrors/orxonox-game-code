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

#ifndef _Server_H__
#define _Server_H__

#include "NetworkPrereqs.h"

#include <deque>
#include <vector>

#include "util/UtilPrereqs.h"
#include "core/CorePrereqs.h"
#include "Host.h"
// #include "GamestateManager.h"
#include "ServerConnection.h"
#include "LANDiscoverable.h"
#include "WANDiscoverable.h"


namespace orxonox
{

  /**
  * This class is the root class of the network module for a server.
  * It implements all functions necessary for a Server
  */
  class _NetworkExport Server : public Host, public ServerConnection, public LANDiscoverable, public WANDiscoverable
  {
  public:
    Server();
    Server(int port);
    Server(int port, const std::string& bindAddress);
    ~Server();

    void open();
    void close();
    void queuePacket(ENetPacket *packet, int clientID, uint8_t channelID);
    virtual bool sendPacket( packet::Packet* packet ){ return packet->send( static_cast<Host*>(this) ); }
    void update(const Clock& time);
    unsigned int getRTT(unsigned int clientID);
    virtual void printRTT();
    float getPacketLoss(unsigned int clientID);
  protected:
    void updateGamestate();
  private:
    virtual bool isServer_(){return true;}
    unsigned int playerID(){return 0;}

    void addPeer(uint32_t peerID);
    void removePeer(uint32_t peerID);
    void processPacket(packet::Packet* packet);

    bool createClient(int clientID);
    void disconnectClient( uint32_t clientID );
    bool sendGameStates();
    bool sendObjectDeletes();
    bool isValidTarget(unsigned int targetID);
    virtual void doSendChat(const std::string& message, unsigned int sourceID, unsigned int targetID);
    virtual void doReceiveChat(const std::string& message, unsigned int sourceID, unsigned int targetID);
    void syncClassid(unsigned int clientID);

    float timeSinceLastUpdate_;
    std::deque<packet::Packet*> packetQueue_;
    std::vector<uint32_t>       clientIDs_;
  };




}

#endif /* _Server_H__ */
