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

#include "TrafficControl.h"

#include <cassert>
#include <boost/bind.hpp>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "synchronisable/Synchronisable.h"

namespace orxonox {

  static const unsigned int SCHED_PRIORITY_OFFSET = static_cast<unsigned int>(-1);

  objInfo::objInfo(uint32_t ID, uint32_t creatorID, int32_t curGsID, int32_t diffGsID, uint32_t size, unsigned int prioperm, unsigned int priosched)
  {
    objID = ID; objCreatorID = creatorID; objCurGS = curGsID; objDiffGS = diffGsID; objSize = size; objValuePerm = prioperm; objValueSched = priosched;
  }

  objInfo::objInfo()
  {
    objID = OBJECTID_UNKNOWN; objCreatorID = OBJECTID_UNKNOWN; objCurGS = GAMESTATEID_INITIAL; objDiffGS = objCurGS; objSize = 0; objValuePerm = 0; objValueSched = 0;
  }



  obj::obj()
  {
    objID = OBJECTID_UNKNOWN; objCreatorID = OBJECTID_UNKNOWN; objSize = 0; objDataOffset = 0;
  }
  obj::obj( uint32_t ID, uint32_t creatorID, uint32_t size, uint32_t offset )
  {
    objID = ID; objCreatorID = creatorID; objSize = size; objDataOffset = offset;
  }

/**
*Initializing protected members
*/
    TrafficControl *TrafficControl::instance_=0;

    /**
    * @brief Constructor: assures that only one reference will be created and sets the pointer
    */
    TrafficControl::TrafficControl()
    {
    RegisterObject(TrafficControl);
      assert(instance_==0);
      instance_=this;
    this->setConfigValues();
    }

    /**
    * @brief Destructor: resets the instance pointer to 0
    */
    TrafficControl::~TrafficControl()
    {
      instance_=0;
    }

/**
*Definition of public members
*/

  void TrafficControl::setConfigValues()
  {
    SetConfigValue ( bActive_, false );
    SetConfigValue ( targetSize, 10000 );
  }

  /**
  * sort-algorithm for sorting the objectlist after priorities
  */
  bool TrafficControl::prioritySort(uint32_t clientID, obj i, obj j)
  {
    assert(clientListPerm_.find(clientID) != clientListPerm_.end());  //make sure the client exists in our list
    assert(clientListPerm_[clientID].find(i.objID) != clientListPerm_[clientID].end()); // make sure the object i is in the client list
    assert(clientListPerm_[clientID].find(j.objID) != clientListPerm_[clientID].end()); // make sure the object j is in the client list

    int prio1 = clientListPerm_[clientID][i.objID].objValuePerm + clientListPerm_[clientID][i.objID].objValueSched;
    int prio2 = clientListPerm_[clientID][j.objID].objValuePerm + clientListPerm_[clientID][j.objID].objValueSched;
    return prio1 < prio2;
  }

  /**
  * sort-algorithm for sorting the objectList after position in original data stream
  */
  bool TrafficControl::dataSort(obj i, obj j)
  {
    int pos1 = i.objDataOffset;
    int pos2 = j.objDataOffset;
    return pos1 < pos2;
  }



    void TrafficControl::processObjectList(unsigned int clientID, unsigned int gamestateID, std::list<obj>& list)
    {
      currentClientID=clientID;
      currentGamestateID=gamestateID;
      evaluateList(clientID, list);
      return;
    }

  TrafficControl *TrafficControl::getInstance()
  {
    assert(instance_);
    return instance_;
  }

    void TrafficControl::ack(unsigned int clientID, unsigned int gamestateID)
    {
    if ( !this->bActive_ )
      return;
      std::list<obj>::iterator itvec;  // iterator to iterate through the acked objects

    //assertions to make sure the maps already exist
    assert(clientListTemp_.find(clientID) != clientListTemp_.end() );
    assert(clientListPerm_.find(clientID) != clientListPerm_.end() );
    assert( clientListTemp_[clientID].find(gamestateID) != clientListTemp_[clientID].end() );

    // shortcut for maps
    std::map<unsigned int, objInfo >& objectListPerm = clientListPerm_[clientID];
    std::map<unsigned int, std::list<obj> >& objectListTemp = clientListTemp_[clientID];

    for(itvec = objectListTemp[gamestateID].begin(); itvec != objectListTemp[gamestateID].end(); itvec++)
      {
      if(objectListPerm.find(itvec->objID) != objectListPerm.end()) // check whether the obj already exists in our lists
      {
        objectListPerm[itvec->objID].objCurGS = gamestateID;
        objectListPerm[itvec->objID].objValueSched = 0; //set scheduling value back
      }
      else
      {
        assert(0);
        objectListPerm[itvec->objID].objCurGS = gamestateID;
        objectListPerm[itvec->objID].objID = itvec->objID;
        objectListPerm[itvec->objID].objCreatorID = itvec->objCreatorID;
        objectListPerm[itvec->objID].objSize = itvec->objSize;
      }
      }
       // remove temporary list (with acked objects) from the map
    objectListTemp.erase( objectListTemp.find(gamestateID) );
    }

/**
*Definition of private members
*/

    /**
    *updateClientListPerm
    *returns void
    */
    void TrafficControl::insertinClientListPerm(unsigned int clientID, obj objinf)
    {
      std::map<unsigned int,std::map<unsigned int, objInfo> >::iterator itperm;//iterator clientListPerm over clientIDs
    unsigned int gsid=GAMESTATEID_INITIAL, gsdiff=currentGamestateID, prioperm=Synchronisable::getSynchronisable(objinf.objID)->getPriority(), priomom=0;
    clientListPerm_[clientID][objinf.objID] = objInfo(objinf.objID, objinf.objCreatorID,gsid,gsdiff, objinf.objSize,prioperm,priomom);
    }

