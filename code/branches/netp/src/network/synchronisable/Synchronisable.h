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

#include <list>
#include <vector>
#include <map>
#include <queue>
#include <cassert>
#include <string>
#include "util/Math.h"
#include "util/mbool.h"
#include "core/OrxonoxClass.h"
#include "NetworkCallback.h"
#include "SynchronisableVariable.h"

/*#define REGISTERDATA(varname, ...) \
    registerVariable((void*)&varname, sizeof(varname), DATA, __VA_ARGS__)
#define REGISTERSTRING(stringname, ...) \
    registerVariable(&stringname, stringname.length()+1, STRING, __VA_ARGS__)*/

namespace orxonox
{

  namespace objectDirection{
    enum objectdirection{
      toclient=0x1,
      toserver=0x2,
      bidirectional=0x3
    };
  }

  namespace priority{
    enum prio{
      very_high   = -100,
      high        = -15,
      normal      = 0,
      low         = 15,
      very_low    = 100
    };
  }

  /**
   * @brief: stores information about a Synchronisable 
   * 
   * This class stores the information about a Synchronisable (objectID, classID, creatorID, dataSize)
   * in an emulated bitset.
   * Bit 1 to 31 store the size of the Data the synchronisable consumes in the stream
   * Bit 32 is a bool and defines whether the data is actually stored or is just filled up with 0
   * Byte 5 to 8: objectID
   * Byte 9 to 12: classID
   * Byte 13 to 16: creatorID
   */
  class _NetworkExport SynchronisableHeader{
    private:
      uint8_t *data_;
    public:
      SynchronisableHeader(uint8_t* data)
        { data_ = data; }
      inline static uint32_t getSize()
        { return 16; }
      inline uint32_t getDataSize() const
        { return (*(uint32_t*)data_) & 0x7FFFFFFF; } //only use the first 31 bits
      inline void setDataSize(uint32_t size)
        { *(uint32_t*)(data_) = (size & 0x7FFFFFFF) | (*(uint32_t*)(data_) & 0x80000000 ); }
      inline bool isDataAvailable() const
        { return ( (*(uint32_t*)data_) & 0x80000000 ) == 0x80000000; }
      inline void setDataAvailable( bool b)
        { *(uint32_t*)(data_) = (b << 31) | (*(uint32_t*)(data_) & 0x7FFFFFFF ); }
      inline uint32_t getObjectID() const
        { return *(uint32_t*)(data_+4); }
      inline void setObjectID(uint32_t objectID)
        { *(uint32_t*)(data_+4) = objectID; }
      inline uint32_t getClassID() const
        { return *(uint32_t*)(data_+8); }
      inline void setClassID(uint32_t classID)
        { *(uint32_t*)(data_+8) = classID; }
      inline uint32_t getCreatorID() const
        { return *(uint32_t*)(data_+12); }
      inline void setCreatorID(uint32_t creatorID)
        { *(uint32_t*)(data_+12) = creatorID; }
      inline void operator=(SynchronisableHeader& h)
        { memcpy(data_, h.data_, getSize()); }
  };


  /**
  * This class is the base class of all the Objects in the universe that need to be synchronised over the network
  * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list.
  * @author Oliver Scheuss
  */
  class _NetworkExport Synchronisable : virtual public OrxonoxClass{
  public:
    friend class packet::Gamestate;
//     friend class Server;
    virtual ~Synchronisable();

    static void setClient(bool b);

    static Synchronisable *fabricate(uint8_t*& mem, uint8_t mode=0x0);
    static bool deleteObject(uint32_t objectID);
    static Synchronisable *getSynchronisable(uint32_t objectID);
    static unsigned int getNumberOfDeletedObject(){ return deletedObjects_.size(); }
    static uint32_t popDeletedObject(){ uint32_t i = deletedObjects_.front(); deletedObjects_.pop(); return i; }

    inline uint32_t getObjectID() const {return objectID;}
    inline unsigned int getCreatorID() const {return creatorID;}
    inline uint32_t getClassID() const {return classID;}
    inline unsigned int getPriority() const { return objectFrequency_;}

  protected:
    Synchronisable(BaseObject* creator);
    template <class T> void registerVariable(T& variable, uint8_t mode=0x1, NetworkCallbackBase *cb=0, bool bidirectional=false);
    //template <class T> void unregisterVariable(T& var);
    void setObjectMode(uint8_t mode);
    void setPriority(unsigned int freq){ objectFrequency_ = freq; }


  private:
    uint32_t getData(uint8_t*& men, int32_t id, uint8_t mode=0x0);
    uint32_t getSize(int32_t id, uint8_t mode=0x0);
    bool updateData(uint8_t*& mem, uint8_t mode=0x0, bool forceCallback=false);
    bool isMyData(uint8_t* mem);
    bool doSync(int32_t id, uint8_t mode=0x0);

    uint32_t objectID;
    uint32_t creatorID;
    uint32_t classID;

    std::vector<SynchronisableVariableBase*> syncList;
    std::vector<SynchronisableVariableBase*> stringList;
    uint32_t dataSize_; //size of all variables except strings
    static uint8_t state_; // detemines wheter we are server (default) or client
    bool backsync_; // if true the variables with mode > 1 will be synchronised to server (client -> server)
    unsigned int objectFrequency_;
    int objectMode_;
    static std::map<uint32_t, Synchronisable *> objectMap_;
    static std::queue<uint32_t> deletedObjects_;
  };

  // ================= Specialisation declarations
  
//   template <> _NetworkExport void Synchronisable::registerVariable( const std::string& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( std::string& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( const ColourValue& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( ColourValue& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( const Vector2& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( Vector2& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( const Vector3& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( Vector3& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( const Vector4& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( Vector4& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( mbool& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( const Quaternion& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  template <> _NetworkExport void Synchronisable::registerVariable( Quaternion& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional);
  
  template <class T> void Synchronisable::registerVariable(T& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional)
  {
    if (bidirectional)
      syncList.push_back(new SynchronisableVariableBidirectional<const T>(variable, mode, cb));
    else
      syncList.push_back(new SynchronisableVariable<const T>(variable, mode, cb));
    if ( this->state_ == mode )
      this->dataSize_ += syncList.back()->getSize(state_);
  }
  


//   template <class T> void Synchronisable::unregisterVariable(T& var){
//     std::vector<SynchronisableVariableBase*>::iterator it = syncList.begin();
//     while(it!=syncList.end()){
//       if( ((*it)->getReference()) == &var ){
//         delete (*it);
//         syncList.erase(it);
//         return;
//       }
//       else
//         it++;
//     }
//     bool unregistered_nonexistent_variable = false;
//     assert(unregistered_nonexistent_variable); //if we reach this point something went wrong:
//     // the variable has not been registered before
//   }

  
}

#endif /* _Synchronisable_H__ */
