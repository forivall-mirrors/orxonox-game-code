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
#ifndef NETWORKHOST_H
#define NETWORKHOST_H

#include <string>

#include "NetworkPrereqs.h"
#include "packet/Chat.h"

namespace network {

/**
	@author Oliver Scheuss
*/
class Host{
  private:
    //TODO add theese functions or adequate
    //virtual bool processChat(packet::Chat *message, unsigned int clientID)=0;
    //virtual bool sendChat(packet::Chat *chat)=0;
    virtual bool queuePacket(ENetPacket *packet, int clientID)=0;
    virtual unsigned int shipID()=0;
    virtual int playerID()=0;



  protected:
    Host();
    virtual ~Host();
    static Host *instance_;
    bool isServer_;	

  public:
    static bool running(){return instance_!=0;}
    static bool addPacket(ENetPacket *packet, int clientID=0);
    //static bool chat(std::string& message);
//     static bool receiveChat(packet::Chat *message, unsigned int clientID);
    static int getPlayerID();
    static unsigned int getShipID(){return instance_->shipID_;}
    static void setClientID(unsigned int id){ instance_->clientID_ = id; }
    static void setShipID(unsigned int id){ instance_->shipID_ = id; }
    static bool isServer(){ return instance_->isServer_; }		
  private:
    unsigned int clientID_;
    unsigned int shipID_;
};

}

#endif
