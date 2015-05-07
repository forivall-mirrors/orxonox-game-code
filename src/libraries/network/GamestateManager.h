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
// C++ Interface: GamestateManager
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _GamestateManager_H__
#define _GamestateManager_H__

#include "NetworkPrereqs.h"

#include <map>
#include "GamestateHandler.h"
#include "core/CorePrereqs.h"
#include "packet/Gamestate.h"
#include <boost/concept_check.hpp>

namespace orxonox
{

  const int KEEP_GAMESTATES = 10;

  /**
  * This Class implements a manager for gamestates:
  * - creating snapshots of gamestates
  * - writing gamestates to universe
  * - diffing gamestates
  * EN/DECODATION:
  * a: last Gamestate a client has received
  * b: new Gamestate
  * x: diffed and compressed gamestate
  * x=(a^b)
  * b=(a^x)
  * diff(a,diff(a,x))=x (hope this is correct)
  * @author Oliver Scheuss
  */
  class _NetworkExport GamestateManager: public GamestateHandler
  {
    struct peerInfo
    {
      uint32_t  peerID;
      uint32_t  lastReceivedGamestateID;  //!< id of the last gamestate which was received (and processed) from the peer
      uint32_t  lastAckedGamestateID;     //!< id of the last gamestate on which we received an ack from the peer
      bool      isSynched;
      std::map< uint32_t, packet::Gamestate* > gamestates;
    };

  public:

    GamestateManager();
    ~GamestateManager();

    virtual bool      addGamestate(packet::Gamestate *gs, unsigned int peerID);
    virtual bool      ackGamestate(unsigned int gamestateID, unsigned int peerID);
    virtual uint32_t  getLastReceivedGamestateID( unsigned int peerID );
    virtual uint32_t  getCurrentGamestateID(){ if( currentGamestate_) return currentGamestate_->getID(); else return GAMESTATEID_INITIAL; }

    bool processGamestates();
    bool sendAck(unsigned int gamestateID, uint32_t peerID);
    bool update();
    std::vector<packet::Gamestate*> getGamestates();
    void finishGamestate( unsigned int peerID, packet::Gamestate*& destgamestate, packet::Gamestate* base, packet::Gamestate* gamestate );

    bool getSnapshot();

    void addPeer( uint32_t peerID );
    void setSynched( uint32_t peerID )
      { assert(peerMap_.find(peerID)!=peerMap_.end()); peerMap_[peerID].isSynched = true; }
    void removePeer( uint32_t peerID );
    bool hasPeers(){ return this->peerMap_.size()!=0; }
//     void removeClient(ClientInformation *client);
  protected:
    virtual bool sendPacket( packet::Packet* packet ) = 0;
  private:
    bool processGamestate(packet::Gamestate *gs);

//     std::map<unsigned int, std::map<unsigned int, packet::Gamestate*> > gamestateMap_;
    std::map<unsigned int, packet::Gamestate*> gamestateQueue;
//     std::map<unsigned int, uint32_t> lastProcessedGamestateID_;
    std::map<uint32_t, peerInfo> peerMap_;
    packet::Gamestate* currentGamestate_;
//     TrafficControl *trafficControl_;
    unsigned int id_;
//     boost::mutex* threadMutex_;
    ThreadPool*   /*thread*/Pool_;
  };

}

#endif /* _GameStateManager_H__ */
