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

#include <cstdlib>
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/BaseObject.h"
#include "network/Host.h"

namespace orxonox
{

  std::map<uint32_t, Synchronisable *> Synchronisable::objectMap_;
  std::queue<uint32_t> Synchronisable::deletedObjects_;

  uint8_t Synchronisable::state_=0x1; // detemines wheter we are server (default) or client

  /**
  * Constructor:
  * Initializes all Variables and sets the right objectID_
  */
  Synchronisable::Synchronisable(Context* context)
  {
    RegisterRootObject(Synchronisable);
    static uint32_t idCounter=0;
    objectMode_=0x1; // by default do not send data to server
    if ( GameMode::isMaster()/* || ( Host::running() && Host::isServer() )*/ )
    {
      this->setObjectID( idCounter++ );
    }
    else
    {
      objectID_=OBJECTID_UNKNOWN;
    }
    classID_ = static_cast<uint32_t>(-1);

    // set dataSize to 0
    this->dataSize_ = 0;
    // set standard priority
    this->setPriority( Priority::Normal );

    // get context id
    this->contextID_ = this->findContextID(context);
  }

  /**
   * Destructor:
   * Delete all callback objects and remove objectID_ from the objectMap_
   */
  Synchronisable::~Synchronisable()
  {
    // delete callback function objects
    if(!IdentifierManager::getInstance().isCreatingHierarchy()){
      // remove object from the static objectMap
      if (this->objectMode_ != 0x0 && (Host::running() && Host::isServer()))
        deletedObjects_.push(objectID_);
    }
    // delete all Synchronisable Variables from syncList_ ( which are also in stringList_ )
    for(std::vector<SynchronisableVariableBase*>::iterator it = syncList_.begin(); it!=syncList_.end(); it++)
      delete (*it);
    syncList_.clear();
    stringList_.clear();
    std::map<uint32_t, Synchronisable*>::iterator it2;
    it2 = objectMap_.find(objectID_);
    if (it2 != objectMap_.end())
      objectMap_.erase(it2);

  }

  /**
   * @brief Returns the id of the context.
   * If the context is not Synchronisable, it moves on to its parent, recursively.
   */
  uint32_t Synchronisable::findContextID(Context* context)
  {
      if (context == NULL)
          return OBJECTID_UNKNOWN;

      Synchronisable* synchronisableContext = orxonox_cast<Synchronisable*>(context);
      if (synchronisableContext != NULL)
          return synchronisableContext->getObjectID();
      else
          return this->findContextID(context->getParentContext());
  }

