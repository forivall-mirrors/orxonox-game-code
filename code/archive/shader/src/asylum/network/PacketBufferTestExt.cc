/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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

#include <string>
#include <iostream>
#include <enet/enet.h>
#include <boost/thread/thread.hpp>

#include "util/Sleep.h"
#include "PacketBuffer.h"
#include "PacketBuffer.cc"

using namespace network;

void write(PacketBuffer *test){
  ENetEvent event;
  ENetPacket *packet;
  if(test->isEmpty())
    std::cout << "empty buffer" << std::endl;
  for(int i=0; i<10; i++){
    std::string temp = "packet ";
    packet = enet_packet_create("packet", strlen("packet ")+1,
      ENET_PACKET_FLAG_RELIABLE);
    std::cout << i << ": pushing " << packet->data << std::endl;
    event.packet=packet;
    test->push(&event);
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
      std::cout << "We popped the value " << test->pop()->data << std::endl;
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

