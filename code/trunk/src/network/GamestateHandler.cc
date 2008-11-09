#include <assert.h>

#include "GamestateHandler.h"
#include "packet/Packet.h"

namespace orxonox {

GamestateHandler *GamestateHandler::instance_=0;
  
GamestateHandler::GamestateHandler()
{
  assert(instance_==0);
  instance_=this;
}


GamestateHandler::~GamestateHandler()
{
  instance_=0;
}



}//namespace orxonox
