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
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Synchronisable_H__
#define _Synchronisable_H__

#include "network/NetworkPrereqs.h"

#include <cassert>
#include <cstring>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include "util/mbool.h"
#include "core/OrxonoxClass.h"
#include "SynchronisableVariable.h"
#include "NetworkCallback.h"


namespace orxonox
{

  namespace ObjectDirection{
    enum Value{
      ToClient=0x1,
      ToServer=0x2,
      Bidirectional=0x3
    };
  }

  namespace Priority{
    enum Value{
      VeryHigh    = -100,
      High        = -15,
      Normal      = 0,
      Low         = 15,
      VeryLow     = 100
    };
  }

    /**
   * @brief: stores information about a Synchronisable (light version)
   *
   * This class stores the information about a Synchronisable (objectID_, dataSize)
   * in an emulated bitset.
   * Bit 1 to 31 store the size of the Data the synchronisable consumes in the stream
   * Bit 32 is a bool and defines whether the variables are stored in diff mode
   * Byte 5 to 8: objectID_
   */
  class _NetworkExport SynchronisableHeaderLight
  {
    protected:
      uint8_t* data_;
    public:
      SynchronisableHeaderLight(uint8_t* data)
        { data_ = data; }
      inline static uint32_t getSize()
        { return 6; }
      inline uint16_t getDataSize() const
        { return (*(uint16_t*)data_) & 0x7FFF; } //only use the first 31 bits
      inline void setDataSize(uint16_t size)
        { *(uint16_t*)(data_) = (size & 0x7FFFFFFF) | (*(uint16_t*)(data_) & 0x8000 ); }
      inline bool isDiffed() const
        { return ( (*(uint16_t*)data_) & 0x8000 ) == 0x8000; }
      inline void setDiffed( bool b)
        { *(uint16_t*)(data_) = (b << 15) | (*(uint16_t*)(data_) & 0x7FFF ); }
      inline uint32_t getObjectID() const
        { return *(uint32_t*)(data_+2); }
      inline void setObjectID(uint32_t objectID_)
        { *(uint32_t*)(data_+2) = objectID_; }
      inline void operator=(SynchronisableHeaderLight& h)
        { memcpy(data_, h.data_, SynchronisableHeaderLight::getSize()); }
  };
  
  typedef uint8_t VariableID;
  
  /**
   * @brief: stores information about a Synchronisable
   *
   * This class stores the information about a Synchronisable (objectID_, classID_, creatorID_, dataSize)
   * in an emulated bitset.
   * Bit 1 to 31 store the size of the Data the synchronisable consumes in the stream
   * Bit 32 is a bool and defines whether the variables are stored in diff mode
   * Byte 5 to 8: objectID_
   * Byte 9 to 12: classID_
   * Byte 13 to 16: creatorID_
   */
  class _NetworkExport SynchronisableHeader: public SynchronisableHeaderLight
  {
    public:
      SynchronisableHeader(uint8_t* data): SynchronisableHeaderLight(data)
        {}
      inline static uint32_t getSize()
        { return SynchronisableHeaderLight::getSize()+8; }
      inline uint32_t getClassID() const
        { return *(uint32_t*)(data_+SynchronisableHeaderLight::getSize()); }
      inline void setClassID(uint32_t classID_)
        { *(uint32_t*)(data_+SynchronisableHeaderLight::getSize()) = classID_; }
      inline uint32_t getCreatorID() const
        { return *(uint32_t*)(data_+SynchronisableHeaderLight::getSize()+4); }
      inline void setCreatorID(uint32_t creatorID_)
        { *(uint32_t*)(data_+SynchronisableHeaderLight::getSize()+4) = creatorID_; }
      inline void operator=(SynchronisableHeader& h)
        { memcpy(data_, h.data_, getSize()); }
  };
  
//   inline void operator=(SynchronisableHeaderLight& h1, SynchronisableHeader& h2)
//   {
//     memcpy(h1.data_, h2.data_, h1.getSize());
//   }

  /**
  * This class is the base class of all the Objects in the universe that need to be synchronised over the network
  * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list.
  * @author Oliver Scheuss
  */
  class _NetworkExport Synchronisable : virtual public OrxonoxClass{
  public:
    friend class packet::Gamestate;
    virtual ~Synchronisable();

    static void setClient(bool b);

