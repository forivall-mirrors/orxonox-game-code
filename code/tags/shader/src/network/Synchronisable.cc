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
 *      Dumeni Manatschal, (C) 2007
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

//
// C++ Implementation: synchronisable
//
// Description:
//
//
// Author:  Dumeni, Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//

#include "Synchronisable.h"

#include <string>
#include <iostream>

#include "core/CoreIncludes.h"

namespace network
{
  
  
  int Synchronisable::state_=0x1; // detemines wheter we are server (default) or client
  
  /**
  * Constructor:
  * calls registarAllVariables, that has to be implemented by the inheriting classID
  */
  Synchronisable::Synchronisable(){
    RegisterRootObject(Synchronisable);
    static int idCounter=0;
    datasize=0;
    objectID=idCounter++;
    syncList = new std::list<synchronisableVariable *>;
    //registerAllVariables();
  }

  Synchronisable::~Synchronisable(){
  }
  
  bool Synchronisable::create(){
    this->classID = this->getIdentifier()->getNetworkID();
    COUT(4) << "creating synchronisable: setting classid from " << this->getIdentifier()->getName() << " to: " << classID << std::endl;
    return true;
  }
  
  void Synchronisable::setClient(bool b){
    if(b) // client
      state_=0x2;
    else  // server
      state_=0x1;
  }

  /**
  * This function is used to register a variable to be synchronized
  * also counts the total datasize needed to save the variables
  * @param var pointer to the variable
  * @param size size of the datatype the variable consists of
  */
  void Synchronisable::registerVar(void *var, int size, variableType t, int mode){
    // create temporary synch.Var struct
    synchronisableVariable *temp = new synchronisableVariable;
    temp->size = size;
    temp->var = var;
    temp->mode = mode; 
    temp->type = t;
    COUT(5) << "Syncronisable::registering var with size: " << temp->size << " and type: " << temp->type << std::endl; 
    // increase datasize
    datasize+=sizeof(int)+size;
    //std::cout << "push temp to syncList (at the bottom) " << datasize << std::endl;
    COUT(5) << "Syncronisable::objectID: " << objectID << " this: " << this << " name: " << this->getIdentifier()->getName() << " networkID: " << this->getIdentifier()->getNetworkID() << std::endl;
    syncList->push_back(temp);
  }

  /**
  * note: only use this function for debug use, because it's inefficient (in order to produce a gamestate, you have to copy the whole data again to another memory location after this process)
  * This function takes all SynchronisableVariables out of the Synchronisable and saves it into a syncData struct
  * structure of the bitstream:
  * (var1_size,var1,var2_size,var2,...)
  * varx_size: size = sizeof(int)
  * varx: size = varx_size
  * @return data containing all variables and their sizes
  */
  // syncData Synchronisable::getData(){
  //   std::list<synchronisableVariable>::iterator i;
  //   int totalsize=0;
  //   //figure out size of data to be allocated
  //   for(i=syncList->begin(); i!=syncList->end(); i++){
  //     // increase size (size of variable and size of size of variable ;)
  //     if(i->type == STRING)
  //       totalsize+=sizeof(int)+((std::string *)i->var)->length()+1;
  //     else
  //       totalsize+=sizeof(int)+i->size;
  //   }
  //   syncData retVal;
  //   retVal.objectID=this->objectID;
  //   retVal.classID=this->classID;
  //   retVal.length=totalsize;
  //   // allocate memory
  //   retVal.data = (unsigned char *)malloc(totalsize);
  //   // copy to location
  //   //CHANGED: REMOVED DECLARATION int n=0 FROM LOOP
  //   int n=0;
  //   for(i=syncList->begin(); n<totalsize && i!=syncList->end(); i++){
  //     std::memcpy(retVal.data+n, (const void*)(i->size), sizeof(int));
  //     n+=sizeof(int);
  //     switch(i->type){
  //     case STRING:
  //       std::memcpy(retVal.data+n, (const void *)(((std::string *)i->var)->c_str()), ((std::string *)i->var)->length()+1);
  //       n+=((std::string *)i->var)->length()+1;
  //       break;
  //     case DATA:
  //       std::memcpy(retVal.data+n, ((const void*)i->var), i->size);
  //       n+=i->size;
  //       break;
  //     }
  //   }
  //   return retVal;
  // }
  /**
  * This function takes all SynchronisableVariables out of the Synchronisable and saves it into a syncData struct
  * Difference to the above function:
  * takes a pointer to already allocated memory (must have at least getSize bytes length)
  * structure of the bitstream:
  * (var1_size,var1,var2_size,var2,...)
  * varx_size: size = sizeof(int)
  * varx: size = varx_size
  * @return data containing all variables and their sizes
  */
  syncData Synchronisable::getData(unsigned char *mem, int mode){
    //std::cout << "inside getData" << std::endl;
    if(mode==0x0)
      mode=state_;
    if(classID==0)
      COUT(3) << "classid 0 " << this->getIdentifier()->getName() << std::endl;
    this->classID=this->getIdentifier()->getNetworkID();
    std::list<synchronisableVariable *>::iterator i;
    syncData retVal;
    retVal.objectID=this->objectID;
    retVal.classID=this->classID;
    retVal.length=getSize();
    COUT(5) << "Synchronisable getting data from objectID: " << retVal.objectID << " classID: " << retVal.classID << " length: " << retVal.length << std::endl;
    retVal.data=mem;
    // copy to location
    int n=0; //offset
    for(i=syncList->begin(); n<datasize && i!=syncList->end(); ++i){
      //(std::memcpy(retVal.data+n, (const void*)(&(i->size)), sizeof(int));
      if( ((*i)->mode & mode) == 0 ){
        COUT(5) << "not getting data: " << std::endl;
        continue;  // this variable should only be received
      }
      switch((*i)->type){
      case DATA:
        std::memcpy( (void *)(retVal.data+n), (void*)((*i)->var), (*i)->size);
        n+=(*i)->size;
        break;
      case STRING:
        memcpy( (void *)(retVal.data+n), (void *)&((*i)->size), sizeof(int) );
        n+=sizeof(int);
        const char *data = ( ( *(std::string *) (*i)->var).c_str());
        std::memcpy( retVal.data+n, (void*)data, (*i)->size);
        COUT(5) << "synchronisable: char: " << (const char *)(retVal.data+n) << " data: " << data << " string: " << *(std::string *)((*i)->var) << std::endl;
        n+=(*i)->size;
        break;
      }
    }
    return retVal;
  }

