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

#include "OgreFrameListener.h"
#include "Server.h"
#include "Client.h"
//#include <iostream>
// #include "orxonox/Orxonox.cc"
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
    //std::cout << "framelistener" << std::endl;
    client_g->tick(evt.timeSinceLastFrame);
    return FrameListener::frameStarted(evt);
  }
};




}

#endif /* _NetworkFrameListener_H__ */
