#include "network/PacketBuffer.h"
#include "network/PacketBuffer.cc"
#include <boost/thread/thread.hpp>
#include <iostream>
#include <string>

using namespace network;


void write(PacketBuffer *test){
  PacketEnvelope p;
  if(test->isEmpty())
    std::cout << "empty buffer" << std::endl;
  for(int i=0; i<10; i++){
    p.data=i*i;
    std::cout << i << ": pushing " << p.data << std::endl;
    test->push(p);
    if(i==5)
      usleep(200000);
  }
  test->setClosed(true);
  return;
}

void read(PacketBuffer *test){
  //test->print();
  // exit if the queue is closed and empty
  while(!test->isClosed() || !test->isEmpty()){
    // only pop if the queue isn't empty
    while(!test->isEmpty()){
      int i=test->pop().data;
      std::cout << "We popped the value " << i << std::endl;
    }
  }
  return;
}


int main(int argc, char **argv[]){
  PacketBuffer test = PacketBuffer();
  boost::thread thrd1(boost::bind(&write, &test));
  boost::thread thrd2(boost::bind(&read, &test));
    
  thrd1.join();
  thrd2.join();
  
  return 0;
}

