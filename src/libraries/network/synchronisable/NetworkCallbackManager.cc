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

#include "NetworkCallbackManager.h"
#include "NetworkCallback.h"

namespace orxonox{

  std::set<NetworkCallbackBase*> NetworkCallbackManager::callbackSet_;
  std::queue<NetworkCallbackBase*> NetworkCallbackManager::triggeredCallbacks_;

  void NetworkCallbackManager::registerCallback(NetworkCallbackBase *cb)
  {
    callbackSet_.insert(cb);
  }
  void NetworkCallbackManager::deleteCallback(NetworkCallbackBase *cb)
  {
    std::set<NetworkCallbackBase*>::iterator it = callbackSet_.find(cb);
    if (it != callbackSet_.end())
    {
      callbackSet_.erase(it);
      delete cb;
    }
  }

  void NetworkCallbackManager::triggerCallback(NetworkCallbackBase *cb)
  {
    if (triggeredCallbacks_.empty() || triggeredCallbacks_.front() != cb)
      triggeredCallbacks_.push(cb);
  }

  void NetworkCallbackManager::callCallbacks()
  {
    while( triggeredCallbacks_.empty()==false )
    {
      //make sure callback hasn't been deleted before
      if ( callbackSet_.find(triggeredCallbacks_.front()) != callbackSet_.end() )
        triggeredCallbacks_.front()->call();
      triggeredCallbacks_.pop();
    }
  }
}
