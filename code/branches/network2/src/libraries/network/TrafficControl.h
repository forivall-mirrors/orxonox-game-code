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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>
 *   Co-authors:
 *      ...
 *

 */
#ifndef _TrafficControl_H__
#define _TrafficControl_H__

#include "NetworkPrereqs.h"

#include <list>
#include <map>
#include "network/ClientConnectionListener.h"

namespace orxonox {


  /**
   *a list of objects of this type will be given by the Server's Gamestate Manager
   */
  class objInfo
  {
    public:
      uint32_t objID;
      uint32_t objCreatorID;
      uint32_t objCurGS;//current GameState ID
      uint32_t objDiffGS;//difference between current and latest GameState
      uint32_t objSize;
      int objValuePerm;
      int objValueSched;
      objInfo(uint32_t ID, uint32_t creatorID, int32_t curGsID, int32_t diffGsID, uint32_t size, unsigned int prioperm, unsigned int priosched);
      objInfo();
  };

/**
   *a list of objects of this type will be given by the Server's Gamestate Manager
 */
  class obj
  {
    public:
      uint32_t objID;
      uint32_t objCreatorID;
      uint32_t objSize;
      uint32_t objDataOffset;
      obj();
      obj( uint32_t ID, uint32_t creatorID, uint32_t size, uint32_t offset );
  };




/**
*
*/
class TrafficControl : public ClientConnectionListener {
  private:

    /**
    *permanent client list: contains client ids, object ids and objectInfos (in this order)
    */
    std::map<unsigned int, std::map<unsigned int, objInfo > > clientListPerm_;
    //has to be created with constructor and then needs to be updated by evaluateList().

    /**
    *temporary client list: contains client ids, gamestate ids and object ids (in this order)
    */
    std::map<unsigned int, std::map<unsigned int, std::list<obj> > > clientListTemp_;

    /**updateReferenceList
    *currentGamestateID and currentClientID will be defined as soon as TrafficControl is being called by Server
    */
    unsigned int currentGamestateID;
    unsigned int currentClientID;
    unsigned int targetSize;
    bool         bActive_;

    void insertinClientListPerm(unsigned int clientID, obj objinf);

    void cut(std::list<obj>& list, unsigned int targetsize);
    void updateClientListTemp(std::list<obj>& list);//done
    /**
    *evaluates Data given (list) and produces result(->Data to be updated)
    */
    void evaluateList(unsigned int clientID, std::list<obj>& list);//done
    void ack(unsigned int clientID, unsigned int gamestateID);  // this function gets called when the server receives an ack from the client

    //ClientConnectionListener functions
    virtual void clientConnected(unsigned int clientID){};
    virtual void clientDisconnected(unsigned int clientID);


  protected:
    static TrafficControl *instance_;

  public:
    TrafficControl();
    virtual ~TrafficControl();
    /**
    *is being used by GSManager from Server:
    *list contains: ObjIds, CreatorIds, Size (in this order) from Client XY
    *Elements of list are accessed by *list[i]
    *Elements of struct i are therefore: *list[i].objID
    */
    void setConfigValues();
    static TrafficControl *getInstance();
    void processObjectList(unsigned int clientID, unsigned int gamestateID, std::list<obj>& list); //gets a pointer to the list (containing objectIDs) and sorts it
    static void processAck(unsigned int clientID, unsigned int gamestateID)
    { return instance_->ack(clientID, gamestateID); }
    void deleteObject(unsigned int objectID);               // this function gets called when an object has been deleted (in order to clean up lists and maps)

    bool prioritySort(uint32_t clientID, obj i, obj j);
    bool dataSort(obj i, obj j);
    void printList(std::list<obj>& list, unsigned int clientID);
    void fixCreatorDependencies(std::list<obj>::iterator it, std::list<obj>& list, unsigned int clientID);
};

}

#endif /* _TrafficControl_H__ */
