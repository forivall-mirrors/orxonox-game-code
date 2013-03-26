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

#include "NetworkFunction.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
  std::map<uint32_t, bool> NetworkFunctionBase::isStaticMap_;

  std::map<NetworkFunctionPointer, NetworkMemberFunctionBase*> NetworkMemberFunctionBase::functorMap_;
  std::map<uint32_t, NetworkMemberFunctionBase*> NetworkMemberFunctionBase::idMap_;

  NetworkFunctionBase::NetworkFunctionBase(const std::string& name)
  {
    RegisterRootObject(NetworkFunctionBase);

    static uint32_t networkID = 0;
    this->networkID_ = networkID++;

    this->name_ = name;
    NetworkFunctionBase::getNameMap()[name] = this;
  }
  NetworkFunctionBase::~NetworkFunctionBase()
  {
  }


  void NetworkFunctionBase::destroyAllNetworkFunctions()
  {
    std::map<std::string, NetworkFunctionBase*>& map = NetworkFunctionBase::getNameMap();
    std::map<std::string, NetworkFunctionBase*>::iterator it;
    for( it=map.begin(); it!=map.end(); ++it )
      delete it->second;
  }


  /*static*/ std::map<std::string, NetworkFunctionBase*>& NetworkFunctionBase::getNameMap()
  {
    static std::map<std::string, NetworkFunctionBase*> nameMap_;
    return nameMap_;
  }


  NetworkFunctionStatic::NetworkFunctionStatic(const FunctorStaticPtr& functor, const std::string& name, const NetworkFunctionPointer& p):
    NetworkFunctionBase(name)
  {
    RegisterObject(NetworkFunctionStatic);

    this->functor_ = functor;
    NetworkFunctionStatic::getFunctorMap()[p] = this;
    NetworkFunctionStatic::getIdMap()[ this->getNetworkID() ] = this;
  }

  /*static*/ std::map<NetworkFunctionPointer, NetworkFunctionStatic*>& NetworkFunctionStatic::getFunctorMap()
  {
    static std::map<NetworkFunctionPointer, NetworkFunctionStatic*> functorMap_;
    return functorMap_;
  }

  /*static*/ std::map<uint32_t, NetworkFunctionStatic*>& NetworkFunctionStatic::getIdMap()
  {
    static std::map<uint32_t, NetworkFunctionStatic*> idMap_;
    return idMap_;
  }


  NetworkMemberFunctionBase::NetworkMemberFunctionBase(const std::string& name, const NetworkFunctionPointer& p):
    NetworkFunctionBase(name)
  {
    RegisterObject(NetworkMemberFunctionBase);

    this->functorMap_[p] = this;
    this->idMap_[ this->getNetworkID() ] = this;
  }

  NetworkMemberFunctionBase::~NetworkMemberFunctionBase()
  {
  }


}
