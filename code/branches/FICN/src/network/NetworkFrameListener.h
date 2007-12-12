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

#ifndef NETWORKFRAMELISTENER_H
#define NETWORKFRAMELISTENER_H

#include "OgreFrameListener.h"
#include "Server.h"
#include "Client.h"
// #include "orxonox/orxonox.cc"
network::Server *server_g;
network::Client *client_g;

namespace network{


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
    client_g->tick(evt.timeSinceLastFrame);
    return FrameListener::frameStarted(evt);
  }
};




}

#endif