  /**
  * This function takes a syncData struct and takes it to update the variables
  * @param vars data of the variables
  * @return true/false
  */
  bool Synchronisable::updateData(syncData vars, int mode){
    if(mode==0x0)
      mode=state_;
    unsigned char *data=vars.data;
    std::list<synchronisableVariable *>::iterator i;
    if(syncList->empty()){
      COUT(4) << "Synchronisable::updateData syncList is empty" << std::endl;
      return false;
    }
    COUT(5) << "Synchronisable: objectID " << vars.objectID << ", classID " << vars.classID << " size: " << vars.length << " synchronising data" << std::endl;
    for(i=syncList->begin(); i!=syncList->end(); i++){
      if( ((*i)->mode ^ mode) == 0 ){
        COUT(5) << "synchronisable: not updating variable " << std::endl;
        continue;  // this variable should only be set
      }
      COUT(5) << "Synchronisable: element size: " << (*i)->size << " type: " << (*i)->type << std::endl;
      switch((*i)->type){
      case DATA:
        memcpy((void*)(*i)->var, data, (*i)->size);
        data+=(*i)->size;
        break;
      case STRING:
        (*i)->size = *(int *)data;
        COUT(5) << "string size: " << (*i)->size << std::endl;
        data+=sizeof(int);
        *((std::string *)((*i)->var)) = std::string((const char*)data);
        COUT(5) << "synchronisable: char: " << (const char*)data << " string: " << std::string((const char*)data) << std::endl;
        data += (*i)->size;
        break;
      }
    }
    return true;
  }

  /**
  * This function returns the total amount of bytes needed by getData to save the whole content of the variables
  * @return amount of bytes
  */
  int Synchronisable::getSize(int mode){
    int tsize=0;
    if(mode==0x0)
      mode=state_;
    std::list<synchronisableVariable *>::iterator i;
    for(i=syncList->begin(); i!=syncList->end(); i++){
      if( ((*i)->mode & mode) == 0 )
        continue;  // this variable should only be received, so dont add its size to the send-size
      switch((*i)->type){
      case DATA:
        tsize+=(*i)->size;
        break;
      case STRING:
        tsize+=sizeof(int);
        (*i)->size=((std::string *)(*i)->var)->length()+1;
        COUT(5) << "String size: " << (*i)->size << std::endl;
        tsize+=(*i)->size;
        break;
      }
    }
    return tsize;
  }
  
  void Synchronisable::setBacksync(bool sync){
    backsync_=sync;
  }

  bool Synchronisable::getBacksync(){
    return backsync_;
  }
  
}