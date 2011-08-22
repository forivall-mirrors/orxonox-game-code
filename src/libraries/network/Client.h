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
// C++ Interface: Client
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _Client_H__
#define _Client_H__

#include "NetworkPrereqs.h"

#include <string>
#include <deque>

#include "util/UtilPrereqs.h"
#include "util/Singleton.h"
#include "ClientConnection.h"
// #include "GamestateClient.h"
#include "Host.h"
#include "LANDiscovery.h"
#include "packet/ServerInformation.h"

// tolua_begin
namespace orxonox
{
// tolua_end
  /**
  Client *client;
  * The network/Client class
  * This class implements all necessary function for the network communication
  * It is the root class of the network module
  *
  */
  class _NetworkExport Client // tolua_export
    : public Host, protected ClientConnection, public Singleton<Client>
  { // tolua_export
  friend class Singleton<Client>;
  public:
    Client();
    ~Client();

    static Client* getInstance(){ return singletonPtr_s; } // tolua_export

    bool establishConnection();
    void setDestination( const std::string& serverAddress, unsigned int port ); // tolua_export
    bool closeConnection();
    void queuePacket(ENetPacket* packet, int clientID, uint8_t channelID);
    virtual bool sendPacket( packet::Packet* packet ){ return packet->send( static_cast<Host*>(this) ); }
    virtual void doSendChat(const std::string& message, unsigned int sourceID, unsigned int targetID);
    virtual void doReceiveChat(const std::string& message, unsigned int sourceID, unsigned int targetID);
    virtual void printRTT();

    void update(const Clock& time);
  protected:
    virtual void connectionClosed();
  private:
    Client(const Client& copy); // not used
    virtual bool isServer_(){return false;}
    void processPacket(packet::Packet* packet);

    static Client* singletonPtr_s;
    bool isSynched_;
    std::deque<packet::Packet*> packetQueue_;

    bool gameStateFailure_;
    float timeSinceLastUpdate_;
  }; // tolua_export


} // tolua_export

#endif /* _Client_H__ */
