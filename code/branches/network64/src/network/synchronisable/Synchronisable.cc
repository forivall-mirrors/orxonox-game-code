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

#include "network/Host.h"
namespace orxonox
{


  std::map<unsigned int, Synchronisable *> Synchronisable::objectMap_;
  std::queue<unsigned int> Synchronisable::deletedObjects_;

  uint8_t Synchronisable::state_=0x1; // detemines wheter we are server (default) or client

  /**
  * Constructor:
  * Initializes all Variables and sets the right objectID
  */
  Synchronisable::Synchronisable(BaseObject* creator){
    RegisterRootObject(Synchronisable);
    static uint32_t idCounter=0;
    objectFrequency_=1;
    objectMode_=0x1; // by default do not send data to server
    if ( !Host::running() || ( Host::running() && Host::isServer() ) )
    {
      this->objectID = idCounter++; //this is only needed when running a server
    //add synchronisable to the objectMap
      objectMap_[this->objectID] = this;
    }
    else
      objectID=OBJECTID_UNKNOWN;
    classID = (unsigned int)-1;


#ifndef NDEBUG
    ObjectList<Synchronisable>::iterator it;
    for(it = ObjectList<Synchronisable>::begin(); it!=ObjectList<Synchronisable>::end(); ++it){
      if( it->getObjectID()==this->objectID )
        assert(*it==this || (it->objectID==OBJECTID_UNKNOWN && it->objectMode_==0x0));
    }
#endif

    this->creatorID = OBJECTID_UNKNOWN;

    searchcreatorID:
    if (creator)
    {
        Synchronisable* synchronisable_creator = dynamic_cast<Synchronisable*>(creator);
        if (synchronisable_creator && synchronisable_creator->objectMode_)
        {
            this->creatorID = synchronisable_creator->getObjectID();
        }
        else if (creator != creator->getCreator())
        {
            creator = creator->getCreator();
            goto searchcreatorID;
        }
    }
  }

