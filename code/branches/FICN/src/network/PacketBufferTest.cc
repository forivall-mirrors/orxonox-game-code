#include "PacketBuffer.h"
#include "PacketBuffer.cc"
#include <iostream>
#include <string>

using namespace network;

int main(int argc, char **argv[]){
  PacketBuffer test = PacketBuffer();
  PacketEnvelope p={8, 5};
  if(test.isEmpty())
    std::cout << "empty buffer" << std::endl;
  for(int i=0; i<10; i++){
    p.data=i*i;
    std::cout << i << ": pushing " << p.data << std::endl;
    test.push((ENetEvent*)&p);
  }
  std::cout << std::endl << "queue.print()" << std::endl;
  test.print();
  while(!test.isEmpty()){
    int i = (int)test.pop()->data;
    std::cout << "We popped the value " << i << std::endl;
  }
  return 0;
}
