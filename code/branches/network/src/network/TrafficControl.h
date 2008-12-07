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
#ifndef NETWORKTRAFFICCONTROL_H
#define NETWORKTRAFFICCONTROL_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "NetworkPrereqs.h"
#include "Synchronisable.h"
#include "ClientInformation.h"
#include "util/Integers.h"

namespace network {

/**
*a vector of objects of this type will be given by the Server's Gamestate Manager
*/
struct objInfo
{
  uint32_t objID;
  uint32_t objCreatorID;
  uint32_t objCurGS;//current GameState ID
  uint32_t objDiffGS;//difference between current and latest GameState
  uint32_t objSize;
  unsigned int objValuePerm;
  unsigned int objValueSched;
  objInfo(uint32_t ID, uint32_t creatorID, int32_t curGsID, int32_t diffGsID, uint32_t size, unsigned int prioperm, unsigned int priosched)
    { objID = ID; objCreatorID = creatorID; objCurGS = curGsID; objDiffGS = diffGsID; objSize = size; objValuePerm = prioperm; objValueSched = priosched; }
  objInfo()
    { objID = OBJECTID_UNKNOWN; objCreatorID = OBJECTID_UNKNOWN; objCurGS = GAMESTATEID_INITIAL; objDiffGS = objCurGS; objSize = 0; objValuePerm = 0; objValueSched = 0; }
};

/**
*a vector of objects of this type will be given by the Server's Gamestate Manager
*/
struct obj
{
  uint32_t objID;
  uint32_t objCreatorID;
  uint32_t objSize;
  uint32_t objDataOffset;
  obj()
    { objID = OBJECTID_UNKNOWN; objCreatorID = OBJECTID_UNKNOWN; objSize = 0; objDataOffset = 0; }
  obj( uint32_t ID, uint32_t creatorID, uint32_t size, uint32_t offset )
  { objID = ID; objCreatorID = creatorID; objSize = size; objDataOffset = offset; }
};


const unsigned int SCHED_PRIORITY_OFFSET = -5;


/**
*
*/
class TrafficControl{
  private:

    /**
    *Lists that will be used:
    *listToProcess
    *clientListPerm
    *clientListTemp
    *referenceList
    *permObjPrio list
    *schedObjPrio
    */
    //start: lists to be used
    /**
    *creates list (typ map) that contains objids, struct with info concerning object(objid)
    */
//     std::map<unsigned int, objInfo> listToProcess_;//copy of argument, when traffic control tool is being called, the original of this must be returned later on, eg the vector given by GS
    /**
    *permanent client list: contains client ids, object ids and objectInfos (in this order)
    */
    std::map<unsigned int, std::map<unsigned int, objInfo > > clientListPerm_;
    //has to be created with constructor and then needs to be updated by evaluateList().

    /**
    *temporary client list: contains client ids, gamestate ids and object ids (in this order)
    */
    std::map<unsigned int, std::map<unsigned int, std::vector<obj> > > clientListTemp_;
    /**
    *static priority list: contains obj id, basic priority (in this order)
    */
//     std::map<unsigned int, unsigned int> permObjPrio_;
    /**
    *dynamic priority list: contains obj id, dynamic priority (eg scheduled) (in this order)
    */
//     std::map<unsigned int, unsigned int> schedObjPrio_;
    //end: lists to be used

    /**updateReferenceList
    *currentGamestateID and currentClientID will be defined as soon as TrafficControl is being called by Server
    */
    unsigned int currentGamestateID;
    unsigned int currentClientID;
    unsigned int targetSize;
    /**
    *copiedVector is a copy of the given Vector by the GSmanager, on this vector all manipulations are performed
    */
//     std::vector<obj> copiedVector;

//     void updateReferenceList(std::map<unsigned int, objInfo> *list);//done
    void insertinClientListPerm(unsigned int clientID, obj objinf);//done
    /**
    *creates listToProcess, which can be easialy compared with other lists
    */
//     void copyList(std::vector<obj> *list);//done
    
    void cut(std::vector<obj> *list, unsigned int targetsize);
    void updateClientListTemp(std::vector<obj> *list);//done
    /**
    *evaluates Data given (vector) and produces result(->Data to be updated)
    */
    void evaluateList(unsigned int clientID, std::vector<obj> *list);//done    

  protected:
    TrafficControl();
    virtual ~TrafficControl();//virtual because???
    static TrafficControl *instance_;

  public:
    /**
    *is being used by GSManager from Server:
    *vector contains: ObjIds, CreatorIds, Size (in this order) from Client XY 
    *Elements of vector are accessed by *list[i]
    *Elements of struct i are therefore: *list[i].objID
    */
    void processObjectList(unsigned int clientID, unsigned int gamestateID, std::vector<obj>* list); //gets a pointer to the vector (containing objectIDs) and sorts it
    //done
    void processAck(unsigned int clientID, unsigned int gamestateID);	// this function gets called when the server receives an ack from the client
    //done
    void deleteObject(unsigned int objectID);				// this function gets called when an object has been deleted (in order to clean up lists and maps)
    
    bool priodiffer(uint32_t clientID, obj i, obj j);
};

}

#endif

