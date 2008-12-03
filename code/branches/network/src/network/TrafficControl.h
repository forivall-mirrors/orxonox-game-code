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

namespace network {

/**
*a vector of objects of this type will be given by the Server's Gamestate Manager
*/
struct objInfo
{
  unsigned int objCreatorID;
  unsigned int objCurGS;//current GameState ID
  unsigned int objDiffGS;//difference between current and latest GameState
  unsigned int objSize;
  unsigned int objValuePerm;
  unsigned int objValueSched;
};

/**
*a vector of objects of this type will be given by the Server's Gamestate Manager
*/
struct obj
{
  unsigned int objID;
  unsigned int objCreatorID;
  unsigned int objSize;
};



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
    std::map<unsigned int, objInfo> listToProcess_;//copy of argument, when traffic control tool is being called, the original of this must be returned later on, eg the vector given by GS
    /**
    *permanent client list: contains client ids, object ids and objectInfos (in this order)
    */
    std::map<unsigned int, std::map<unsigned int, objInfo>> clientListPerm_;
    //has to be created with constructor and then needs to be updated by evaluateList().

    /**
    *temporary client list: contains client ids, gamestate ids and object ids (in this order)
    */
    std::map<unsigned int, std::map<unsigned int, std::vector<obj>>> clientListTemp_;
    /**
    *static priority list: contains obj id, basic priority (in this order)
    */
    std::map<unsigned int, unsigned int> permObjPrio_;
    /**
    *dynamic priority list: contains obj id, dynamic priority (eg scheduled) (in this order)
    */
    std::map<unsigned int, unsigned int> schedObjPrio_;
    //end: lists to be used

    /**
    *currentGamestateID and currentClientID will be defined as soon as TrafficControl is being called by Server
    */
    unsigned int currentGamestateID;
    unsigned int currentClientID;
    unsigned int targetSize;
    /**
    *copiedVector is a copy of the given Vector by the GSmanager, on this vector all manipulations are performed
    */
    std::vector<obj> copiedVector;

    void updateReferenceList(std::map<unsigned int, objInfo> *list);//done
    void insertinClientListPerm(unsigned int clientid, unsigned int objid, objInfo objinf);//done
    /**
    *creates listToProcess, which can be easialy compared with other lists
    */
    void copyList(std::vector<obj> *list);//done
    
    void cut(std::vector<obj> *list,int targetsize);
    void updateClientListTemp(std::vector<obj> *list);//done
    /**
    *evaluates Data given (vector) and produces result(->Data to be updated)
    */
    void evaluateList(std::vector<obj> *list);//done

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
    std::vector<obj>* processObjectList(unsigned int clientID, unsigned int gamestateID, std::vector<obj>* list); //gets a pointer to the vector (containing objectIDs) and sorts it
    //done
    void processAck(unsigned int clientID, unsigned int gamestateID);	// this function gets called when the server receives an ack from the client
    //done
    void deleteObject(unsigned int objectID);				// this function gets called when an object has been deleted (in order to clean up lists and maps)
    
    bool priodiffer(obj i, obj j);
};

}

#endif

