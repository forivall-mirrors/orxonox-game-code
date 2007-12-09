#include <iostream>
#include <string>
#include "PacketManager.h"
#include "Client.h"

using namespace network;

int main(){
  network::PacketGenerator pck;
  const int PORT = 55556;
  std::cout << "Enter address of the server xxx.xxx.xxx.xxx (enter for localhost)" << std::endl;
  std::string str;
  std::getline(std::cin, str);
  std::cout << "You entered: " << str << std::endl;
  if(str.compare("")==0)
    str="127.0.0.1";
  
  Client client( str, PORT );
  if ( client.establishConnection() )
	  std::cout << "connection established" << std::endl;
  else std::cout << "problems establishing connection" << std::endl;
  
  while (true) {
	  client.update();
	  std::cout << "your message: ";
	  std::getline( std::cin, str );
	  client.sendChat( str );
	  std::cout << "send message" << std::endl;
  }
}
