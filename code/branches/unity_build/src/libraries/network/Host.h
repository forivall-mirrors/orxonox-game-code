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

#ifndef _NETWORK_Host_H__
#define _NETWORK_Host_H__

#include "NetworkPrereqs.h"
#include "GamestateManager.h"
#include "core/CorePrereqs.h"

#include <vector>

namespace orxonox {

  const unsigned int CLIENTID_SERVER = 0;
  const unsigned int NETWORK_FREQUENCY = 25;
  const float NETWORK_PERIOD = 1.0f/NETWORK_FREQUENCY;

/**
*       @brief Base class of Server and Client
*       This is the Base class of the Server and Client classes
*       - Makes server and client a singleton
*       - defines static functions available on both server and client
*       - is the interface to be used when communicating with the network
*       @author Oliver Scheuss
*/
class _NetworkExport Host: public GamestateManager
{
  private:
    //TODO add these functions or adequate
    //virtual bool processChat(packet::Chat *message, unsigned int clientID)=0;
    //virtual bool sendChat(packet::Chat *chat)=0;
    virtual void queuePacket(ENetPacket *packet, int clientID, uint8_t channelID)=0;
    virtual bool chat(const std::string& message)=0;
    virtual bool broadcast(const std::string& message)=0;
    virtual bool processChat(const std::string& message, unsigned int playerID)=0;
    virtual bool isServer_()=0;



  protected:
    Host();
    virtual ~Host();
    void setActive( bool bActive ){ bIsActive_ = bActive; }
//     static Host *instance_;

  public:
    static Host* getActiveInstance();
    static bool running(){ return instances_s.size(); }
    static void addPacket(ENetPacket* packet, int clientID = NETWORK_PEER_ID_SERVER, uint8_t channelID = 0);
    //static bool chat(std::string& message);
//     static bool receiveChat(packet::Chat *message, unsigned int clientID);
    static unsigned int getPlayerID(){ return clientID_s; }
    static void setClientID(unsigned int id){ clientID_s = id; }
    static bool isServer();
    static void Chat(const std::string& message);
    static bool Broadcast(const std::string& message);
    static bool incomingChat(const std::string& message, unsigned int playerID);
    virtual void printRTT()=0;
    bool isActive(){ return bIsActive_; }
  private:
    static uint32_t clientID_s;
    static std::vector<Host*> instances_s;
    bool bIsActive_;
};

}

#endif /* _NETWORK_Host_H__ */
