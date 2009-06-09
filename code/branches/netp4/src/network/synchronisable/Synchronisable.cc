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


#include "Synchronisable.h"

#include <cstring>
#include <string>
#include <iostream>
#include <cassert>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
// #include "core/Identifier.h"

#include "network/Host.h"
namespace orxonox
{

  std::map<uint32_t, Synchronisable *> Synchronisable::objectMap_;
  std::queue<uint32_t> Synchronisable::deletedObjects_;

  uint8_t Synchronisable::state_=0x1; // detemines wheter we are server (default) or client

  /**
  * Constructor:
  * Initializes all Variables and sets the right objectID
  */
  Synchronisable::Synchronisable(BaseObject* creator){
    RegisterRootObject(Synchronisable);
    static uint32_t idCounter=0;
    objectMode_=0x1; // by default do not send data to server
    if ( !Host::running() || ( Host::running() && Host::isServer() ) )
    {
      this->objectID = idCounter++; //this is only needed when running a server
    //add synchronisable to the objectMap
      objectMap_[this->objectID] = this;
    }
    else
      objectID=OBJECTID_UNKNOWN;
    classID = static_cast<uint32_t>(-1);
    
    // set dataSize to 0
    this->dataSize_ = 0;
    // set standard priority
    this->setPriority( priority::normal );

    // get creator id
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
      for(std::vector<SynchronisableVariableBase*>::iterator it = syncList.begin(); it!=syncList.end(); it++)
        delete (*it);
      if (this->objectMode_ != 0x0 && (Host::running() && Host::isServer()))
        deletedObjects_.push(objectID);
    }
    std::map<uint32_t, Synchronisable*>::iterator it;
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
    SynchronisableHeader header(mem);

    if(!header.isDataAvailable())
    {
      mem += header.getDataSize();
      return 0;
    }

    COUT(4) << "fabricating object with id: " << header.getObjectID() << std::endl;

