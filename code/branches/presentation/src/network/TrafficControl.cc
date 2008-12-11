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

#include "TrafficControl.h"

#include "synchronisable/Synchronisable.h"
#include "core/ConfigValueIncludes.h"

#include <cassert>
#include <boost/bind.hpp>

namespace orxonox {

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
	  //was macht das genau? da instance ja gleich this ist im moment
	  instance_=0;
	  //muss ich alles deallozieren was ich im constructor aufgebaut habe?
	}

/**
*Definition of public members
*/

  void TrafficControl::setConfigValues()
  {
    SetConfigValue ( targetSize, 5000 );
  }

  /**
          *eigener sortieralgorithmus
  */
//   bool TrafficControl::priodiffer(obj i, obj j)
//   {
//     std::map<unsigned int, objInfo>::iterator iti;
//     std::map<unsigned int, objInfo>::iterator itj;
//     iti=listToProcess_.find(i.objID);
//     itj=listToProcess_.find(j.objID);
//     return iti->second.objValuePerm < itj->second.objValuePerm;
//   }
  
  /**
  * eigener sortieralgorithmus
  */
  bool TrafficControl::priodiffer(uint32_t clientID, obj i, obj j)
  {
    assert(clientListPerm_.find(clientID) != clientListPerm_.end());  //make sure the client exists in our list
    assert(clientListPerm_[clientID].find(i.objID) != clientListPerm_[clientID].end()); // make sure the object i is in the client list
    assert(clientListPerm_[clientID].find(j.objID) != clientListPerm_[clientID].end()); // make sure the object j is in the client list
    
    int prio1 = clientListPerm_[clientID][i.objID].objValuePerm + clientListPerm_[clientID][i.objID].objValueSched;
    int prio2 = clientListPerm_[clientID][j.objID].objValuePerm + clientListPerm_[clientID][j.objID].objValueSched;
//     int prio1 = clientListPerm_[clientID][i.objID].objID;
//     int prio2 = clientListPerm_[clientID][j.objID].objID;
    // NOTE: smaller priority is better
    return prio1 < prio2;
  }


	void TrafficControl::processObjectList(unsigned int clientID, unsigned int gamestateID, std::list<obj> *list)
	{
// 	  copiedVector = *list;
	  currentClientID=clientID;
	  currentGamestateID=gamestateID;
	  evaluateList(clientID, list);
	  //list hatte vorher ja vielmehr elemente, nach zuweisung nicht mehr... speicherplatz??
// 	  *list=copiedVector;
    //später wird copiedVector ja überschrieben, ist das ein problem für list-dh. für gamestatemanager?
	  return;
	}
  
  TrafficControl *TrafficControl::getInstance()
  {
    assert(instance_);
    return instance_;
  }
	
	void TrafficControl::ack(unsigned int clientID, unsigned int gamestateID)
	{
	  std::list<obj>::iterator itvec;  // iterator to iterate through the acked objects
    
    //assertions to make sure the maps already exist
    assert(clientListTemp_.find(clientID) != clientListTemp_.end() );
    assert(clientListPerm_.find(clientID) != clientListPerm_.end() );
	  assert( clientListTemp_[clientID].find(gamestateID) != clientListTemp_[clientID].end() );
    
    for(itvec = clientListTemp_[clientID][gamestateID].begin(); itvec != clientListTemp_[clientID][gamestateID].end(); itvec++)
	  {
      if(clientListPerm_[clientID].find((*itvec).objID) != clientListPerm_[clientID].end()) // check whether the obj already exists in our lists
      {
        clientListPerm_[clientID][(*itvec).objID].objCurGS = gamestateID;
        clientListPerm_[clientID][(*itvec).objID].objValueSched = 0; //set scheduling value back
      }
      else
      {
        assert(0);
        clientListPerm_[clientID][(*itvec).objID].objCurGS = gamestateID;
        clientListPerm_[clientID][(*itvec).objID].objID = (*itvec).objID;
        clientListPerm_[clientID][(*itvec).objID].objCreatorID = (*itvec).objCreatorID;
        clientListPerm_[clientID][(*itvec).objID].objSize = (*itvec).objSize;
      }
	  }
	   // remove temporary list (with acked objects) from the map
    clientListTemp_[clientID].erase( clientListTemp_[clientID].find(gamestateID) );
	}

/**
*Definition of private members
*/
	
	
	/**
	*copyList gets list of Gamestate Manager and turns it to *listToProcess
	*/
// 	void TrafficControl::copyList(std::list<obj> *list)
// 	{
// 	  std::list<obj>::iterator itvec;
// 	  for(itvec = (*list).begin(); itvec != (*list).end(); itvec++)
// 	  {
// 	    objInfo objectA;
// 	    objectA.objCreatorID=(*itvec).objCreatorID;
// 	    objectA.objSize = (*itvec).objSize;
// 	    listToProcess_.insert(std::pair<currentClientID, map<(*itvec).objID,objectA>>);//unsicher: ob map<...> so richtig ist
// 	  }
// 	}
	/**
	*updateReferenceList compares the sent list by GSmanager with the current *reference list and updates it.
	*returns void
	*/
