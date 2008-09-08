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
#include <assert.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
// #include "core/Identifier.h"

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
    objectFrequency_=1;
    objectID=idCounter++;
    syncList = new std::list<synchronisableVariable *>;
    //registerAllVariables();
  }

  Synchronisable::~Synchronisable(){
    // delete callback function objects
    if(!orxonox::Identifier::isCreatingHierarchy())
      for(std::list<synchronisableVariable *>::iterator it = syncList->begin(); it!=syncList->end(); it++)
        delete (*it)->callback;
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
  
  bool Synchronisable::fabricate(unsigned char*& mem, int mode)
  {
    unsigned int size, objectID, classID;
    size = *(unsigned int *)mem;
    objectID = *(unsigned int*)(mem+sizeof(unsigned int));
    classID = *(unsigned int*)(mem+2*sizeof(unsigned int));
    
    if(size==3*sizeof(unsigned int)) //not our turn, dont do anything
      return true;
    
    orxonox::Identifier* id = ID(classID);
    if(!id){
      COUT(3) << "We could not identify a new object; classid: " << classID << " uint: " << (unsigned int)classID << " objectID: " << objectID << " size: " << size << std::endl;
      assert(0);
      return false; // most probably the gamestate is corrupted
    }
    orxonox::BaseObject *bo = id->fabricate();
    Synchronisable *no = dynamic_cast<Synchronisable *>(bo);
    assert(no);
    no->objectID=objectID;
    no->classID=classID;
    COUT(3) << "fabricate objectID: " << no->objectID << " classID: " << no->classID << std::endl;
          // update data and create object/entity...
    if( !no->updateData(mem, mode) ){
      COUT(1) << "We couldn't update the object: " << objectID << std::endl;
      return false;
    }
    if( !no->create() )
    {
      COUT(1) << "We couldn't manifest (create() ) the object: " << objectID << std::endl;
      return false;
    }
    return true;
  }

  /**
  * This function is used to register a variable to be synchronized
  * also counts the total datasize needed to save the variables
  * @param var pointer to the variable
  * @param size size of the datatype the variable consists of
  */
  void Synchronisable::registerVar(void *var, int size, variableType t, int mode, NetworkCallbackBase *cb){
    // create temporary synch.Var struct
    synchronisableVariable *temp = new synchronisableVariable;
    temp->size = size;
    temp->var = var;
    temp->mode = mode;
    temp->type = t;
    temp->callback = cb;
    COUT(5) << "Syncronisable::registering var with size: " << temp->size << " and type: " << temp->type << std::endl;
    // increase datasize
    datasize+=sizeof(int)+size;
    //std::cout << "push temp to syncList (at the bottom) " << datasize << std::endl;
    COUT(5) << "Syncronisable::objectID: " << objectID << " this: " << this << " name: " << this->getIdentifier()->getName() << " networkID: " << this->getIdentifier()->getNetworkID() << std::endl;
    syncList->push_back(temp);
  }
  
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
  bool Synchronisable::getData(unsigned char*& mem, unsigned int id, int mode){
    //std::cout << "inside getData" << std::endl;
    unsigned int tempsize = 0;
    if(mode==0x0)
      mode=state_;
    if(classID==0)
      COUT(3) << "classid 0 " << this->getIdentifier()->getName() << std::endl;
    this->classID=this->getIdentifier()->getNetworkID(); // TODO: correct this
    std::list<synchronisableVariable *>::iterator i;
    unsigned int size;
    size=getSize2(id, mode);
    
    // start copy header
    memcpy(mem, &size, sizeof(unsigned int));
    mem+=sizeof(unsigned int);
    memcpy(mem, &(this->objectID), sizeof(unsigned int));
    mem+=sizeof(unsigned int);
    memcpy(mem, &(this->classID), sizeof(unsigned int));
    mem+=sizeof(unsigned int);
    tempsize+=12;
    // end copy header
    
    //if this tick is we dont synchronise, then abort now
    if(!isMyTick(id))
      return true;
    
    COUT(5) << "Synchronisable getting data from objectID: " << objectID << " classID: " << classID << " length: " << size << std::endl;
    // copy to location
    for(i=syncList->begin(); i!=syncList->end(); ++i){
      //(std::memcpy(retVal.data+n, (const void*)(&(i->size)), sizeof(int));
      if( ((*i)->mode & mode) == 0 ){
        COUT(5) << "not getting data: " << std::endl;
        continue;  // this variable should only be received
      }
      switch((*i)->type){
        case DATA:
          memcpy( (void *)(mem), (void*)((*i)->var), (*i)->size);
          mem+=(*i)->size;
          tempsize+=(*i)->size;
          break;
        case STRING:
          memcpy( (void *)(mem), (void *)&((*i)->size), sizeof(int) );
          mem+=sizeof(int);
          const char *data = ( ( *(std::string *) (*i)->var).c_str());
          memcpy( mem, (void*)data, (*i)->size);
          COUT(5) << "synchronisable: char: " << (const char *)(mem) << " data: " << data << " string: " << *(std::string *)((*i)->var) << std::endl;
          mem+=(*i)->size;
          tempsize+=(*i)->size + 4;
          break;
      }
    }
    assert(tempsize==size);
    return true;
  }

  
  /**
   * This function takes a syncData struct and takes it to update the variables
   * @param vars data of the variables
   * @return true/false
   */
  bool Synchronisable::updateData(unsigned char*& mem, int mode){
    unsigned char *data = mem;
    if(mode==0x0)
      mode=state_;
    std::list<synchronisableVariable *>::iterator i;
    if(syncList->empty()){
      COUT(4) << "Synchronisable::updateData syncList is empty" << std::endl;
      return false;
    }
    
    // start extract header
    unsigned int objectID, classID, size;
    size = *(int *)mem;
    mem+=sizeof(size);
    objectID = *(int *)mem;
    mem+=sizeof(objectID);
    classID = *(int *)mem;
    mem+=sizeof(classID);
    // stop extract header
    assert(this->objectID==objectID);
    assert(this->classID==classID);
    if(size==3*sizeof(unsigned int)) //if true, only the header is available
      return true;
      //assert(0);
    
    COUT(5) << "Synchronisable: objectID " << objectID << ", classID " << classID << " size: " << size << " synchronising data" << std::endl;
    for(i=syncList->begin(); i!=syncList->end() && mem <= data+size; i++){
      if( ((*i)->mode ^ mode) == 0 ){
        COUT(5) << "synchronisable: not updating variable " << std::endl;
        continue;  // this variable should only be set
      }
      COUT(5) << "Synchronisable: element size: " << (*i)->size << " type: " << (*i)->type << std::endl;
      bool callback=false;
      switch((*i)->type){
        case DATA:
          if((*i)->callback) // check whether this variable changed (but only if callback was set)
            if(strncmp((char *)(*i)->var, (char *)mem, (*i)->size)!=0)
              callback=true;
          memcpy((void*)(*i)->var, mem, (*i)->size);
          mem+=(*i)->size;
          break;
        case STRING:
          (*i)->size = *(int *)mem;
          COUT(5) << "string size: " << (*i)->size << std::endl;
          mem+=sizeof(int);
          if((*i)->callback) // check whether this string changed
            if( *(std::string *)((*i)->var) != std::string((char *)mem) )
              callback=true;
          *((std::string *)((*i)->var)) = std::string((const char*)mem);
          COUT(5) << "synchronisable: char: " << (const char*)mem << " string: " << std::string((const char*)mem) << std::endl;
          mem += (*i)->size;
          break;
      }
      // call the callback function, if defined
      if(callback && (*i)->callback)
        (*i)->callback->call();
    }
    return true;
  }

  /**
  * This function returns the total amount of bytes needed by getData to save the whole content of the variables
  * @return amount of bytes
  */
  int Synchronisable::getSize(unsigned int id, int mode){
    if(!isMyTick(id))
      return 0;
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

  /**
   * This function returns the total amount of bytes needed by getData to save the whole content of the variables
   * @return amount of bytes
   */
  int Synchronisable::getSize2(unsigned int id, int mode){
    return sizeof(synchronisableHeader) + getSize( id, mode );
  }
  
  bool Synchronisable::isMyTick(unsigned int id){
//     return true;
    return id==0 || id%objectFrequency_==objectID%objectFrequency_;
  }
  
  bool Synchronisable::isMyData(unsigned char* mem)
  {
    unsigned int objectID, classID, size;
    size = *(int *)mem;
    mem+=sizeof(size);
    objectID = *(int *)mem;
    mem+=sizeof(objectID);
    classID = *(int *)mem;
    mem+=sizeof(classID);
    
    assert(classID == this->classID);
    return (objectID == this->objectID);
  }
  
  void Synchronisable::setBacksync(bool sync){
    backsync_=sync;
  }

  bool Synchronisable::getBacksync(){
    return backsync_;
  }

}
