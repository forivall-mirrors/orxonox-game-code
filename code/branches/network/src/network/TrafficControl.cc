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

#include <cassert>
#include <boost/bind.hpp>

namespace network {

/**
*Initializing protected members
*/
	TrafficControl *TrafficControl::instance_=0;
	
	/**
	* @brief Constructor: assures that only one reference will be created and sets the pointer
	*/
	TrafficControl::TrafficControl()
	{
	  assert(instance_=0);
	  instance_=this;
    targetSize = 5000;//5000bytes
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

        /**
                *eigener sortieralgorithmus
        */
        bool TrafficControl::priodiffer(obj i, obj j)
        {
          std::map<unsigned int, objInfo>::iterator iti;
          std::map<unsigned int, objInfo>::iterator itj;
          iti=listToProcess_.find(i.objID);
          itj=listToProcess_.find(j.objID);
          return iti->second.objValuePerm < itj->second.objValuePerm;
        }



	std::vector<obj>* TrafficControl::processObjectList(unsigned int clientID, unsigned int gamestateID, std::vector<obj> *list)
	{
	  copiedVector = *list;
	  currentClientID=clientID;
	  currentGamestateID=gamestateID;
	  evaluateList(list);
	  //list hatte vorher ja vielmehr elemente, nach zuweisung nicht mehr... speicherplatz??
	  *list=copiedVector;
          //später wird copiedVector ja überschrieben, ist das ein problem für list-dh. für gamestatemanager?
	  return list;
	}
	
	void TrafficControl::processAck(unsigned int clientID, unsigned int gamestateID)
	{
	  std::map<unsigned int, std::map<unsigned int, std::vector<objInfo> > >::iterator itperm;//iterator clientListPerm
	  std::map<unsigned int, std::vector<objInfo> >::iterator itpermObjectIDList;//iterator over objectid
    std::vector<objInfo>::iterator itpermObjectID;
    std::map<unsigned int, std::map<unsigned int, std::vector<obj> > >::iterator ittemp;//iterator clientListTemp, iterates over clientIDs
	  std::map<unsigned int, std::vector<obj> >::iterator ittempgs;//iterator that iterates over gsIDs of a client
	  std::vector<obj>::iterator itvec;
	  //following code helps to put clientListTemp infos into clientListPerm infos
	  ittemp = clientListTemp_.find(clientID);
	  assert(ittemp != clientListTemp_.end() ); //muss da nicht was anderes überprüft werden? -> nein
	  itperm = clientListPerm_.find(clientID);
	  assert(itperm != clientListPerm_.end() );
	  ittempgs = ittemp->second.find(gamestateID);
	  assert( ittempgs != ittemp->second.end() );//gleiche frage wie vorher -> nein
	  for(itvec = ittempgs->second.begin(); itvec != ittempgs->second.end(); itvec++)
	  {
      bool alreadyExisting = false;
      itpermObjectIDList = itperm->second.find(gamestateID);
      assert(itpermObjectIDList != itperm->second.end());
      for (itpermObjectID = itpermObjectIDList->second.begin(); itpermObjectID != itpermObjectIDList->second.end(); itpermObjectID++ )
      {
        if ( (*itpermObjectID).objID == (*itvec).objID)
        {
          (*itpermObjectID).objCurGS = gamestateID;
          alreadyExisting = true;
          break;
        }
        if ( !alreadyExisting )
        {
          objInfo objinf;    // TODO: is this initialisation complete ?
          objinf.objCurGS = gamestateID;
          objinf.objID = (*itvec).objID;
          insertinClientListPerm(clientID, (*itvec).objID, objinf);
        }
      }
	  }
	   // remove temporary vector (with acked objects) from the map
    ittemp->second.erase(ittempgs);
	}

/**
*Definition of private members
*/
	
	
	/**
	*copyList gets vector of Gamestate Manager and turns it to *listToProcess
	*/
	void TrafficControl::copyList(std::vector<obj> *list)
	{
	  std::vector<obj>::iterator itvec;
	  for(itvec = (*list).begin(); itvec != (*list).end(); itvec++)
	  {
	    objInfo objectA;
	    objectA.objCreatorID=(*itvec).objCreatorID;
	    objectA.objSize = (*itvec).objSize;
	    listToProcess_.insert(pair<currentClientID, map<(*itvec).objID,objectA>>);//unsicher: ob map<...> so richtig ist
	  }
	}
	/**
	*updateReferenceList compares the sent list by GSmanager with the current *reference list and updates it.
	*returns void
	*/
	void TrafficControl::updateReferenceList(std::map<unsigned int, objInfo> *list)
	{
	  std::map<unsigned int, Synchronisable*>::iterator itref;
	  std::map<unsigned int, objInfo>::iterator itproc;
	  for(itproc=(*listToProcess_).begin();itproc != (*listToProcess_).end(); itproc++)
	  {
	    //itproc->first=objectid that is looked for
	    if(referenceList_->find(itproc->first))
	    {
	      continue;
	    }
	    else
	    {
	      (*referenceList_).insert(pair<unsigned int,          Synchronisable*>((*itproc).first,Synchronisable::getSynchronisable((*itproc).first)));//important: how to get adress of an object!
	      insertinClientListPerm(currentClientID,itproc->first,itproc->second);
	    }
	  }
	}
	/**
	*updateClientListPerm
	*returns void
	*/
	void TrafficControl::insertinClientListPerm(unsigned int clientid, unsigned int objid, objInfo objinf)
	{ 
	  std::map<unsigned int,std::map<unsigned int, objInfo>>::iterator itperm;//iterator clientListPerm over clientIDs
	  itperm = (clientListPerm_).find(clientiD);
	  assert(itperm != clientListPerm_.end() );
	  (*itperm).insert(pair<unsigned int, objInfo>(objid,objinf));
          (permObjPrio_).insert(objid,objinf.objValuePerm);
	}
	
        /**
        *updateClientListTemp
        *takes the shortened vector which will be sent to the gsmanager and puts the *info into clientListTemp
        */	
        TrafficControl::updateClientListTemp(std::vector<obj> *list)
        {
          vector<obj>::iterator itvec;
          map<unsigned int,std::map<unsigned int, obj>>::iterator ittemp;
          map<unsigned int, obj>::iterator ittempgs;
          ittemp = (clientListTemp_).find(currentClientID);
          ittempgs = (*ittemp).find(currentGamestateID);
          for(itvec=list.begin();itvec!=list.end(),itvec++)
          {
            ittempgs.insert(itvec);
          }
        }

        /**
        *cut
        *takes the current vector that has to be returned to the gsmanager and shortens it in criteria of bandwidth of clientID(XY)
        */
        TrafficControl::cut(std::vector<obj> *list,int targetsize)
        {
          unsigned int size=0;
          vector<obj>::iterator itvec;
          itvec = list.begin();
          unsigned int i=0;
	  while(size<targetsize && (itvec!=list.end()))
          {
            size = size + (*itvec).objSize;//objSize is given in bytes!??
            i++;
            itvec = list.begin()+i;
          }
          list.erase(itvec, list.end());
	}


	/**
	*evaluateList evaluates whether new obj are there, whether there are things to be updatet and manipulates all this.
	*/
	void TrafficControl::evaluateList(std::vector<obj> *list)
	{
	  copyList(list);
	
	  //now the sorting
	
	  //compare listToProcess vs clientListPerm
          //if listToProcess contains new Objects, add them to clientListPerm
	  std::map<unsigned int, objInfo>::iterator itproc;
	  std::map<unsigned int, std::map<unsigned int, objInfo> >::iterator itperm;
	  std::map<unsigned int, objInfo>::iterator itpermobj;
	  std::map<unsigned int, unsigned int>::iterator itpermprio;
	  for((*itproc)=(listToProcess_).begin(); itproc != (listToProcess_).end();it++)
	  {
	    itperm=(clientListPerm_).find(currentClientID);
	    if(itpermobj=(*itperm).find((*itproc).first))
            {
	     if(currentGamestateID > ((*itpermobj).second).objCurGS)
	      {
	      //obj bleibt in liste und permanente prio wird berechnet
	        ((*itpermobj).second).objDiffGS = ((*itpermobj).second).objCurGS - currentGamestateID;
	        itpermprio = (permObjPrio_).find((*itproc).first);
	        ((*itpermobj).second).objValuePerm = ((*itpermobj).second).objDiffGS * (*itpermprio).second;
	        continue;//check next objId
	      }
	      else
	      {
	        (listToProcess_).erase(itproc);
	      }
             }
             else
             {
               insertinClientListPerm(currentClientID,(*itproc).first,(*itproc).second);
               itpermobj=(*itperm).find((*itproc).first)
               ((*itpermobj).second).objDiffGS = ((*itpermobj).second).objCurGS - currentGamestateID;
	       itpermprio = (permObjPrio_).find((*itproc).first);
	       ((*itpermobj).second).objValuePerm = ((*itpermobj).second).objDiffGS * (*itpermprio).second;
	       continue;//check next objId
             }
	    }
	  //end compare listToProcess vs clientListPerm
	
	//listToProc vs clientListTemp
	map<unsigned int, std::map<unsigned int, unsigned int> >::iterator ittemp;
	map<unsigned int, unsigned int>::iterator ittempgs;
	for((itproc=listToProcess_).begin(); itproc != (listToProcess_).end();itproc++)
	{
	  ittemp = clientListTemp_->find(currentClientID);
	  if(ittempgs = (*ittemp).find(currentGamestateID))
	  {
	    if((*itproc).first == (*ittempgs).find((*itproc).first))//ja, dann ist objekt schon in der zu sendenden liste-muss nicht nochmal gesendet werden
	    {
 	      (listToProcess_).erase (itproc);
	    }
            else continue;
          }
	  else continue;
	}
	//end listToProc vs clientListTemp
	
	//listToProcess contains obj to send now, and since we give gsmanager the copiedvector and not listToProcess shorten copiedvector therefor too.
	vector<obj>::iterator itvec;
	for(itvec = copiedVector.begin(); itvec < copiedVector.end(); itvec++)
	{
	  if((listToProcess_).find(itvec->objID))
	  {
            continue;//therefore object wasnt thrown out yet and has to be sent back to gsmanager
          }
	  else
	  {
	    copiedVector.remove(itvec);
	  }
	}
	//sort copied vector aufgrund der objprioperm in clientlistperm
        // use boost bind here because we need to pass a memberfunction to stl sort
        sort(copiedvector.begin(), copiedvector.end(), boost::bind(&TrafficControl::priodiffer,this,_1,_2) );
	//swappen aufgrund von creator oder ganz rausnehmen!?
	for(itvec = copiedVector.begin(); itvec < copiedVector.end(); itvec++)
	{ 
	  itproc = (listToProcess_).find((*itvec).objID);
	  if((*itproc).second.objCreatorID)
	  {
	  //vor dem child in copiedvector einfügen, wie?
	    copiedVector.insert(copiedVector.find((*itproc).first),(*itproc).second.objCreatorID);
	  }
	  else continue;
	}
	  //end of sorting
	  //now the cutting, work the same obj out in processobjectlist and copiedvector, compression rate muss noch festgelegt werden. 
	  cut(copiedVector,targetSize);
	  //diese Funktion updateClientList muss noch gemacht werden
	  updateClientListTemp(copiedVector);
	  //end of sorting
      }




/*
void bvlabla(vector *a){
//sort a
vector *cache;
cache = new vector<unsigned int>(*a);
return a;
}
*/


}//namespace network
