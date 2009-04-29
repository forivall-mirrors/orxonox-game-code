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

#ifndef NETWORKFUNCTION_H
#define NETWORKFUNCTION_H

#include "NetworkPrereqs.h"
#include "core/OrxonoxClass.h"

#include <string>
#include <map>
#include <cassert>
#include "util/MultiType.h"
#include "synchronisable/Synchronisable.h"
#include "FunctionCallManager.h"


namespace orxonox
{

#ifdef ORXONOX_COMPILER_GCC
static const unsigned int MAX_FUNCTION_POINTER_SIZE = 8;
#else //ORXONOX_COMPILER_GCC
static const unsigned int MAX_FUNCTION_POINTER_SIZE = 16;
#endif //ORXONOX_COMPILER_GCC
static const unsigned int MAX_FUNCTION_POINTER_INTS = (MAX_FUNCTION_POINTER_SIZE-1)/4+1;

struct _NetworkExport NetworkFunctionPointer {
  uint32_t pointer[MAX_FUNCTION_POINTER_INTS];
  bool operator<(const NetworkFunctionPointer& b) const
  {
#ifdef ORXONOX_COMPILER_GCC
    return pointer[0]<b.pointer[0] ? true : pointer[1]<b.pointer[1];
#else //ORXONOX_COMPILER_GCC
    return pointer[0]<b.pointer[0] ? true : ( pointer[1]<b.pointer[1] ? true : ( pointer[2]<b.pointer[2] ? true : pointer[3]<b.pointer[3] ) );
#endif //ORXONOX_COMPILER_GCC
  }
};





class NetworkFunctionBase: virtual public OrxonoxClass {
  public:
    NetworkFunctionBase(std::string name);
    ~NetworkFunctionBase();
    
    inline void         setNetworkID(uint32_t id)       { this->networkID_ = id; }
    inline uint32_t     getNetworkID() const            { return this->networkID_; }
    inline std::string  getName() const                 { return name_; }
    static inline bool  isStatic( uint32_t networkID )  { return isStaticMap_[networkID]; }
    
    
    static inline void setNetworkID(std::string name, uint32_t id){ assert( nameMap_.find(name)!=nameMap_.end() ); nameMap_[name]->setNetworkID(id); }
    
  private:
    static std::map<std::string, NetworkFunctionBase*> nameMap_;
    static std::map<uint32_t, bool> isStaticMap_;
    uint32_t networkID_;
    std::string name_;
      
};


class _NetworkExport NetworkFunctionStatic: public NetworkFunctionBase {
  public:
    NetworkFunctionStatic(Functor* functor, std::string name, const NetworkFunctionPointer& p);
    ~NetworkFunctionStatic();
    
    inline void call(){ (*this->functor_)(); }
    inline void call(const MultiType& mt1){ (*this->functor_)(mt1); }
    inline void call(const MultiType& mt1, const MultiType& mt2){ (*this->functor_)(mt1, mt2); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3){ (*this->functor_)(mt1, mt2, mt3); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4){ (*this->functor_)(mt1, mt2, mt3, mt4); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5){ (*this->functor_)(mt1, mt2, mt3, mt4, mt5); }
    
    static inline NetworkFunctionStatic* getNetworkFunction( uint32_t id){ assert( idMap_.find(id)!=idMap_.end() ); return idMap_[id]; }
    static NetworkFunctionStatic* getFunction( uint32_t id ){ assert( idMap_.find(id) != idMap_.end() ); return idMap_[id]; }
    static NetworkFunctionStatic* getFunction( const NetworkFunctionPointer& p ){ assert( functorMap_.find(p) != functorMap_.end() ); return functorMap_[p]; }
    
  private:
    static std::map<NetworkFunctionPointer, NetworkFunctionStatic*> functorMap_;
    static std::map<uint32_t, NetworkFunctionStatic*> idMap_;
    
    FunctorStatic* functor_;
    
};


class _NetworkExport NetworkMemberFunctionBase: public NetworkFunctionBase {
  public:
    NetworkMemberFunctionBase(std::string name, const NetworkFunctionPointer& p);
    ~NetworkMemberFunctionBase();
    