  /**
   * Destructor:
   * Delete all callback objects and remove objectID from the objectMap_
   */
  Synchronisable::~Synchronisable(){
    // delete callback function objects
    if(!Identifier::isCreatingHierarchy()){
      for(std::list<SynchronisableVariableBase*>::iterator it = syncList.begin(); it!=syncList.end(); it++)
        delete (*it);
      if (this->objectMode_ != 0x0 && (Host::running() && Host::isServer()))
        deletedObjects_.push(objectID);
//       COUT(3) << "destruct synchronisable +++" << objectID << " | " << classID << std::endl;
//       COUT(3) << " bump ---" << objectID << " | " << &objectMap_ << std::endl;
//       assert(objectMap_[objectID]->objectID==objectID);
//       objectMap_.erase(objectID);
    }
    std::map<unsigned int, Synchronisable*>::iterator it;
    it = objectMap_.find(objectID);
    if (it != objectMap_.end())
      objectMap_.erase(it);
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
  Synchronisable *Synchronisable::fabricate(uint8_t*& mem, uint8_t mode)
  {
    synchronisableHeader *header = (synchronisableHeader *)mem;

    if(!header->dataAvailable)
    {
      mem += header->size;
      return 0;
    }

    COUT(4) << "fabricating object with id: " << header->objectID << std::endl;

    Identifier* id = ClassByID(header->classID);
    assert(id);
    BaseObject* creator = 0;
    if (header->creatorID != OBJECTID_UNKNOWN)
    {
      Synchronisable* synchronisable_creator = Synchronisable::getSynchronisable(header->creatorID);
      if (!synchronisable_creator)
      {
        mem += header->size; //.TODO: this suckz.... remove size from header
        return 0;
      }
      else
        creator = dynamic_cast<BaseObject*>(synchronisable_creator);
    }
    assert(getSynchronisable(header->objectID)==0);   //make sure no object with this id exists
    BaseObject *bo = id->fabricate(creator);
    assert(bo);
    Synchronisable *no = dynamic_cast<Synchronisable *>(bo);
    assert(no);
    no->objectID=header->objectID;
    no->creatorID=header->creatorID; //TODO: remove this
    no->classID=header->classID;
    COUT(4) << "fabricate objectID: " << no->objectID << " classID: " << no->classID << std::endl;
          // update data and create object/entity...
    bool b = no->updateData(mem, mode, true);
    assert(b);
    if (b)
    {
//        b = no->create();
        assert(b);
    }
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
    std::map<unsigned int, Synchronisable*>::iterator it1;
    it1 = objectMap_.find(objectID);
    if (it1 != objectMap_.end())
      return it1->second;

    ObjectList<Synchronisable>::iterator it;
    for(it = ObjectList<Synchronisable>::begin(); it; ++it){
      if( it->getObjectID()==objectID ){
        objectMap_[objectID] = *it;
        return *it;
      }
    }
    return NULL;
  }


  /**
  * This function is used to register a variable to be synchronized
  * also counts the total datasize needed to save the variables
  * @param var pointer to the variable
  * @param size size of the datatype the variable consists of
  * @param t the type of the variable (DATA or STRING
  * @param mode same as in getData
  * @param cb callback object that should get called, if the value of the variable changes
  */

/*  void Synchronisable::registerVariable(void *var, int size, variableType t, uint8_t mode, NetworkCallbackBase *cb){
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
      if(t!=STRING)
      {
        temp->varBuffer = new uint8_t[size];
        memcpy(temp->varBuffer, temp->var, size); //now fill the buffer for the first time
      }
      else
      {
        temp->varBuffer=new std::string( *static_cast<std::string*>(var) );
      }
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
  }*/
  

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
  bool Synchronisable::getData(uint8_t*& mem, unsigned int id, uint8_t mode){
    if(mode==0x0)
      mode=state_;
    //if this tick is we dont synchronise, then abort now
    if(!doSync(id, mode))
      return true;
    //std::cout << "inside getData" << std::endl;
    unsigned int tempsize = 0;
    if(classID==0)
      COUT(3) << "classid 0 " << this->getIdentifier()->getName() << std::endl;

    if (this->classID == (unsigned int)-1)
        this->classID = this->getIdentifier()->getNetworkID();

    assert(this->classID==this->getIdentifier()->getNetworkID());
//     this->classID=this->getIdentifier()->getNetworkID(); // TODO: correct this
    std::list<SynchronisableVariableBase*>::iterator i;
    unsigned int size;
    size=getSize(id, mode);

    // start copy header
    synchronisableHeader *header = (synchronisableHeader *)mem;
    header->size = size;
    header->objectID = this->objectID;
    header->creatorID = this->creatorID;
    header->classID = this->classID;
    header->dataAvailable = true;
    tempsize += sizeof(synchronisableHeader);
    mem += sizeof(synchronisableHeader);
    // end copy header


    COUT(5) << "Synchronisable getting data from objectID: " << objectID << " classID: " << classID << " length: " << size << std::endl;
    // copy to location
    for(i=syncList.begin(); i!=syncList.end(); ++i){
      (*i)->getData( mem, mode );
      tempsize += (*i)->getSize( mode );
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
  bool Synchronisable::updateData(uint8_t*& mem, uint8_t mode, bool forceCallback){
    if(mode==0x0)
      mode=state_;
    std::list<SynchronisableVariableBase *>::iterator i;
    //assert(objectMode_!=0x0);
    //assert( (mode ^ objectMode_) != 0);
    if(syncList.empty()){
      COUT(4) << "Synchronisable::updateData syncList is empty" << std::endl;
      return false;
    }

    uint8_t* data=mem;
    // start extract header
    synchronisableHeader *syncHeader = (synchronisableHeader *)mem;
    assert(syncHeader->objectID==this->objectID);
    assert(syncHeader->creatorID==this->creatorID);
    assert(this->classID==syncHeader->classID); //TODO: fix this!!! maybe a problem with the identifier ?
    if(syncHeader->dataAvailable==false){
      mem += syncHeader->size;
      return true;
    }

    mem += sizeof(synchronisableHeader);
    // stop extract header

    COUT(5) << "Synchronisable: objectID " << syncHeader->objectID << ", classID " << syncHeader->classID << " size: " << syncHeader->size << " synchronising data" << std::endl;
    for(i=syncList.begin(); i!=syncList.end() && mem <= data+syncHeader->size; i++)
    {
      (*i)->putData( mem, mode, forceCallback );
    }
    assert(mem == data+syncHeader->size);
    return true;
  }

  /**
  * This function returns the total amount of bytes needed by getData to save the whole content of the variables
  * @param id id of the gamestate
  * @param mode same as getData
  * @return amount of bytes
  */
  uint32_t Synchronisable::getSize(unsigned int id, uint8_t mode){
    int tsize=sizeof(synchronisableHeader);
    if(mode==0x0)
      mode=state_;
    if(!doSync(id, mode))
      return 0;
    std::list<SynchronisableVariableBase*>::iterator i;
    for(i=syncList.begin(); i!=syncList.end(); i++){
      tsize += (*i)->getSize( mode );
    }
    return tsize;
  }

  /**
   * This function determines, wheter the object should be saved to the bytestream (according to its syncmode/direction)
   * @param id gamestate id
   * @return true/false
   */
  bool Synchronisable::doSync(unsigned int id, uint8_t mode){
    if(mode==0x0)
      mode=state_;
    return ( (objectMode_&mode)!=0 && (!syncList.empty() ) );
  }

  bool Synchronisable::doSelection(unsigned int id){
    return true; //TODO: change this
    //return ( id==0 || id%objectFrequency_==objectID%objectFrequency_ ) && ((objectMode_&state_)!=0);
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
   * If set to 0x0 variables will not be synchronised at all
   * If set to 0x1 variables will only be synchronised to the client
   * If set to 0x2 variables will only be synchronised to the server
   * If set to 0x3 variables will be synchronised bidirectionally (only if set so in registerVar)
   * @param mode same as in registerVar
   */
  void Synchronisable::setObjectMode(uint8_t mode){
    assert(mode==0x0 || mode==0x1 || mode==0x2 || mode==0x3);
    objectMode_=mode;
  }
  

}
