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

#include <cstring>
#include <string>
#include <iostream>
#include <assert.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
// #include "core/Identifier.h"

namespace network
{


  std::map<unsigned int, Synchronisable *> Synchronisable::objectMap_;
  std::queue<unsigned int> Synchronisable::deletedObjects_;

  int Synchronisable::state_=0x1; // detemines wheter we are server (default) or client

  /**
  * Constructor:
  * Initializes all Variables and sets the right objectID
  */
  Synchronisable::Synchronisable(){
    RegisterRootObject(Synchronisable);
    static uint32_t idCounter=0;
    objectFrequency_=1;
    objectMode_=0x1; // by default do not send data to server
    objectID=idCounter++;
    classID = (unsigned int)-1;
    syncList = new std::list<synchronisableVariable *>;
  }

  /**
   * Destructor:
   * Delete all callback objects and remove objectID from the objectMap_
   */
  Synchronisable::~Synchronisable(){
    // delete callback function objects
    if(!orxonox::Identifier::isCreatingHierarchy()){
      for(std::list<synchronisableVariable *>::iterator it = syncList->begin(); it!=syncList->end(); it++)
        delete (*it)->callback;
      if (this->objectMode_ != 0x0)
        deletedObjects_.push(objectID);
//       COUT(3) << "destruct synchronisable +++" << objectID << " | " << classID << std::endl;
//       COUT(3) << " bump ---" << objectID << " | " << &objectMap_ << std::endl;
//       assert(objectMap_[objectID]->objectID==objectID);
//       objectMap_.erase(objectID);
    }
  }

  /**
   * This function gets called after all neccessary data has been passed to the object
   * Overload this function and recall the create function of the parent class
   * @return true/false
   */
  bool Synchronisable::create(){
    this->classID = this->getIdentifier()->getNetworkID();
//     COUT(4) << "creating synchronisable: setting classid from " << this->getIdentifier()->getName() << " to: " << classID << std::endl;

//     COUT(3) << "construct synchronisable +++" << objectID << " | " << classID << std::endl;
//     objectMap_[objectID]=this;
//     assert(objectMap_[objectID]==this);
//     assert(objectMap_[objectID]->objectID==objectID);
    return true;
  }


  /**
   * This function sets the internal mode for synchronisation
   * @param b true if this object is located on a client or on a server
   */
  void Synchronisable::setClient(bool b){
    if(b) // client
      state_=0x2;
    else  // server
      state_=0x1;
  }

  /**
   * This function fabricated a new synchrnisable (and children of it), sets calls updateData and create
   * After calling this function the mem pointer will be increased by the size of the needed data
   * @param mem pointer to where the appropriate data is located
   * @param mode defines the mode, how the data should be loaded
   * @return pointer to the newly created synchronisable
   */
  Synchronisable *Synchronisable::fabricate(uint8_t*& mem, int mode)
  {
    synchronisableHeader *header = (synchronisableHeader *)mem;

    COUT(3) << "fabricating object with id: " << header->objectID << std::endl;

    orxonox::Identifier* id = ClassByID(header->classID);
    assert(id);
    orxonox::BaseObject *bo = id->fabricate(0); //TODO: get BaseObject* from header->creatorID here
    Synchronisable *no = dynamic_cast<Synchronisable *>(bo);
    assert(no);
    no->objectID=header->objectID;
    no->classID=header->classID;
    COUT(3) << "fabricate objectID: " << no->objectID << " classID: " << no->classID << std::endl;
          // update data and create object/entity...
    bool b = no->updateData(mem, mode, true);
    assert(b);
    b = no->create();
    assert(b);
    return no;
  }


  /**
   * Finds and deletes the Synchronisable with the appropriate objectID
   * @param objectID objectID of the Synchronisable
   * @return true/false
   */
  bool Synchronisable::deleteObject(unsigned int objectID){
//     assert(getSynchronisable(objectID));
    if(!getSynchronisable(objectID))
      return false;
    assert(getSynchronisable(objectID)->objectID==objectID);
//     delete objectMap_[objectID];
    Synchronisable *s = getSynchronisable(objectID);
    if(s)
      delete s;
    else
      return false;
    return true;
  }

