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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

//
// C++ Interface: NetworkFrameListener
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _NetworkFrameListener_H__
#define _NetworkFrameListener_H__

#include <OgreFrameListener.h>

#include "NetworkPrereqs.h"
#include "Server.h"
#include "Client.h"

network::Server *server_g;
network::Client *client_g;

namespace network
{
  class ServerFrameListener : public Ogre::FrameListener{
  private:
    bool frameStarted(const Ogre::FrameEvent &evt){
      server_g->tick(evt.timeSinceLastFrame);
      return FrameListener::frameStarted(evt);
    }
  };

  class ClientFrameListener : public Ogre::FrameListener{
  private:
    bool frameStarted(const Ogre::FrameEvent &evt){
      //std::cout << "framelistener" << std::endl;
      client_g->tick(evt.timeSinceLastFrame);
      return FrameListener::frameStarted(evt);
    }
  };




}

#endif /* _NetworkFrameListener_H__ */