    Identifier* id = ClassByID(header.getClassID());
    if (!id)
    {
        for(int i = 0; i<160; i++)
            COUT(0) << "classid: " << i << " identifier: " << ClassByID(i) << endl;
        COUT(0) << "Assertion failed: id" << std::endl;
        COUT(0) << "Possible reason for this error: Client received a synchronizable object whose class has no factory." << std::endl;
        abort();
    }
    assert(id);
    BaseObject* creator = 0;
    if (header.getCreatorID() != OBJECTID_UNKNOWN)
    {
      Synchronisable* synchronisable_creator = Synchronisable::getSynchronisable(header.getCreatorID());
      if (!synchronisable_creator)
      {
        mem += header.getDataSize(); //.TODO: this suckz.... remove size from header
        assert(0); // TODO: uncomment this if we have a clean objecthierarchy (with destruction of children of objects) ^^
        return 0;
      }
      else
        creator = dynamic_cast<BaseObject*>(synchronisable_creator);
    }
    assert(getSynchronisable(header.getObjectID())==0);   //make sure no object with this id exists
    BaseObject *bo = id->fabricate(creator);
    assert(bo);
    Synchronisable *no = dynamic_cast<Synchronisable *>(bo);
    assert(no);
    no->objectID=header.getObjectID();
    no->creatorID=header.getCreatorID(); //TODO: remove this
    no->classID=header.getClassID();
    COUT(4) << "fabricate objectID: " << no->objectID << " classID: " << no->classID << std::endl;
          // update data and create object/entity...
    assert( Synchronisable::objectMap_.find(header.getObjectID()) == Synchronisable::objectMap_.end() );
    Synchronisable::objectMap_[header.getObjectID()] = no;
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
  bool Synchronisable::deleteObject(uint32_t objectID){
    if(!getSynchronisable(objectID))
      return false;
    assert(getSynchronisable(objectID)->objectID==objectID);
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
  Synchronisable* Synchronisable::getSynchronisable(uint32_t objectID){
    std::map<uint32_t, Synchronisable*>::iterator it1;
    it1 = objectMap_.find(objectID);
    if (it1 != objectMap_.end())
      return it1->second;

//     ObjectList<Synchronisable>::iterator it;
//     for(it = ObjectList<Synchronisable>::begin(); it; ++it){
//       if( it->getObjectID()==objectID ){
//         objectMap_[objectID] = *it;
//         return *it;
//       }
//     }
    // if the objects not in the map it should'nt exist at all anymore
    return NULL;
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
  uint32_t Synchronisable::getData(uint8_t*& mem, int32_t id, uint8_t mode){
    if(mode==0x0)
      mode=state_;
    //if this tick is we dont synchronise, then abort now
    if(!doSync(id, mode))
      return 0;
    uint32_t tempsize = 0;
    if (this->classID==0)
      COUT(3) << "classid 0 " << this->getIdentifier()->getName() << std::endl;

    if (this->classID == static_cast<uint32_t>(-1))
        this->classID = this->getIdentifier()->getNetworkID();

    assert(ClassByID(this->classID));
    assert(this->classID==this->getIdentifier()->getNetworkID());
    std::vector<SynchronisableVariableBase*>::iterator i;

    // start copy header
    SynchronisableHeader header(mem);
    mem += SynchronisableHeader::getSize();
    // end copy header


    COUT(5) << "Synchronisable getting data from objectID: " << objectID << " classID: " << classID << std::endl;
    // copy to location
    for(i=syncList.begin(); i!=syncList.end(); ++i){
      tempsize += (*i)->getData( mem, mode );
      //tempsize += (*i)->getSize( mode );
    }
    
    tempsize += SynchronisableHeader::getSize();
    header.setObjectID( this->objectID );
    header.setCreatorID( this->creatorID );
    header.setClassID( this->classID );
    header.setDataAvailable( true );
    header.setDataSize( tempsize );
    
#ifndef NDEBUG
    uint32_t size;
    size=getSize(id, mode);
    assert(tempsize==size);
#endif
    return tempsize;
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
    std::vector<SynchronisableVariableBase *>::iterator i;
    if(syncList.empty()){
      assert(0);
      COUT(4) << "Synchronisable::updateData syncList is empty" << std::endl;
      return false;
    }

    uint8_t* data=mem;
    // start extract header
    SynchronisableHeader syncHeader(mem);
    assert(syncHeader.getObjectID()==this->objectID);
    assert(syncHeader.getCreatorID()==this->creatorID);
    assert(syncHeader.getClassID()==this->classID);
    if(syncHeader.isDataAvailable()==false){
      mem += syncHeader.getDataSize();
      return true;
    }

    mem += SynchronisableHeader::getSize();
    // stop extract header

    //COUT(5) << "Synchronisable: objectID " << syncHeader.getObjectID() << ", classID " << syncHeader.getClassID() << " size: " << syncHeader.getDataSize() << " synchronising data" << std::endl;
    for(i=syncList.begin(); i!=syncList.end(); i++)
    {
      assert( mem <= data+syncHeader.getDataSize() ); // always make sure we don't exceed the datasize in our stream
      (*i)->putData( mem, mode, forceCallback );
    }
    assert(mem == data+syncHeader.getDataSize());
    return true;
  }

  /**
  * This function returns the total amount of bytes needed by getData to save the whole content of the variables
  * @param id id of the gamestate
  * @param mode same as getData
  * @return amount of bytes
  */
  uint32_t Synchronisable::getSize(int32_t id, uint8_t mode){
    int tsize=SynchronisableHeader::getSize();
    if (mode==0x0)
      mode=state_;
    if (!doSync(id, mode))
      return 0;
    assert( mode==state_ );
    tsize += this->dataSize_;
    std::vector<SynchronisableVariableBase*>::iterator i;
    for(i=stringList.begin(); i!=stringList.end(); ++i){
      tsize += (*i)->getSize( mode );
    }
    return tsize;
  }

  /**
   * This function determines, wheter the object should be saved to the bytestream (according to its syncmode/direction)
   * @param id gamestate id
   * @return true/false
   */
  bool Synchronisable::doSync(int32_t id, uint8_t mode){
    if(mode==0x0)
      mode=state_;
    return ( (objectMode_&mode)!=0 && (!syncList.empty() ) );
  }

  /**
   * This function looks at the header located in the bytestream and checks wheter objectID and classID match with the Synchronisables ones
   * @param mem pointer to the bytestream
   */
  bool Synchronisable::isMyData(uint8_t* mem)
  {
    SynchronisableHeader header(mem);
    assert(header.getObjectID()==this->objectID);
    return header.isDataAvailable();
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