  /**
   * This function looks up the objectID in the objectMap_ and returns a pointer to the right Synchronisable
   * @param objectID objectID of the Synchronisable
   * @return pointer to the Synchronisable with the objectID
   */
  Synchronisable* Synchronisable::getSynchronisable(unsigned int objectID){
    orxonox::ObjectList<Synchronisable>::iterator it;
    for(it = orxonox::ObjectList<Synchronisable>::begin(); it; ++it){
      if( it->getObjectID()==objectID )
           return *it;
    }
    return NULL;

//     std::map<unsigned int, Synchronisable *>::iterator i = objectMap_.find(objectID);
//     if(i==objectMap_.end())
//       return NULL;
//     assert(i->second->objectID==objectID);
//     return (*i).second;
  }


  /**
  * This function is used to register a variable to be synchronized
  * also counts the total datasize needed to save the variables
  * @param var pointer to the variable
  * @param size size of the datatype the variable consists of
  * @param t the type of the variable (network::DATA or network::STRING
  * @param mode same as in getData
  * @param cb callback object that should get called, if the value of the variable changes
  */
  void Synchronisable::registerVar(void *var, int size, variableType t, int mode, NetworkCallbackBase *cb){
    assert( mode==direction::toclient || mode==direction::toserver || mode==direction::serverMaster || mode==direction::clientMaster);
    // create temporary synch.Var struct
    synchronisableVariable *temp = new synchronisableVariable;
    temp->size = size;
    temp->var = var;
    temp->mode = mode;
    temp->type = t;
    temp->callback = cb;
    if( ( mode & direction::bidirectional ) )
    {
      temp->varBuffer = new uint8_t[size];
      memcpy(temp->varBuffer, temp->var, size); //now fill the buffer for the first time
      temp->varReference = 0;
    }
    COUT(5) << "Syncronisable::registering var with size: " << temp->size << " and type: " << temp->type << std::endl;
    //std::cout << "push temp to syncList (at the bottom) " << datasize << std::endl;
    COUT(5) << "Syncronisable::objectID: " << objectID << " this: " << this << " name: " << this->getIdentifier()->getName() << " networkID: " << this->getIdentifier()->getNetworkID() << std::endl;
    syncList->push_back(temp);
#ifndef NDEBUG
    std::list<synchronisableVariable *>::iterator it = syncList->begin();
    while(it!=syncList->end()){
      assert(*it!=var);
      it++;
    }
#endif
  }

