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

namespace network {

/**
*Initializing protected members
*/
TrafficControl *TraffiControl::instance_=0;

/**
 * @brief Constructor: assures that only one reference will be created and sets the pointer
 */
TrafficControl::TrafficControl()
{
  assert(instance_=0);
  instance_=this;
  clientListPerm_=new std::map<unsigned int,std::map<unsigned int, unsigned int>>;
  referenceList_=new std::map<unsigned int, Synchronisable*>;
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

TrafficControl::processObjectList(unsigned int clientID, unsigned int gamestateID, std::vector<obj> *list)
{
  currentClientID=clientID;
  currentGamestateID=gamestateID;
  list = evaluateList(list);
  return list;
}

/**
*Definition of private members
*/

//brauch ich die noch??
TrafficControl::createReferenceList(Synchronisable *list)
{
  map<unsigned int, Synchronisable*>::iterator itref;
  itref=referenceList_->begin();
  referenceList_->insert(itref,pair<unsigned int, Synchronisable*>(itref->getObjectID,itref->getSynchronisable));
}
//end brauch ich die noch

/**
*copyList gets vector of Gamestate Manager and turns it to *listToProcess
*/
TrafficControl::copyList(std::map<obj> *list)
{
  listToProcess_=new std::map<unsigned int,std::map<unsigned int, unsigned int>>;
  vector<obj>::iterator itvec;
  for(itvec = list->begin(); itvec < list->end(); itvec++)
  {
   objInfo * objectA = new objInfo;
   objectA.objCreatorID=*itvec.objCreatorID;
   objectA.objSize = *itvec.objSize;
   listToProcess_->insert(pair<currentClientID, map<*itvec.objID,objectA>)//unsicher: ob map<...> so richtig ist
  }
}
/**
*updateReferenceList compares the sent list by GSmanager with *the current reference list and updates it.
*returns void
*/
TrafficControl::updateReferenceList(std::map<unsigned int, objInfo> *list)
{
  map<unsigned int, Synchronisable*>::iterator itref;
  map<unsigned int, objInfo>::iterator itproc;
  for(itproc=listToProcess_->begin();itproc != listToProcess_->.end(); itproc++)
  {
    //itproc->first=objectid that is looked for
    if(referenceList_->find(itproc->first))
    {
      continue;
    }
    else
    {
      referenceList_->insert(pair<unsigned int, Synchronisable*>(itproc->first,Synchronisable::getSynchronisable(itproc->first));
      insertinClientListPerm(itproc->first,itproc->second);
    }
  }
}
/**
*updateClientListPerm
*returns void
*/
TrafficControl::insertinClientListPerm(unsigned int objid, objInfo objinf)
{
  clientListPerm_->insert(pair<unsigned int,objInfo>(objid,objinf));
}


/**
*evaluateList evaluates whether new obj are there, whether there are things to be updatet and manipulates all this.
*/
TrafficControl::evaluateList(std::map<obj> *list)
{
  copyList(list);
  updateReferenceList(listToProcess_);
  
  //now the sorting

  //compare listToProcess vs clientListPerm
  map<unsigned int, objInfo>::iterator itproc;
  map<unsigned int,std::map<unsigned int, objInfo>>::iterator itperm;
  map<unsigned int, objInfo>::iterator itpermobj;
  map<unsigned int, unsigned int>::iterator itpermprio
  for(itproc=listToProcess_->begin(); itproc != listToProcess_->end();it++)
  {
    itperm=clientListPerm->find(currentClientID);
    itpermobj=itperm->find(itproc->first);
    if(currentGamestateID < (itpermobj->second).objCurGS)
    {
      //obj bleibt in liste und permanente prio wird berechnet
      (itpermobj->second).objDiffGS = (itpermobj->second).objCurGS - currentGamestateID;
      itpermprio = permObjPrio_->find(itproc->first);
      (itpermobj->second).objValuePerm = (itpermobj->second).objDiffGS * itpermprio->second;
      continue;//check next objId
    }
    else
    {
      listToProcess_->erase (itproc);
    }
  }
  //end compare listToProcess vs clientListPerm

  //listToProc vs clientListTemp
  map<unsigned int, std::map<unsigned int, unsigned int>>::iterator ittemp;
  map<unsigned int, unsigned int>::iterator ittempgs;
  for(itproc=listToProcess_->begin(); itproc != listToProcess_->end();it++)
  {
    ittemp = clientListTemp_->find(currentClientID);
    if(ittempgs = ittemp->find(currentGamestateID))
    {
      if(itproc->first == ittempgs->find(itproc->first))
      {
        listToProcess_->erase (itproc);
      }
      else continue;
    }
    else continue;
  }
  //end listToProc vs clientListTemp
  
  //listToProcess contains obj to send now, shorten copiedvector therefor too.
  vector<obj>::iterator itvec;
  for(itvec = copiedvector.begin(); itvec < copiedvector.end(); itvec++)
  {
    if(listToProcess_->find(itvec->objID))
    {
      continue;
    }
    else
    {
      copiedvector.remove(itvec);
    }
  }
  //sort copied vector aufgrund der objprioperm in clientlistperm
  sort(copiedvector.begin(),copiedvector.end(),priodiffer);
  //swappen aufgrund von creator oder ganz rausnehmen!?

  //end of sorting
  return evaluatedList_;
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
