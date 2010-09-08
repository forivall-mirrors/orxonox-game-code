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
 *      Dumeni Manatschal
 *
 */

#include "GamestateClient.h"

#include <zlib.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "core/Iterator.h"
#include "Synchronisable.h"
#include "packet/Acknowledgement.h"


namespace network
{
  struct GameStateItem{
    packet::Gamestate *state;
    int id;
  };

  GamestateClient::GamestateClient() {
    COUT(5) << "this: " << this << std::endl;
    last_diff_=0;
    last_gamestate_=GAMESTATEID_INITIAL-1;
    tempGamestate_=NULL;
    myShip_=NULL;
  }

  GamestateClient::~GamestateClient() {
  }

  bool GamestateClient::ack(int gamestateID, int clientID){
    return true;
  }

  bool GamestateClient::add(packet::Gamestate *gs, int clientID){
    if(tempGamestate_!=NULL){
      //delete the obsolete gamestate
      if(tempGamestate_->getID()>gs->getID())
        return false;
      delete tempGamestate_;
    }
    tempGamestate_=gs;
    return true;
  }

  bool GamestateClient::processGamestates(){
    if(tempGamestate_==NULL)
      return false;
    int id = GAMESTATEID_INITIAL;
    bool b = saveShipCache();
    packet::Gamestate *processed = processGamestate(tempGamestate_);
    if(!processed){
      if(b)
        loadShipCache();
      return false;
    }
//    assert(processed);
    //successfully loaded data from gamestate. now save gamestate for diff and delete the old gs
    tempGamestate_=NULL;
    gamestateMap_[processed->getID()]=processed;
    last_diff_ = processed->getID();
    if(b)
      loadShipCache();
    id = processed->getID();
    sendAck(id);
    return true;
  }


  /**
  * This function removes a Synchronisable out of the universe
  * @param it iterator of the list pointing to the object
  * @return iterator pointing to the next object in the list
  */
  void GamestateClient::removeObject(orxonox::ObjectList<Synchronisable>::iterator &it) {
    orxonox::ObjectList<Synchronisable>::iterator temp=it;
    ++it;
    delete  *temp;
  }

  packet::Gamestate *GamestateClient::getGamestate(){
    packet::Gamestate *gs = new packet::Gamestate();
    gs->collectData(0);
    return gs;
  }

  void GamestateClient::cleanup(){
    std::map<int, packet::Gamestate*>::iterator temp, it = gamestateMap_.begin();
    while(it!=gamestateMap_.end()){
      if(it->first>=last_diff_)
        break;
      // otherwise delete that stuff
      delete (*it).second;
      temp=it++;
      gamestateMap_.erase(temp);
    }
    tempGamestate_=NULL;
  }

  void GamestateClient::printGamestateMap(){
    std::map<int, packet::Gamestate*>::iterator it;
    COUT(4) << "gamestates: ";
    for(it=gamestateMap_.begin(); it!=gamestateMap_.end(); it++){
      COUT(4) << it->first << ":" << it->second << "|";
    }
    COUT(4) << std::endl;

  }
  
  bool GamestateClient::sendAck(unsigned int gamestateID){
    packet::Acknowledgement *ack = new packet::Acknowledgement(gamestateID, 0);
    if(!ack->send()){
      COUT(3) << "could not ack gamestate: " << gamestateID << std::endl;
      return false;
    }
    else{
      COUT(3) << "acked a gamestate: " << gamestateID << std::endl;
      return true;
    }
  }

  bool GamestateClient::saveShipCache(){
    if(myShip_==NULL){
      myShip_ = orxonox::SpaceShip::getLocalShip();
//      COUT(2) << "myShip_: " << myShip_ << " getLocalShip(): " << orxonox::SpaceShip::getLocalShip() << std::endl;
      if(!myShip_)
	return false;
    }
    if(myShip_){
      //      unsigned char *data = new unsigned char[myShip_->getSize()];
      int size=myShip_->getSize2(0, 0x1);
      if(size==0)
        return false;
      shipCache_ = new unsigned char [size];
      unsigned char *temp = shipCache_;
      if(!myShip_->getData(temp, 0, 0x1))
        COUT(3) << "could not save shipCache" << std::endl;
      return true;
    }else
      return false;
  }

  bool GamestateClient::loadShipCache(){
    myShip_=orxonox::SpaceShip::getLocalShip(); //TODO: remove this (only a hack)
    if(myShip_ && shipCache_){
      assert(myShip_->getIdentifier());
      unsigned char *temp = shipCache_;
      myShip_->updateData(temp, 0x2);
      delete shipCache_;
      return true;
    }else
      return false;
  }

  packet::Gamestate *GamestateClient::processGamestate(packet::Gamestate *gs){
    if(gs->isCompressed())
      assert(gs->decompressData());
    if(gs->isDiffed()){
      packet::Gamestate *base = gamestateMap_[gs->getBaseID()];
      if(!base){
        delete gs;
        return 0;
      }
//      assert(base); //TODO: fix this
      packet::Gamestate *undiffed = gs->undiff(base);
      delete gs;
      gs=undiffed;
      COUT(3) << "successfully undiffed gamestate id: " << undiffed->getID() << std::endl;
    }
    if(gs->spreadData())
      return gs;
    else
      return NULL;
  }

}
