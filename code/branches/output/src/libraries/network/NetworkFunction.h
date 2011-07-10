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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _NetworkFunction_H__
#define _NetworkFunction_H__

#include "NetworkPrereqs.h"

#include <cassert>
#include <cstring>
#include <map>
#include <string>
#include <boost/preprocessor/cat.hpp>
#include <boost/static_assert.hpp>

#include "core/Identifier.h"
#include "core/command/Functor.h"
#include "FunctionCallManager.h"
#include "synchronisable/Synchronisable.h"

namespace orxonox
{

#if defined(ORXONOX_COMPILER_GCC) && defined(ORXONOX_ARCH_32)
static const unsigned int MAX_FUNCTION_POINTER_SIZE = 8;
#else
static const unsigned int MAX_FUNCTION_POINTER_SIZE = 16;
#endif //ORXONOX_COMPILER_GCC
static const unsigned int MAX_FUNCTION_POINTER_INTS = (MAX_FUNCTION_POINTER_SIZE-1)/4+1;

struct _NetworkExport NetworkFunctionPointer {
  uint32_t pointer[MAX_FUNCTION_POINTER_INTS];
  bool operator<(const NetworkFunctionPointer& b) const
  {
#if defined(ORXONOX_COMPILER_GCC) && defined(ORXONOX_ARCH_32)
    return pointer[0]<b.pointer[0] ? true : pointer[1]<b.pointer[1];
#else //ORXONOX_COMPILER_GCC
    return pointer[0]<b.pointer[0] ? true : ( pointer[1]<b.pointer[1] ? true : ( pointer[2]<b.pointer[2] ? true : pointer[3]<b.pointer[3] ) );
#endif //ORXONOX_COMPILER_GCC
  }
};





class _NetworkExport NetworkFunctionBase: virtual public OrxonoxClass {
  public:
    NetworkFunctionBase(const std::string& name);
    ~NetworkFunctionBase();

    virtual void        setNetworkID(uint32_t id)       { this->networkID_ = id; }
    inline uint32_t     getNetworkID() const            { return this->networkID_; }
    inline const std::string& getName() const           { return name_; }
    static inline bool  isStatic( uint32_t networkID )  { return isStaticMap_[networkID]; }

    static inline void setNetworkID(const std::string& name, uint32_t id)
    {
        std::map<std::string, NetworkFunctionBase*>& map = NetworkFunctionBase::getNameMap();
        assert( map.find(name)!=map.end() );
        map[name]->setNetworkID(id);
    }

    static void destroyAllNetworkFunctions();

  protected:
    static std::map<uint32_t, bool> isStaticMap_;

  private:
    static std::map<std::string, NetworkFunctionBase*>& getNameMap();
    uint32_t networkID_;
    std::string name_;

};


class _NetworkExport NetworkFunctionStatic: public NetworkFunctionBase {
  public:
    NetworkFunctionStatic(const FunctorStaticPtr& functor, const std::string& name, const NetworkFunctionPointer& p);

    inline void call(){ (*this->functor_)(); }
    inline void call(const MultiType& mt1){ (*this->functor_)(mt1); }
    inline void call(const MultiType& mt1, const MultiType& mt2){ (*this->functor_)(mt1, mt2); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3){ (*this->functor_)(mt1, mt2, mt3); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4){ (*this->functor_)(mt1, mt2, mt3, mt4); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5){ (*this->functor_)(mt1, mt2, mt3, mt4, mt5); }

    virtual void setNetworkID( uint32_t id )
        { NetworkFunctionBase::setNetworkID( id ); NetworkFunctionStatic::getIdMap()[id] = this; }
    static inline NetworkFunctionStatic* getNetworkFunction( uint32_t id)
        { assert( NetworkFunctionStatic::getIdMap().find(id)!=NetworkFunctionStatic::getIdMap().end() ); return NetworkFunctionStatic::getIdMap()[id]; }
    static NetworkFunctionStatic* getFunction( uint32_t id )
        { assert( NetworkFunctionStatic::getIdMap().find(id) != NetworkFunctionStatic::getIdMap().end() ); return NetworkFunctionStatic::getIdMap()[id]; }
    static NetworkFunctionStatic* getFunction( const NetworkFunctionPointer& p )
        { assert( NetworkFunctionStatic::getFunctorMap().find(p) != NetworkFunctionStatic::getFunctorMap().end() ); return NetworkFunctionStatic::getFunctorMap()[p]; }

  private:
    static std::map<NetworkFunctionPointer, NetworkFunctionStatic*>& getFunctorMap();
    static std::map<uint32_t, NetworkFunctionStatic*>& getIdMap();
    FunctorStaticPtr functor_;

};


class _NetworkExport NetworkMemberFunctionBase: public NetworkFunctionBase {
  public:
    NetworkMemberFunctionBase(const std::string& name, const NetworkFunctionPointer& p);
    ~NetworkMemberFunctionBase();

