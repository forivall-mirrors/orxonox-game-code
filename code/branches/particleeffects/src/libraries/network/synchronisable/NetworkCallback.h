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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */


#ifndef _NetworkCallback_H__
#define _NetworkCallback_H__

#include "network/NetworkPrereqs.h"
#include "NetworkCallbackManager.h"
// #include "util/MultiType.h"

namespace orxonox{

  struct EmptyType{};

  class _NetworkExport NetworkCallbackBase
  {
    public:
      virtual void call() = 0;
      NetworkCallbackBase(){ NetworkCallbackManager::registerCallback( this ); }
      virtual ~NetworkCallbackBase() {}
  };

  template <class T>
  class NetworkCallback: public NetworkCallbackBase
  {
    public:
      NetworkCallback(T* object, void (T::*function) (void)) : object_(object), function_(function) {}
      virtual ~NetworkCallback() {}
      virtual void call()
        { (this->object_->*function_)(); }

    private:
      T* object_;
      void (T::*function_) (void);
  };

  template <class T, class U>
  class NetworkCallbackNotify: public NetworkCallbackBase
  {
    public:
      NetworkCallbackNotify(T* object, void (T::*function) (const U&)) : object_(object), function_(function) {}
      NetworkCallbackNotify() {}
      virtual ~NetworkCallbackNotify() {}
      virtual void call()
        { (this->object_->*function_)( this->oldValue_ ); }
      void setOldValue(const U& value){ this->oldValue_ = value; }
    private:
      T* object_;
      U oldValue_;
      void (T::*function_) (const U&);
  };

}


#endif /* _NetworkCallback_H__ */