  /**
   * This function takes all SynchronisableVariables out of the Synchronisable and saves them together with the size, objectID and classID to the given memory
   * takes a pointer to already allocated memory (must have at least getSize bytes length)
   * structure of the bitstream:
   * |totalsize,objectID,classID,var1,var2,string1_length,string1,var3,...|
   * length of varx: size saved int syncvarlist
   * @param mem pointer to allocated memory with enough size
   * @param id gamestateid of the gamestate to be saved (important for priorities)
   * @param mode defines the direction in which the data will be send/received
   *             0x1: server->client
   *             0x2: client->server (not recommended)
   *             0x3: bidirectional
   * @return true: if !doSync or if everything was successfully saved
   */
  bool Synchronisable::getData(uint8_t*& mem, unsigned int id, int mode){
    //if this tick is we dont synchronise, then abort now
    if(!doSync(id))
      return true;
    //std::cout << "inside getData" << std::endl;
    unsigned int tempsize = 0;
    if(mode==0x0)
      mode=state_;
    if(classID==0)
      COUT(3) << "classid 0 " << this->getIdentifier()->getName() << std::endl;

    if (this->classID == (unsigned int)-1)
        this->classID = this->getIdentifier()->getNetworkID();

    assert(this->classID==this->getIdentifier()->getNetworkID());
//     this->classID=this->getIdentifier()->getNetworkID(); // TODO: correct this
    std::list<synchronisableVariable *>::iterator i;
    unsigned int size;
    size=getSize(id, mode);

    // start copy header
    synchronisableHeader *header = (synchronisableHeader *)mem;
    header->size = size;
    header->objectID = this->objectID;
    header->creatorID = this->creatorID;
    header->classID = this->classID;
    header->dataAvailable = true;
    tempsize+=sizeof(synchronisableHeader);
    mem+=sizeof(synchronisableHeader);
    // end copy header


    COUT(5) << "Synchronisable getting data from objectID: " << objectID << " classID: " << classID << " length: " << size << std::endl;
    // copy to location
    for(i=syncList->begin(); i!=syncList->end(); ++i){
      if( ((*i)->mode & mode) == 0 ){
        COUT(5) << "not getting data: " << std::endl;
        continue;  // this variable should only be received
      }
      // if the variable gets synchronised bidirectional, then add the reference to the bytestream
      if( ( (*i)->mode & direction::bidirectional ) )
      {
        *(uint8_t*)mem = (*i)->varReference;
        mem += sizeof( (*i)->varReference );
      }
      switch((*i)->type){
        case DATA:
          memcpy( (void *)(mem), (void*)((*i)->var), (*i)->size);
          mem+=(*i)->size;
          tempsize+=(*i)->size + sizeof( (*i)->varReference );
          break;
        case STRING:
          memcpy( (void *)(mem), (void *)&((*i)->size), sizeof(size_t) );
          mem+=sizeof(size_t);
          const char *data = ( ( *(std::string *) (*i)->var).c_str());
          memcpy( mem, (void*)data, (*i)->size);
          COUT(5) << "synchronisable: char: " << (const char *)(mem) << " data: " << data << " string: " << *(std::string *)((*i)->var) << std::endl;
          mem+=(*i)->size;
          tempsize+=(*i)->size + sizeof( (*i)->varReference ) + sizeof(size_t);
          break;
      }
    }
    assert(tempsize==size);
    return true;
  }