    virtual void setNetworkID( uint32_t id ){ NetworkFunctionBase::setNetworkID( id ); idMap_[id] = this; }
    static inline NetworkMemberFunctionBase* getNetworkFunction( uint32_t id){ assert( idMap_.find(id)!=idMap_.end() ); return idMap_[id]; }
    static NetworkMemberFunctionBase* getFunction( uint32_t id ){ assert( idMap_.find(id) != idMap_.end() ); return idMap_[id]; }
    static NetworkMemberFunctionBase* getFunction( const NetworkFunctionPointer& p ){ assert( functorMap_.find(p) != functorMap_.end() ); return functorMap_[p]; }

    //
    virtual bool call(uint32_t objectID)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)=0;

  private:
    static std::map<NetworkFunctionPointer, NetworkMemberFunctionBase*> functorMap_;
    static std::map<uint32_t, NetworkMemberFunctionBase*> idMap_;
};


template <class T> class NetworkMemberFunction: public NetworkMemberFunctionBase {
  public:
    NetworkMemberFunction(const FunctorMemberPtr<T>& functor, const std::string& name, const NetworkFunctionPointer& p);

    inline bool call(uint32_t objectID)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)));
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3, mt4);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3, mt4, mt5);
        return true;
      }
      else
        return false;
    }

  private:
    FunctorMemberPtr<T> functor_;
};

template <class T> NetworkMemberFunction<T>::NetworkMemberFunction(const FunctorMemberPtr<T>& functor, const std::string& name, const NetworkFunctionPointer& p):
    NetworkMemberFunctionBase(name, p), functor_(functor)
{
}

template<class T> inline void copyPtr( T ptr, NetworkFunctionPointer& destptr)
{
  if( sizeof(NetworkFunctionPointer)-sizeof(T) > 0)
    memset((uint8_t*)&destptr + sizeof(T), 0, sizeof(NetworkFunctionPointer)-sizeof(T));
  T p2 = ptr;
  memcpy( &destptr, &p2, sizeof(T) );
//   for(unsigned int i=0; i<(sizeof(T)-1/4)+1; i++)
//     *((uint32_t*)destptr+i) = p2>>32*i;
}

template<class T> inline void* registerStaticNetworkFunctionFct( T ptr, const std::string& name )
{
  BOOST_STATIC_ASSERT( sizeof(T)<=sizeof(NetworkFunctionPointer) ); // if this fails your compiler uses bigger pointers for static functions than defined above
  NetworkFunctionPointer destptr;
  copyPtr( ptr, destptr );
  new NetworkFunctionStatic( createFunctor(ptr), name, destptr );
  return 0;
}

template<class T, class PT> inline void* registerMemberNetworkFunctionFct( PT ptr, const std::string& name )
{
  BOOST_STATIC_ASSERT( sizeof(PT)<=sizeof(NetworkFunctionPointer) ); // if this fails your compiler uses bigger pointers for a specific kind of member functions than defined above
  NetworkFunctionPointer destptr;
  copyPtr( ptr, destptr );
  new NetworkMemberFunction<T>( createFunctor(ptr), name, destptr );
  return 0;
}

#define registerStaticNetworkFunction( functionPointer ) \
  static void* BOOST_PP_CAT( NETWORK_FUNCTION_, __UNIQUE_NUMBER__ ) = registerStaticNetworkFunctionFct( functionPointer, #functionPointer );
#define registerMemberNetworkFunction( class, function ) \
  static void* BOOST_PP_CAT( NETWORK_FUNCTION_##class, __UNIQUE_NUMBER__ ) = registerMemberNetworkFunctionFct<class>( &class::function, #class "_" #function);
  // call it with functionPointer, clientID, args
#define callStaticNetworkFunction( functionPointer, ...) \
  { \
    NetworkFunctionPointer p1; \
    copyPtr( functionPointer, p1 ); \
    FunctionCallManager::addCallStatic(NetworkFunctionStatic::getFunction(p1)->getNetworkID(), __VA_ARGS__); \
  }
  // call it with class, function, objectID, clientID, args
#define callMemberNetworkFunction( class, function, objectID, ...) \
  { \
    NetworkFunctionPointer p1; \
    copyPtr( &class::function, p1 ); \
    FunctionCallManager::addCallMember(NetworkMemberFunctionBase::getFunction(p1)->getNetworkID(), objectID, __VA_ARGS__); \
  }


}

#endif /* _NetworkFunction_H__ */