  /**
  * updateClientListTemp
  * takes the shortened list which will be sent to the gsmanager and puts the *info into clientListTemp
  */
  void TrafficControl::updateClientListTemp(std::list<obj>& list)
  {
    clientListTemp_[currentClientID][currentGamestateID] = list;
  }

  /**
  *cut
  *takes the current list that has to be returned to the gsmanager and shortens it in criteria of bandwidth of clientID(XY)
  */
  void TrafficControl::cut(std::list<obj>& list, unsigned int targetsize)
  {
    unsigned int size=0;
    std::list<obj>::iterator itvec, ittemp;
    assert(!list.empty());
    for(itvec = list.begin(); itvec != list.end();)
    {
      assert( itvec->objSize < 1000);
      if ( ( size + itvec->objSize ) < targetsize )
      {
        size += itvec->objSize;//objSize is given in bytes
        ++itvec;
      }
      else
      {
        clientListPerm_[currentClientID][itvec->objID].objValueSched += SCHED_PRIORITY_OFFSET; // NOTE: SCHED_PRIORITY_OFFSET is negative
        list.erase(itvec, list.end());
        break;
      }
//       printList(list, currentClientID);
    }
    assert(!list.empty());
  }


    /**
    *evaluateList evaluates whether new obj are there, whether there are things to be updatet and manipulates all this.
    */
    void TrafficControl::evaluateList(unsigned int clientID, std::list<obj>& list)
    {

    if( bActive_ )
    {
      //now the sorting

      //compare listToProcess vs clientListPerm
      //if listToProcess contains new Objects, add them to clientListPerm
      std::list<obj>::iterator itvec;

      std::map<unsigned int, objInfo >& objectListPerm = clientListPerm_[clientID];

      for( itvec=list.begin(); itvec != list.end(); itvec++)
      {
        if ( objectListPerm.find( itvec->objID) != objectListPerm.end() )
        {
        // we already have the object in our map
        //obj bleibt in liste und permanente prio wird berechnet
          objectListPerm[itvec->objID].objDiffGS = currentGamestateID - objectListPerm[itvec->objID].objCurGS;
          continue;//check next objId
        }
        else
        {
        // insert the object into clientListPerm
          insertinClientListPerm(clientID,*itvec);
          continue;//check next objId
        }
      }
    //end compare listToProcess vs clientListPerm

      //sort copied list according to priorities
      // use boost bind here because we need to pass a memberfunction to stl sort
//       sort( list.begin(), list.end(), boost::bind(&TrafficControl::prioritySort, this, clientID, _1, _2) );
      list.sort( boost::bind(&TrafficControl::prioritySort, this, clientID, _1, _2) );

//       list.sort(boost::bind(&TrafficControl::prioritySort, this, clientID, _1, _2) );

      //now we check, that the creator of an object always exists on a client
      std::list<obj>::iterator itcreator;
      for(itvec = list.begin(); itvec != list.end(); itvec++)
      {
        fixCreatorDependencies(itvec, list, clientID);
      }
      //end of sorting
      //now the cutting, work the same obj out in processobjectlist and copiedlist, compression rate muss noch festgelegt werden.
//       printList(list, clientID);
      cut(list, targetSize);
      //now sort again after objDataOffset
//       sort(list.begin(), list.end(), boost::bind(&TrafficControl::dataSort, this, _1, _2) );
      list.sort( boost::bind(&TrafficControl::dataSort, this, _1, _2) );

      //diese Funktion updateClientList muss noch gemacht werden
      updateClientListTemp(list);
      //end of sorting
    }
  }

  void TrafficControl::printList(std::list<obj>& list, unsigned int clientID)
  {
    std::list<obj>::iterator it;
    orxout(debug_output, context::network) << "=========== Objectlist ===========" << endl;
    for( it=list.begin(); it!=list.end(); it++)
      orxout(debug_output, context::network) << "ObjectID: " << it->objID << " creatorID: " << it->objCreatorID << " Priority: " << clientListPerm_[clientID][it->objID].objValuePerm + clientListPerm_[clientID][it->objID].objValueSched << " size: " << it->objSize << endl;
  }

  void TrafficControl::fixCreatorDependencies(std::list<obj>::iterator it1, std::list<obj>& list, unsigned int clientID)
  {
    if ( it1->objCreatorID == OBJECTID_UNKNOWN )
      return;
    if( clientListPerm_[clientID][it1->objCreatorID].objCurGS != GAMESTATEID_INITIAL )
      return;
    std::list<obj>::iterator it2, it3=it1;
    for( it2 = ++it3; it2 != list.end(); it2++ )
    {
      if( it2->objID == it1->objCreatorID )
      {
        it3 = list.insert(it1, *it2); //insert creator before it1
        list.erase(it2);
//         printList(list, clientID);
        fixCreatorDependencies( it3, list, clientID );
        break;
      }
    }
  }

  void TrafficControl::clientDisconnected(unsigned int clientID)
  {
    if( clientListPerm_.find(clientID) == clientListPerm_.end() )
    {
      assert(!this->bActive_);
      return;
    }
    clientListTemp_.erase(clientListTemp_.find(clientID));
    if( clientListTemp_.find(clientID) != clientListTemp_.end() )
      clientListPerm_.erase(clientListPerm_.find(clientID));
  }


}//namespace network