// 	void TrafficControl::updateReferenceList(std::map<unsigned int, objInfo> *list)
// 	{
// 	  std::map<unsigned int, Synchronisable*>::iterator itref;
// 	  std::map<unsigned int, objInfo>::iterator itproc;
// 	  for(itproc=listToProcess_.begin(); itproc != listToProcess_.end(); itproc++)
// 	  {
// 	    //itproc->first=objectid that is looked for
// 	    if(referenceList_->find(itproc->first))
// 	    {
// 	      continue;
// 	    }
// 	    else
// 	    {
// 	      (*referenceList_).insert(pair<unsigned int,          Synchronisable*>((*itproc).first,Synchronisable::getSynchronisable((*itproc).first)));//important: how to get adress of an object!
// 	      insertinClientListPerm(currentClientID,itproc->first,itproc->second);
// 	    }
// 	  }
// 	}
	/**
	*updateClientListPerm
	*returns void
	*/
	void TrafficControl::insertinClientListPerm(unsigned int clientID, obj objinf)
	{ 
	  std::map<unsigned int,std::map<unsigned int, objInfo> >::iterator itperm;//iterator clientListPerm over clientIDs
// 	  itperm = (clientListPerm_).find(clientID);
// 	  assert(itperm != clientListPerm_.end() );
    unsigned int gsid=GAMESTATEID_INITIAL, gsdiff=currentGamestateID, prioperm=Synchronisable::getSynchronisable(objinf.objID)->getPriority(), priomom=0;
    clientListPerm_[clientID][objinf.objID] = objInfo(objinf.objID, objinf.objCreatorID,gsid,gsdiff, objinf.objSize,prioperm,priomom);
// 	  itperm->second.insert(std::pair<unsigned int, objInfo>(objid,objinf));
//     permObjPrio_.insert(objid, objinf.objValuePerm);
	}
	
  /**
  * updateClientListTemp
  * takes the shortened list which will be sent to the gsmanager and puts the *info into clientListTemp
  */	
  void TrafficControl::updateClientListTemp(std::list<obj> *list)
  {
    clientListTemp_[currentClientID][currentGamestateID] = std::list<obj>(*list);
//     std::list<obj>::iterator itvec;
//     std::map<unsigned int,std::map<unsigned int, obj> >::iterator ittemp;
//     std::map<unsigned int, obj>::iterator ittempgs;
//     ittemp = clientListTemp_.find(currentClientID);
//     ittempgs = (*ittemp).find(currentGamestateID);
//     for(itvec = list.begin(); itvec!=list.end(), itvec++)
//     {
//       ittempgs.insert(itvec);static
//     }
  }

  /**
  *cut
  *takes the current list that has to be returned to the gsmanager and shortens it in criteria of bandwidth of clientID(XY)
  */
  void TrafficControl::cut(std::list<obj> *list, unsigned int targetsize)
  {
    unsigned int size=0;
    std::list<obj>::iterator itvec, ittemp;
    assert(!list->empty());
    for(itvec = list->begin(); itvec != list->end();)
    {
      assert( (*itvec).objSize < 1000);
//       COUT(0) << "==targetsize==  " << targetsize << endl;
      if ( ( size + (*itvec).objSize ) < targetsize )
      {
//         COUT(0) << "no cut" << endl;
        size += (*itvec).objSize;//objSize is given in bytes
        ++itvec;
      }
      else
      {
        COUT(0) << "cut" << endl;
        clientListPerm_[currentClientID][(*itvec).objID].objValueSched += SCHED_PRIORITY_OFFSET; // NOTE: SCHED_PRIORITY_OFFSET is negative
//         ittemp = itvec;
        list->erase(itvec++);
//         itvec = ittemp;
      }
//       printList(list, currentClientID);
    }
    assert(!list->empty());
  }


	/**
	*evaluateList evaluates whether new obj are there, whether there are things to be updatet and manipulates all this.
	*/
	void TrafficControl::evaluateList(unsigned int clientID, std::list<obj> *list)
	{
	  //copyList(list);
	
	  //now the sorting
	
	  //compare listToProcess vs clientListPerm
    //if listToProcess contains new Objects, add them to clientListPerm
// 	  std::map<unsigned int, objInfo>::iterator itproc;
    std::list<obj>::iterator itvec;
// 	  std::map<unsigned int, std::map<unsigned int, objInfo> >::iterator itperm;
// 	  std::map<unsigned int, objInfo>::iterator itpermobj;
	  for( itvec=list->begin(); itvec != list->end(); itvec++)
	  {
// 	    itperm = clientListPerm_.find(clientID);
	    if ( clientListPerm_[clientID].find( (*itvec).objID) != clientListPerm_[clientID].end() )
      {
        // we already have the object in our map
        //obj bleibt in liste und permanente prio wird berechnet
        clientListPerm_[clientID][(*itvec).objID].objDiffGS = currentGamestateID - clientListPerm_[clientID][(*itvec).objID].objCurGS;
//         ((*itpermobj).second).objDiffGS = ((*itpermobj).second).objCurGS - currentGamestateID;
//         permprio = clientListPerm_[clientID][(*itproc).first].objValuePerm;
//         itpermprio = (permObjPrio_).find((*itproc).first);
//         ((*itpermobj).second).objValuePerm = ((*itpermobj).second).objDiffGS * (*itpermprio).second;
        continue;//check next objId
      }
      else
      {
        // insert the object into clientListPerm
        insertinClientListPerm(clientID,*itvec);
//         itpermobj=(*itperm).find((*itproc).first)
//         ((*itpermobj).second).objDiffGS = ((*itpermobj).second).objCurGS - currentGamestateID;
//         itpermprio = (permObjPrio_).find((*itproc).first);
//         ((*itpermobj).second).objValuePerm = ((*itpermobj).second).objDiffGS * (*itpermprio).second;
        continue;//check next objId
      }
    }
	  //end compare listToProcess vs clientListPerm
	
    //listToProc vs clientListTemp
    //TODO: uncomment it again and change some things
    /*
    std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator ittemp;
    std::map<unsigned int, unsigned int>::iterator ittempgs;
    for( itproc=listToProcess_.begin(); itproc != listToProcess_.end();itproc++)
    {
      ittemp = clientListTemp_->find(currentClientID);
      if( ittempgs = (*ittemp).find(currentGamestateID))
      {
        if((*itproc).first == (*ittempgs).find((*itproc).first))//ja, dann ist objekt schon in der zu sendenden liste-muss nicht nochmal gesendet werden
        {
          (listToProcess_).erase (itproc);
        }
        else
          continue;
      }
      else
        continue;
    }*/
    //end listToProc vs clientListTemp
    
    //TODO: check whether we need this, cause i don't think so.
    //listToProcess contains obj to send now, and since we give gsmanager the copiedlist and not listToProcess shorten copiedlist therefor too.
    /*std::list<obj>::iterator itvec;
    for(itvec = copiedVector.begin(); itvec != copiedVector.end(); itvec++)
    {
      if ( listToProcess_.find(itvec->objID) )
      {
        continue;//therefore object wasnt thrown out yet and has to be sent back to gsmanager
      }
      else
      {
        copiedVector.remove(itvec);
      }
    }*/
    //sort copied list aufgrund der objprioperm in clientlistperm
    // use boost bind here because we need to pass a memberfunction to stl sort
    list->sort(boost::bind(&TrafficControl::priodiffer, this, clientID, _1, _2) );
    
    //now we check, that the creator of an object always exists on a client
//     printList(list, clientID);
    std::list<obj>::iterator itcreator;
    for(itvec = list->begin(); itvec != list->end(); itvec++)
    { 
      fixCreatorDependencies(itvec, list, clientID);
    }
    //end of sorting
    //now the cutting, work the same obj out in processobjectlist and copiedlist, compression rate muss noch festgelegt werden. 
    cut(list, targetSize);
//     printList(list, clientID);
    //diese Funktion updateClientList muss noch gemacht werden
    updateClientListTemp(list);
    //end of sorting
  }

  void TrafficControl::printList(std::list<obj> *list, unsigned int clientID)
  {
    std::list<obj>::iterator it;
    COUT(0) << "=========== Objectlist ===========" << endl;
    for( it=list->begin(); it!=list->end(); it++)
      COUT(0) << "ObjectID: " << (*it).objID << " creatorID: " << (*it).objCreatorID << " Priority: " << clientListPerm_[clientID][(*it).objID].objValuePerm + clientListPerm_[clientID][(*it).objID].objValueSched << " size: " << (*it).objSize << endl;
  }
  
  void TrafficControl::fixCreatorDependencies(std::list<obj>::iterator it1, std::list<obj> *list, unsigned int clientID)
  {
    if ( (*it1).objCreatorID == OBJECTID_UNKNOWN )
      return;
    if( clientListPerm_[clientID][(*it1).objCreatorID].objCurGS != GAMESTATEID_INITIAL )
      return;
    std::list<obj>::iterator it2, it3=it1;
    for( it2 = ++it3; it2 != list->end(); it2++ )
    {
      if( (*it2).objID == (*it1).objCreatorID )
      {
        it3 = list->insert(it1, *it2); //insert creator before it1
        list->erase(it2);
//         printList(list, clientID);
        fixCreatorDependencies( it3, list, clientID );
        break;
      }
    }
  }
  
  void TrafficControl::clientDisconnected(unsigned int clientID)
  {
    assert(clientListTemp_.find(clientID) != clientListTemp_.end() );
    assert(clientListPerm_.find(clientID) != clientListPerm_.end() );
    clientListTemp_.erase(clientListTemp_.find(clientID));
    clientListPerm_.erase(clientListPerm_.find(clientID));
  }

/*
void bvlabla(list *a){
//sort a
list *cache;
cache = new list<unsigned int>(*a);
return a;
}
*/


}//namespace network