  /**
   * This function takes a bytestream and loads the data into the registered variables
   * @param mem pointer to the bytestream
   * @param mode same as in getData
   * @return true/false
   */
  bool Synchronisable::updateData(uint8_t*& mem, int mode, bool forceCallback){
    if(mode==0x0)
      mode=state_;
    std::list<synchronisableVariable *>::iterator i;
    if(syncList->empty()){
      COUT(4) << "Synchronisable::updateData syncList is empty" << std::endl;
      return false;
    }

    uint8_t *data=mem;
    // start extract header
    synchronisableHeader *syncHeader = (synchronisableHeader *)mem;
    assert(syncHeader->objectID==this->objectID);
    assert(syncHeader->creatorID==this->creatorID);
    if(syncHeader->dataAvailable==false){
      mem+=syncHeader->size;
      return true;
    }

    mem+=sizeof(synchronisableHeader);
    // stop extract header
    assert(this->objectID==syncHeader->objectID);
//    assert(this->classID==syncHeader->classID); //TODO: fix this!!! maybe a problem with the identifier ?

    COUT(5) << "Synchronisable: objectID " << syncHeader->objectID << ", classID " << syncHeader->classID << " size: " << syncHeader->size << " synchronising data" << std::endl;
    for(i=syncList->begin(); i!=syncList->end() && mem <= data+syncHeader->size; i++){
      if( ((*i)->mode ^ mode) == 0 ){
        COUT(5) << "synchronisable: not updating variable " << std::endl;
        continue;  // this variable should only be set
      }
      COUT(5) << "Synchronisable: element size: " << (*i)->size << " type: " << (*i)->type << std::endl;
      bool callback=false;
      switch((*i)->type){
        case DATA:
          if( ( (*i)->mode & direction::bidirectional ) )
          {
            if( ( mode == 0x1 && (*i)->mode == direction::serverMaster ) || \
                  ( mode == 0x2 && (*i)->mode == direction::clientMaster ) )    // if true we are master on this variable
            {
              uint8_t refNr = *(uint8_t *)mem;
              if( refNr != (*i)->varReference )
              {
                mem += sizeof((*i)->varReference) + (*i)->size; // the reference for this variable is not recent, discard data
                break;
              }
            }
            else //we are slave for this variable
            {
              (*i)->varReference = *(uint8_t *)mem; //copy the reference value for this variable
            }
            mem += sizeof((*i)->varReference);
          }
          if((*i)->callback) // check whether this variable changed (but only if callback was set)
            if(strncmp((char *)(*i)->var, (char *)mem, (*i)->size)!=0)
              callback=true;
          memcpy((void*)(*i)->var, mem, (*i)->size);
          mem+=(*i)->size;
          break;
        case STRING:
          if( ( (*i)->mode & direction::bidirectional ) )
          {
            if( ( mode == 0x1 && (*i)->mode == direction::serverMaster ) || \
                  ( mode == 0x2 && (*i)->mode == direction::clientMaster ) )    // if true we are master for this variable
            {
              uint8_t refNr = *(uint8_t *)mem;
              mem += sizeof( (*i)->varReference );
              if( refNr != (*i)->varReference ){
                mem += sizeof(size_t) + *(size_t *)mem; // the reference for this variable is not recent, discard data
                break;
              }
            }
            else //we are slave for this variable
            {
              (*i)->varReference = *(uint8_t *)mem; //copy the reference value for this variable
            }
            mem += sizeof( (*i)->varReference );
          }
          (*i)->size = *(size_t *)mem;
          COUT(5) << "string size: " << (*i)->size << std::endl;
          mem += sizeof(size_t);
          if((*i)->callback) // check whether this string changed
            if( *(std::string *)((*i)->var) != std::string((char *)mem) )
              callback=true;
          *((std::string *)((*i)->var)) = std::string((const char*)mem);
          COUT(5) << "synchronisable: char: " << (const char*)mem << " string: " << std::string((const char*)mem) << std::endl;
          mem += (*i)->size;
          break;
      }
      // call the callback function, if defined
      if((callback || forceCallback) && (*i)->callback)
        (*i)->callback->call();
    }
    return true;
  }

  /**
  * This function returns the total amount of bytes needed by getData to save the whole content of the variables
  * @param id id of the gamestate
  * @param mode same as getData
  * @return amount of bytes
  */
  uint32_t Synchronisable::getSize(unsigned int id, int mode){
    if(!doSync(id))
      return 0;
    int tsize=sizeof(synchronisableHeader);
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
      if( ( (*i)->mode & direction::bidirectional ) != 0)
      {
        tsize+=sizeof( (*i)->varReference );
      }
    }
    return tsize;
  }

  /**
   * This function determines, wheter the object should be saved to the bytestream (according to its syncmode/direction)
   * @param id gamestate id
   * @return true/false
   */
  bool Synchronisable::doSync(unsigned int id){
    return ( (objectMode_&state_)!=0 );
  }

  bool Synchronisable::doSelection(unsigned int id){
    return ( id==0 || id%objectFrequency_==objectID%objectFrequency_ ) && ((objectMode_&state_)!=0);
  }

  /**
   * This function looks at the header located in the bytestream and checks wheter objectID and classID match with the Synchronisables ones
   * @param mem pointer to the bytestream
   */
  bool Synchronisable::isMyData(uint8_t* mem)
  {
    synchronisableHeader *header = (synchronisableHeader *)mem;
    assert(header->objectID==this->objectID);
    return header->dataAvailable;
  }

  /**
   * This function sets the synchronisation mode of the object
   * If set to 0x1 variables will only be synchronised to the client
   * If set to 0x2 variables will only be synchronised to the server
   * If set to 0x3 variables will be synchronised bidirectionally (only if set so in registerVar)
   * @param mode same as in registerVar
   */
  void Synchronisable::setObjectMode(int mode){
    assert(mode==0x0 || mode==0x1 || mode==0x2 || mode==0x3);
    objectMode_=mode;
  }


}
