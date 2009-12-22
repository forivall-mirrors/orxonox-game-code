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

#include "util/Debug.h"
#include "core/ObjectList.h"
#include "synchronisable/Synchronisable.h"
#include "synchronisable/NetworkCallbackManager.h"
#include "packet/Acknowledgement.h"
#include "packet/Gamestate.h"


namespace orxonox
{
  struct _NetworkExport GameStateItem{
    packet::Gamestate *state;
    unsigned int id;
  };

  GamestateClient::GamestateClient() {
    COUT(5) << "this: " << this << std::endl;
    last_diff_=0;
    last_gamestate_=GAMESTATEID_INITIAL-1;
    tempGamestate_=NULL;
  }

  GamestateClient::~GamestateClient() {
      std::map<unsigned int, packet::Gamestate *>::iterator it;
      for ( it = this->gamestateMap_.begin(); it != this->gamestateMap_.end(); ++it )
          delete it->second;
      if( this->tempGamestate_ )
          delete this->tempGamestate_;
  }

  bool GamestateClient::ack(unsigned int gamestateID, unsigned int clientID){
    return true;
  }

  bool GamestateClient::add(packet::Gamestate *gs, unsigned int clientID){
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
    bool isDiffed = tempGamestate_->isDiffed();
    int id = GAMESTATEID_INITIAL;
    packet::Gamestate *processed = processGamestate(tempGamestate_);
    assert(processed);

    //now call the queued callbacks
    NetworkCallbackManager::callCallbacks();

    if (!processed){
      sendAck(0);
      return false;
    }
    //successfully loaded data from gamestate. now save gamestate for diff and delete the old gs
    tempGamestate_=NULL;
    gamestateMap_[processed->getID()]=processed;
    if(isDiffed)
      last_diff_ = processed->getBaseID();
    id = processed->getID();
    sendAck(id);
    return true;
  }


  /**
  * This function removes a Synchronisable out of the universe
  * @param it iterator of the list pointing to the object
  * @return iterator pointing to the next object in the list
  */
  void GamestateClient::removeObject(ObjectList<Synchronisable>::iterator &it) {
    ObjectList<Synchronisable>::iterator temp=it;
    ++it;
    temp->destroy(); // or delete?
  }

  packet::Gamestate *GamestateClient::getGamestate(){
    packet::Gamestate *gs = new packet::Gamestate();
    if(!gs->collectData(0,0x2)){
      delete gs;
      return 0;
    }
    return gs;
  }

  void GamestateClient::cleanup(){
    std::map<unsigned int, packet::Gamestate*>::iterator temp, it = gamestateMap_.begin();
    while(it!=gamestateMap_.end()){
      if(it->first>=last_diff_)
        break;
      // otherwise delete that stuff
      delete it->second;
      temp=it++;
      gamestateMap_.erase(temp);
    }
    tempGamestate_=NULL;
  }

  void GamestateClient::printGamestateMap(){
    std::map<unsigned int, packet::Gamestate*>::iterator it;
    COUT(4) << "gamestates: ";
    for(it=gamestateMap_.begin(); it!=gamestateMap_.end(); it++){
      COUT(4) << it->first << ':' << it->second << '|';
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
      COUT(5) << "acked a gamestate: " << gamestateID << std::endl;
      return true;
    }
  }

  packet::Gamestate *GamestateClient::processGamestate(packet::Gamestate *gs){
    if(gs->isCompressed())
    {
      bool b = gs->decompressData();
      assert(b);
    }
    if(gs->isDiffed()){
      packet::Gamestate *base = gamestateMap_[gs->getBaseID()];
      if(!base){
        COUT(3) << "could not find base gamestate id: " << gs->getBaseID() << endl;
        delete gs;
        return 0;
      }
//       assert(base); //TODO: fix this
      packet::Gamestate *undiffed = gs->undiff(base);
      delete gs;
      gs=undiffed;
      COUT(5) << "successfully undiffed gamestate id: " << undiffed->getID() << std::endl;
    }
    if(gs->spreadData(0x2))
      return gs;
    else
    {
      COUT(3) << "could not spread gamestate" << endl;
      return NULL;
    }
  }

}

