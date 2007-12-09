#include <iostream>
#include "Server.h"
#include "PacketManager.h"
#include "PacketTypes.h"

namespace network{
  
class dummyserver3 : Server{
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
