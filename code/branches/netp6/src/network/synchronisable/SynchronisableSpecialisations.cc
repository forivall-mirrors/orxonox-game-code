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

#include <string>
#include "util/Math.h"
#include "Synchronisable.h"
#include "SynchronisableVariable.h"

// ================ template spezialisation
  
  
namespace orxonox{
  
//   template <> void Synchronisable::registerVariable( const std::string& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional)
//   {
//     if (bidirectional)
//       syncList.push_back(new SynchronisableVariableBidirectional<const std::string>(variable, mode, cb));
//     else
//       syncList.push_back(new SynchronisableVariable<const std::string>(variable, mode, cb));
//     stringList.push_back(syncList.back());
//   }
  
  template <> void Synchronisable::registerVariable( std::string& variable, uint8_t mode, NetworkCallbackBase *cb, bool bidirectional)
  {
    if (bidirectional)
      syncList.push_back(new SynchronisableVariableBidirectional<const std::string>(variable, mode, cb));
    else
      syncList.push_back(new SynchronisableVariable<const std::string>(variable, mode, cb));
    stringList.push_back(syncList.back());
  }
  
  template <> void Synchronisable::registerVariable( const ColourValue& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable(variable.r, mode, cb, bidirectional);
    registerVariable(variable.g, mode, cb, bidirectional);
    registerVariable(variable.b, mode, cb, bidirectional);
    registerVariable(variable.a, mode, cb, bidirectional);
  }
  template <> void Synchronisable::registerVariable( ColourValue& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    return Synchronisable::registerVariable( (const ColourValue&)variable, mode, cb, bidirectional);
  }

  template <> void Synchronisable::registerVariable( const Vector2& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable(variable.x, mode, cb, bidirectional);
    registerVariable(variable.y, mode, cb, bidirectional);
  }
  
  template <> void Synchronisable::registerVariable( const Vector3& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable(variable.x, mode, cb, bidirectional);
    registerVariable(variable.y, mode, cb, bidirectional);
    registerVariable(variable.z, mode, cb, bidirectional);
  }
  template <> void Synchronisable::registerVariable( Vector3& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable( (const Vector3&)variable, mode, cb, bidirectional);
  }

  template <> void Synchronisable::registerVariable( const Vector4& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable(variable.x, mode, cb, bidirectional);
    registerVariable(variable.y, mode, cb, bidirectional);
    registerVariable(variable.z, mode, cb, bidirectional);
    registerVariable(variable.w, mode, cb, bidirectional);
  }
  template <> void Synchronisable::registerVariable( Vector4& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable( (const Vector4&)variable, mode, cb, bidirectional);
  }
  
  template <> void Synchronisable::registerVariable( mbool& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable(variable.getMemory(), mode, cb, bidirectional);
  }
  
  template <> void Synchronisable::registerVariable( const Quaternion& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable(variable.x, mode, cb, bidirectional);
    registerVariable(variable.y, mode, cb, bidirectional);
    registerVariable(variable.z, mode, cb, bidirectional);
    registerVariable(variable.w, mode, cb, bidirectional);
  }
  template <> void Synchronisable::registerVariable( Quaternion& variable, uint8_t mode, NetworkCallbackBase* cb, bool bidirectional)
  {
    registerVariable( (const Quaternion&)variable, mode, cb, bidirectional);
  }

  

} //namespace
