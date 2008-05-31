/* *   ORXONOX - the hottest 3D action shooter ever to exist *                    > www.orxonox.net < * * *   License notice: * *   This program is free software; you can redistribute it and/or *   modify it under the terms of the GNU General Public License *   as published by the Free Software Foundation; either version 2 *   of the License, or (at your option) any later version. * *   This program is distributed in the hope that it will be useful, *   but WITHOUT ANY WARRANTY; without even the implied warranty of *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the *   GNU General Public License for more details. * *   You should have received a copy of the GNU General Public License *   along with this program; if not, write to the Free Software *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. * *   Author: *      Oliver Scheuss, (C) 2007 *   Co-authors: *      ... * */#include "Server.h"

#include <iostream>

#include "util/Sleep.h"
#include "PacketManager.h"
#include "PacketTypes.h"

namespace network
{

  class dummyserver3 : public Server
  {
  public:
    dummyserver3();
    ~dummyserver3();
    void loop();
  private:
    void tick();
    void processChat( chat *data, int clientId);


  };

  dummyserver3::dummyserver3(){
  }
  dummyserver3::~dummyserver3(){
  }

  void dummyserver3::loop(){
    open();
    while(true){
      tick();
      usleep(100);
    }
  }

  void dummyserver3::processChat( chat *data, int clientId){
    std::cout << "Client " << clientId << " sent: " << data->message << std::endl;
    sendMSG(data->message);
  }

  void dummyserver3::tick(){
    processQueue();
  }

}

int main(int argc, char **argv[]){
  network::dummyserver3 server;
  server.loop();
  return 0;
}