    static inline NetworkMemberFunctionBase* getNetworkFunction( uint32_t id){ assert( idMap_.find(id)!=idMap_.end() ); return idMap_[id]; }
    static NetworkMemberFunctionBase* getFunction( uint32_t id ){ assert( idMap_.find(id) != idMap_.end() ); return idMap_[id]; }
    static NetworkMemberFunctionBase* getFunction( const NetworkFunctionPointer& p ){ assert( functorMap_.find(p) != functorMap_.end() ); return functorMap_[p]; }
    
    // 
    virtual void call(uint32_t objectID)=0;
    virtual void call(uint32_t objectID, const MultiType& mt1)=0;
    virtual void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2)=0;
    virtual void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)=0;
    virtual void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)=0;
    virtual void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)=0;
    
  private:
    static std::map<NetworkFunctionPointer, NetworkMemberFunctionBase*> functorMap_;
    static std::map<uint32_t, NetworkMemberFunctionBase*> idMap_;
};


template <class T> class _NetworkExport NetworkMemberFunction: public NetworkMemberFunctionBase {
  public:
    NetworkMemberFunction(FunctorMember<T>* functor, std::string name, const NetworkFunctionPointer& p);
    ~NetworkMemberFunction();
    
    inline void call(uint32_t objectID){ (*this->functor_)(dynamic_cast<T*>(Synchronisable::getSynchronisable(objectID))); }
    inline void call(uint32_t objectID, const MultiType& mt1){ (*this->functor_)(dynamic_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1); }
    inline void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2){ (*this->functor_)(dynamic_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2); }
    inline void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3){ (*this->functor_)(dynamic_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3); }
    inline void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4){ (*this->functor_)(dynamic_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3, mt4); }
    inline void call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5){ (*this->functor_)(dynamic_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3, mt4, mt5); }
    
  private:
    FunctorMember<T>* functor_;
};


template<class T> inline void copyPtr( T ptr, NetworkFunctionPointer& destptr)
{
  memset(&destptr, 0, sizeof(ptr));
  T p2 = ptr;
  memcpy( &destptr, &p2, sizeof(T) );
//   for(unsigned int i=0; i<(sizeof(T)-1/4)+1; i++)
//     *((uint32_t*)destptr+i) = p2>>32*i;
}

template<class T> inline void* registerStaticNetworkFunctionFct( T ptr, std::string name )
{
  NetworkFunctionPointer destptr;
  copyPtr( ptr, destptr );
  new NetworkFunctionStatic( createFunctor(ptr), name, destptr );
  return 0;
}

template<class T, class PT> inline void* registerMemberNetworkFunctionFct( PT ptr, std::string name )
{
  NetworkFunctionPointer destptr;
  copyPtr( ptr, destptr );
  new NetworkMemberFunction<T>( new FunctorMember<T>(ptr), name, destptr );
  return 0;
}

#define registerStaticNetworkFunction( functionPointer, name ) \
  void* NETWORK_FUNCTION_name##a = registerStaticNetworkFunctionFct( functionPointer, name );
#define registerMemberNetworkFunction( functionPointer, class, name ) \
  void* NETWORK_FUNCTION_class##name##a = registerMemberNetworkFunction<class>( functionPointer, name );
#define callStaticNetworkFunction( functionPointer, ...) \
  { \
    NetworkFunctionPointer p1; \
    copyPtr( functionPointer, p1 ); \
    FunctionCallManager::addCallStatic(NetworkMemberFunctionBase::getFunction(p1)->getNetworkID(), __VA_ARGS__); \
  }
#define callMemberNetworkFunction( functionPointer, objectID, ...) \
  { \
    NetworkFunctionPointer p1; \
    copyPtr( functionPointer, p1 ); \
    FunctionCallManager::addCallMember(NetworkMemberFunctionBase::getFunction(p1)->getNetworkID(), objectID, __VA_ARGS__) \
  }


}

#endif