    static Synchronisable *fabricate(uint8_t*& mem, uint8_t mode=0x0);
    static bool deleteObject(uint32_t objectID_);
    static Synchronisable *getSynchronisable(uint32_t objectID_);
    static unsigned int getNumberOfDeletedObject(){ return deletedObjects_.size(); }
    static uint32_t popDeletedObject(){ uint32_t i = deletedObjects_.front(); deletedObjects_.pop(); return i; }

    inline uint32_t getObjectID() const {return this->objectID_;}
    inline unsigned int getCreatorID() const {return this->creatorID_;}
    inline uint32_t getClassID() const {return this->classID_;}
    inline unsigned int getPriority() const { return this->objectFrequency_;}
    inline uint8_t getSyncMode() const { return this->objectMode_; }

    void setSyncMode(uint8_t mode);
    
    inline uint32_t getNrOfVariables(){ return this->syncList_.size(); }
    inline uint32_t getVarSize( VariableID ID )
    { return this->syncList_[ID]->getSize(state_); }

  protected:
    Synchronisable(BaseObject* creator);
    template <class T> void registerVariable(T& variable, uint8_t mode=0x1, NetworkCallbackBase *cb=0, bool bidirectional=false);
    template <class T> void registerVariable(std::set<T>& variable, uint8_t mode=0x1, NetworkCallbackBase *cb=0, bool bidirectional=false);
    template <class T> void unregisterVariable(T& var);

    void setPriority(unsigned int freq){ objectFrequency_ = freq; }


  private:
    uint32_t getData(uint8_t*& mem, std::vector<uint32_t>& sizes, int32_t id, uint8_t mode);
    uint32_t getSize(int32_t id, uint8_t mode=0x0);
    bool updateData(uint8_t*& mem, uint8_t mode=0x0, bool forceCallback=false);
    bool doSync(/*int32_t id,*/ uint8_t mode=0x0);
    bool doReceive( uint8_t mode );

    inline void setObjectID(uint32_t id){ this->objectID_ = id; objectMap_[this->objectID_] = this; }
    inline void setClassID(uint32_t id){ this->classID_ = id; }

    uint32_t objectID_;
    uint32_t creatorID_;
    uint32_t classID_;

    std::vector<SynchronisableVariableBase*> syncList_;
    std::vector<SynchronisableVariableBase*> stringList_;
    uint32_t dataSize_; //size of all variables except strings
    static uint8_t state_; // detemines wheter we are server (default) or client
    bool backsync_; // if true the variables with mode > 1 will be synchronised to server (client -> server)
    unsigned int objectFrequency_;
    int objectMode_;
    static std::map<uint32_t, Synchronisable *> objectMap_;
    static std::queue<uint32_t> deletedObjects_;
  };

  template <class T> void Synchronisable::registerVariable(T& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional)
  {
    if (bidirectional)
    {
      syncList_.push_back(new SynchronisableVariableBidirectional<T>(variable, mode, cb));
      this->dataSize_ += syncList_.back()->getSize(state_);
    }
    else
    {
      syncList_.push_back(new SynchronisableVariable<T>(variable, mode, cb));
      if ( this->state_ == mode )
        this->dataSize_ += syncList_.back()->getSize(state_);
    }
  }
  
  template <class T> void Synchronisable::unregisterVariable(T& variable)
  {
    std::vector<SynchronisableVariableBase*>::iterator it = syncList_.begin();
    while(it!=syncList_.end())
    {
      if( ((*it)->getReference()) == &variable )
      {
        this->dataSize_ -= (*it)->getSize(Synchronisable::state_);
        delete (*it);
        syncList_.erase(it);
        return;
      }
      else
        it++;
    }
    COUT(1) << "Tried to unregister not registered variable" << endl;
    assert(false); //if we reach this point something went wrong:
    // the variable has not been registered before
  }

  template <class T> void Synchronisable::registerVariable( std::set<T>& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional)
  {
    SynchronisableVariableBase* sv;
    if (bidirectional)
      sv = new SynchronisableVariableBidirectional<std::set<T> >(variable, mode, cb);
    else
      sv = new SynchronisableVariable<std::set<T> >(variable, mode, cb);
    syncList_.push_back(sv);
    stringList_.push_back(sv);
  }

  template <> _NetworkExport void Synchronisable::registerVariable( std::string& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional);
//   template <class T> _NetworkExport void Synchronisable::registerVariable<std::set<T> >( std::set<T>& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::unregisterVariable( std::string& variable );


}

#endif /* _Synchronisable_H__ */