  /**
   * This function sets the internal mode for synchronisation
   * @param b true if this object is located on a client or on a server
   */
  void Synchronisable::setClient(bool b)
  {
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
    if( header.isDiffed() )
    {
      mem += header.getDataSize() + header.getSize();
      return 0;
    }
//     assert( !header.isDiffed() );

    orxout(verbose, context::network) << "fabricating object with id: " << header.getObjectID() << endl;

    Identifier* id = ClassByID(header.getClassID());
    if (!id)
    {
        for(int i = 0; i<160; i++)
            orxout(user_error, context::network) << "classid: " << i << " identifier: " << ClassByID(i) << endl;
        orxout(user_error, context::network) << "Assertion failed: id" << endl;
        orxout(user_error, context::network) << "Possible reason for this error: Client received a synchronizable object whose class has no factory." << endl;
        abort();
    }
    assert(id);
    Context* context = 0;
    if (header.getContextID() != OBJECTID_UNKNOWN)
    {
      Synchronisable* synchronisable_context = Synchronisable::getSynchronisable(header.getContextID());
      if (!synchronisable_context)
      {
        mem += header.getDataSize()+SynchronisableHeader::getSize(); //.TODO: this suckz.... remove size from header
        assert(0); // TODO: uncomment this if we have a clean objecthierarchy (with destruction of children of objects) ^^
        return 0;
      }
      else
        context = orxonox_cast<Context*>(synchronisable_context);
    }
    else
      context = Context::getRootContext();

    assert(getSynchronisable(header.getObjectID())==0);   //make sure no object with this id exists
    BaseObject *bo = orxonox_cast<BaseObject*>(id->fabricate(context));
    assert(bo);
    Synchronisable *no = orxonox_cast<Synchronisable*>(bo);
    assert(no);
    assert( Synchronisable::objectMap_.find(header.getObjectID()) == Synchronisable::objectMap_.end() );
    no->setObjectID(header.getObjectID());
    //no->contextID=header.getContextID(); //TODO: remove this
    no->setClassID(header.getClassID());
    assert(no->contextID_ == header.getContextID());
    if( context )
    {
      BaseObject* boContext = orxonox_cast<BaseObject*>(context);
      if (boContext)
          bo->setLevel(boContext->getLevel()); // Note: this ensures that the level is known on the client for child objects of the scene (and the scene itself)
    }
    //assert(no->classID_ == header.getClassID());
    orxout(verbose, context::network) << "fabricate objectID_: " << no->objectID_ << " classID_: " << no->classID_ << endl;
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
   * Finds and deletes the Synchronisable with the appropriate objectID_
   * @param objectID_ objectID_ of the Synchronisable
   * @return true/false
   */
  bool Synchronisable::deleteObject(uint32_t objectID_)
  {
    if(!getSynchronisable(objectID_))
      return false;
    assert(getSynchronisable(objectID_)->objectID_==objectID_);
    Synchronisable *s = getSynchronisable(objectID_);
    if(s)
      s->destroy(); // or delete?
    else
      return false;
    return true;
  }

  /**
   * This function looks up the objectID_ in the objectMap_ and returns a pointer to the right Synchronisable
   * @param objectID_ objectID_ of the Synchronisable
   * @return pointer to the Synchronisable with the objectID_
   */
  Synchronisable* Synchronisable::getSynchronisable(uint32_t objectID_)
  {
    std::map<uint32_t, Synchronisable*>::iterator it1;
    it1 = objectMap_.find(objectID_);
    if (it1 != objectMap_.end())
      return it1->second;
    // if the objects not in the map it should'nt exist at all anymore
    return NULL;
  }


  /**
   * This function takes all SynchronisableVariables out of the Synchronisable and saves them together with the size, objectID_ and classID_ to the given memory
   * takes a pointer to already allocated memory (must have at least getSize bytes length)
   * structure of the bitstream:
   * |totalsize,objectID_,classID_,var1,var2,string1_length,string1,var3,...|
   * length of varx: size saved int syncvarlist
   * @param mem pointer to allocated memory with enough size
   * @param sizes vector containing sizes of all objects in gamestate (to be appended)
   * @param id gamestateid of the gamestate to be saved (important for priorities)
   * @param mode defines the direction in which the data will be send/received
   *             0x1: server->client
   *             0x2: client->server
   *             0x3: bidirectional
   * @return true: if !doSync or if everything was successfully saved
   */
  uint32_t Synchronisable::getData(uint8_t*& mem, std::vector<uint32_t>& sizes, int32_t id, uint8_t mode)
  {
    unsigned int test = 0;
    if(mode==0x0)
      mode=state_;
    //if this tick is we dont synchronise, then abort now
    if(!doSync(/*id,*/ mode))
      return 0;
    uint32_t tempsize = 0;
#ifndef NDEBUG
    uint8_t* oldmem = mem;
    if (this->classID_==0)
      orxout(internal_info, context::network) << "classid 0 " << this->getIdentifier()->getName() << endl;
#endif

    if (this->classID_ == static_cast<uint32_t>(-1))
        this->classID_ = this->getIdentifier()->getNetworkID();

    assert(ClassByID(this->classID_));
    assert(this->classID_==this->getIdentifier()->getNetworkID());
    assert(this->objectID_!=OBJECTID_UNKNOWN);
    std::vector<SynchronisableVariableBase*>::iterator i;

    // start copy header
    SynchronisableHeader header(mem);
    mem += SynchronisableHeader::getSize();
    // end copy header

    orxout(verbose_more, context::network) << "getting data from objectID_: " << objectID_ << ", classID_: " << classID_ << endl;
//     orxout(verbose, context::network) << "objectid: " << this->objectID_ << ":";
    // copy to location
    for(i=syncList_.begin(); i!=syncList_.end(); ++i)
    {
      uint32_t varsize = (*i)->getData( mem, mode );
//       orxout(verbose, context::network) << " " << varsize;
      tempsize += varsize;
      sizes.push_back(varsize);
      ++test;
      //tempsize += (*i)->getSize( mode );
    }
    assert(tempsize!=0);  // if this happens an empty object (with no variables) would be transmitted
//     orxout(verbose, context::network) << endl;

    header.setObjectID( this->objectID_ );
    header.setContextID( this->contextID_ );
    header.setClassID( this->classID_ );
    header.setDataSize( tempsize );
    assert( tempsize == mem-oldmem-SynchronisableHeader::getSize() );
    assert( test == this->getNrOfVariables() );
    header.setDiffed(false);
    tempsize += SynchronisableHeader::getSize();

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
   * @param forceCallback this makes updateData call each callback
   * @return true/false
   */
  bool Synchronisable::updateData(uint8_t*& mem, uint8_t mode, bool forceCallback)
  {
    if(mode==0x0)
      mode=state_;
    
    if(syncList_.empty())
    {
      orxout(internal_warning, context::network) << "Synchronisable::updateData syncList_ is empty" << endl;
      assert(0);
      return false;
    }

    uint8_t* data=mem;
    // start extract header
    SynchronisableHeaderLight syncHeaderLight(mem);
    assert(syncHeaderLight.getObjectID()==this->getObjectID());
    
    if( !this->doReceive(mode) )
    {
      uint32_t headerSize;
      if( syncHeaderLight.isDiffed() )
        headerSize = SynchronisableHeaderLight::getSize();
      else
        headerSize = SynchronisableHeader::getSize();
      mem += syncHeaderLight.getDataSize() + headerSize;
      return true;
    }

    //orxout(verbose_more, context::network) << "Synchronisable: objectID_ " << syncHeader.getObjectID() << ", classID_ " << syncHeader.getClassID() << " size: " << syncHeader.getDataSize() << " synchronising data" << endl;
    if( !syncHeaderLight.isDiffed() )
    {
      SynchronisableHeader syncHeader2(mem);
      assert( this->getClassID() == syncHeader2.getClassID() );
      assert( this->getContextID() == syncHeader2.getContextID() );
      mem += SynchronisableHeader::getSize();
      std::vector<SynchronisableVariableBase *>::iterator i;
      for(i=syncList_.begin(); i!=syncList_.end(); ++i)
      {
        assert( mem <= data+syncHeader2.getDataSize()+SynchronisableHeader::getSize() ); // always make sure we don't exceed the datasize in our stream
        (*i)->putData( mem, mode, forceCallback );
      }
      assert(mem == data+syncHeaderLight.getDataSize()+SynchronisableHeader::getSize() );
    }
    else
    {
      mem += SynchronisableHeaderLight::getSize();
//       orxout(debug_output, context::network) << "objectID: " << this->objectID_ << endl;
      while( mem < data+syncHeaderLight.getDataSize()+SynchronisableHeaderLight::getSize() )
      {
        VariableID varID = *(VariableID*)mem;
//         orxout(debug_output, context::network) << "varID: " << varID << endl;
        assert( varID < syncList_.size() );
        mem += sizeof(VariableID);
        syncList_[varID]->putData( mem, mode, forceCallback );
      }
      assert(mem == data+syncHeaderLight.getDataSize()+SynchronisableHeaderLight::getSize() );
    }
    return true;
  }

  /**
  * This function returns the total amount of bytes needed by getData to save the whole content of the variables
  * @param id id of the gamestate
  * @param mode same as getData
  * @return amount of bytes
  */
  uint32_t Synchronisable::getSize(int32_t id, uint8_t mode)
  {
    uint32_t tsize=SynchronisableHeader::getSize();
    if (mode==0x0)
      mode=state_;
    if (!doSync(/*id, */mode))
      return 0;
    assert( mode==state_ );
    tsize += this->dataSize_;
    std::vector<SynchronisableVariableBase*>::iterator i;
    for(i=stringList_.begin(); i!=stringList_.end(); ++i)
    {
      tsize += (*i)->getSize( mode );
    }
    return tsize;
  }

  /**
   * This function determines, wheter the object should be saved to the bytestream (according to its syncmode/direction)
   * @param mode Synchronisation mode (toclient, toserver or bidirectional)
   * @return true/false
   */
  bool Synchronisable::doSync(/*int32_t id,*/ uint8_t mode)
  {
//     if(mode==0x0)
//       mode=state_;
    assert(mode!=0x0);
    return ( (this->objectMode_ & mode)!=0 && (!syncList_.empty() ) );
  }
  
  /**
   * This function determines, wheter the object should accept data from the bytestream (according to its syncmode/direction)
   * @param mode Synchronisation mode (toclient, toserver or bidirectional)
   * @return true/false
   */
  bool Synchronisable::doReceive( uint8_t mode)
  {
    //return mode != this->objectMode_ || this->objectMode_ == ObjectDirection::Bidirectional;
    return true;
  }

  /**
   * This function sets the synchronisation mode of the object
   * If set to 0x0 variables will not be synchronised at all
   * If set to 0x1 variables will only be synchronised to the client
   * If set to 0x2 variables will only be synchronised to the server
   * If set to 0x3 variables will be synchronised bidirectionally (only if set so in registerVar)
   * @param mode same as in registerVar
   */
  void Synchronisable::setSyncMode(uint8_t mode)
  {
    assert(mode==0x0 || mode==0x1 || mode==0x2 || mode==0x3);
    this->objectMode_=mode;
  }

  template <> void Synchronisable::registerVariable( std::string& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional)
  {
    SynchronisableVariableBase* sv;
    if (bidirectional)
      sv = new SynchronisableVariableBidirectional<std::string>(variable, mode, cb);
    else
      sv = new SynchronisableVariable<std::string>(variable, mode, cb);
    syncList_.push_back(sv);
    stringList_.push_back(sv);
  }

template <> void Synchronisable::unregisterVariable( std::string& variable )
  {
    bool unregistered_nonexistent_variable = true;
    std::vector<SynchronisableVariableBase*>::iterator it = syncList_.begin();
    while(it!=syncList_.end())
    {
      if( ((*it)->getReference()) == &variable )
      {
        delete (*it);
        syncList_.erase(it);
        unregistered_nonexistent_variable = false;
        break;
      }
      else
        ++it;
    }
    assert(unregistered_nonexistent_variable == false);
    
    it = stringList_.begin();
    while(it!=stringList_.end())
    {
      if( ((*it)->getReference()) == &variable )
      {
        delete (*it);
        stringList_.erase(it);
        return;
      }
      else
        ++it;
    }
    unregistered_nonexistent_variable = true;
    assert(unregistered_nonexistent_variable == false); //if we reach this point something went wrong:
    // the variable has not been registered before
  }


}
