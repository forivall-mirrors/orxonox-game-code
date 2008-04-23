#include "enet/enet.h"
#include "Orxonox.h"
#include "NetworkPrereqs.h"
#include "PacketTypes.h"
#include "GameStateManager.h"
#include "Synchronisable.h"
#include "GameStateClient.h"
#include "iostream"
#include "core/CoreIncludes.h"
#include "time.h"
#include "ConnectionManager.h"
#include "ClientInformation.h"
#include "Synchronisable.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "util/Sleep.h"

using namespace network;

void printData( unsigned char* data, int length ) {
  for ( int i=0; i<length; i++ )
    std::cout << (int)data[i] << " ";
  std::cout << std::endl;
}

void printGameStateCompressed( GameStateCompressed* gc ) {
  //std::cout << "=================================================" << std::endl;
  std::cout << "GameStateCompressed id:\t\t" << gc->id << std::endl;
  std::cout << "GameStateCompressed normsize:\t" << gc->normsize << std::endl;
  std::cout << "GameStateCompressed compsize:\t" << gc->compsize << std::endl;
  std::cout << "GameStateCompressed diffed:\t" << gc->diffed << std::endl;
  //std::cout << "GameState data:\t" << gc->data << std::endl;
  std::cout << "GameStateCompressed compressing rate:\t" << 100.0-((100.0/(gc->normsize))*(gc->compsize)) << "%" << std::endl;
  std::cout << "=================================================" << std::endl;
  return;
}

bool compareData( GameState* g1, GameState* g2 ) {
  if ( g1->id != g2->id ) {
    std::cout << "\t--> GameStates are not comparable -> not same id" << std::endl;
    return true;
  }
  else if ( g1->size != g2->size ) {
    std::cout << "\t--> GameStates are not the same size!!" << std::endl;
    std::cout << g1->size << " != " << g2->size << std::endl;
  }
  int length = g1->size;
  for ( int i=0; i<length; i++ ) {
    if ( g1->data[i] != g2->data[i] ) {
      std::cout << "\t--> data of both GameStates are not identical" << std::endl;
      return false;
    }
  }
  //std::cout << "\t--> GameData are identical (compareData)" << std::endl;
  return true;
}

bool compareData2( GameState* g1, GameState* g2 ) {
  int length = g1->size;
  for ( int i=0; i<length; i++ ) {
    if ( g1->data[i] != g2->data[i] ) {
      return false;
    }
  }
  //std::cout << "\t--> GameData are identical (compareData)" << std::endl;
  return true;
}

bool compareGameStates( GameState* g1, GameState* g2 ) {
  if ( g1->id != g2->id ) {
    std::cout << "\t==> GameState id's not identical (GameStateCompare)" << std::endl;
  }
  if( g1->size != g2->size ) {
    std::cout << "\t==> GameState sizes are not identical (GameStateCompare)" << std::endl;
    return false;
  }
  else if ( g1->diffed != g2->diffed ) {
    std::cout << "\t==> GameState diffed params not identical (GameStateCompare)" << std::endl;
    return false;
  }
  else if ( !compareData2( g1, g2 ) ) {
    std::cout << "\t==> GameState data are not identical (GameStateCompare)" << std::endl;
    return false;
  }
  //std::cout << "\t==> GameStates are identical (GameStateCompare)" << std::endl;
  return true;
}

void printGameState( GameState* gstate ) {
  //std::cout << "=================================================" << std::endl;
  std::cout << "GameState id:\t\t" << gstate->id << std::endl;
  std::cout << "GameState size:\t\t" << gstate->size << std::endl;
  std::cout << "GameState diffed:\t" << gstate->diffed << std::endl;
  //std::cout << "GameState data:\t" << gstate->data << std::endl;
  std::cout << "=================================================" << std::endl;
  return;
}

unsigned char* createData( int length, int mode ) {
  char* data = new char[ length ];
  if ( mode == 1 ) {
    for ( int i=0; i<length; i++ )
      data[i] = (char)(i%255);
  }
  else if ( mode == 2 ) {
    for ( int i=0; i<length; i++ ) {
      if ( i%98 == 0 ) data[i] = (char)(i%255);
      else data[i] = (char)0;
    }
  }
  else if ( mode == 3 ) {
    for ( int i=0; i<length; i++ ){
      data[i] = (char)(rand()%255);
    }
  }
  else if ( mode == 4 ) {
    for ( int i=0; i<length; i++ ){
      data[i] = (char)(rand()%127);
    }
  }

  //printData( data, length );  

  unsigned char* dat = (unsigned char*)data;
  return dat;
}

GameState* changeGameStateABit( GameState* a, int mode ) {
  int length = a->size;
  GameState* b = new GameState;
  b->id = a->id;
  b->diffed = a->diffed;

  if ( mode == 1 ) {
    b->data = new unsigned char[length];
    b->size = a->size;
    for ( int i=0; i<length; i++ ) {
      if ( i%10 == 0 ) b->data[i] = rand()%255;
      else b->data[i] = a->data[i];
    }
  }
  else if ( mode == 2 ) {
    b->data = new unsigned char[length];
    b->size = length;
    for ( int i=0; i<length; i++ ) {
      if ( i%(rand()%((length)/11)+rand()) == 0 ) b->data[i] = (char)rand()%255;
      else b->data[i] = a->data[i];
    }
  }
  else if ( mode == 3 ) {
    int s = length + (rand()%(length));
    b->data = new unsigned char[s];
    b->size = s;
    for ( int i=0; i<length; i++ ) {
      if ( i%10 == 0 ) b->data[i] = (char)rand()%255;
      else b->data[i] = a->data[i];
    }
    for( int i=length; i<s; i++ ) {
      b->data[i] = (char)rand()%255;
    }
  }
  else if ( mode == 4 ) {
    int s = length + (rand()%(length));
    b->data = new unsigned char[s];
    b->size = s;
    for ( int i=0; i<length; i++ ) {
      if ( i%(rand()%(length)+rand()) == 0 ) b->data[i] = (char)rand()%255;
      else b->data[i] = a->data[i];
    }
    for( int i=length; i<s; i++ ) {
      b->data[i] = rand()%255;
    }
  }
    else if ( mode == 5 ) {
    int s = (length)/2;
    b->data = new unsigned char[s];
    b->size = s;
    for ( int i=0; i<s; i++ ) {
      if ( i%10 == 0 ) b->data[i] = (char)rand()%255;
      else b->data[i] = a->data[i];
    }
  }

  return b;
}

void testCompression( int size, int mode ) {
  std::cout << "testing compression with: size = " << size << " ,mode = " << mode << std::endl; 
  GameStateClient* g_client;
  GameStateManager* g_manager;;
  
  GameState* g_new = new GameState;
  GameState* g_old = new GameState;
  GameStateCompressed* gc = new GameStateCompressed;
  
  g_old->data = createData( size, mode );
  g_old->size = size;
  g_old->id = 0;
  g_old->diffed = false;
  printGameState( g_old );
  
  gc = g_manager->testCompress( g_old );
  printGameStateCompressed( gc );

  g_new = g_client->testDecompress( gc );
  printGameState( g_new );

  compareGameStates( g_new, g_old );

  return;
}

void testDifferentiation( int size, int modeCreateData, int modeChangeData ) {
  std::cout << "testing diff with: size = " << size << " modeCreateData = " 
            << modeCreateData << " modeChangeData = " << modeChangeData << std::endl; 
  GameStateClient* g_client;
  GameStateManager* g_manager;
  
  GameState* g_undiff1 = new GameState;
  GameState* g_undiff2 = new GameState;
  GameState* g_diffed;
  GameState* g_result;

  g_undiff1->data = createData( size, modeCreateData );
  g_undiff1->size = size;
  g_undiff1->id = 1;
  g_undiff1->diffed = false;
  printGameState( g_undiff1 );

  g_undiff2 = changeGameStateABit( g_undiff1, modeChangeData );
  printGameState( g_undiff2 );

  if( !compareData( g_undiff1, g_undiff2 ) ) std::cout << " BUT THAT'S HOW IT HAS TO BE" << std::endl;

  //printData( g_undiff1->data, g_undiff1->size );
  //printData( g_undiff2->data, g_undiff2->size );
  
  g_diffed = g_manager->testDiff( g_undiff1, g_undiff2 );
  printGameState( g_diffed );
  //printData( g_diffed->data, g_diffed->size );
  
  g_result = g_client->testUndiff( g_undiff1, g_diffed );
  compareGameStates( g_result, g_undiff2 );

  return;
}

void testCompressWithDiff( int size, int modeCreateData, int modeChangeData ) {
  std::cout << "testing CompressWithDiff with: size = " << size << " modeCreateData = " 
            << modeCreateData << " modeChangeData = " << modeChangeData << std::endl; 
  GameStateClient* g_client;
  GameStateManager* g_manager;;
  
  GameStateCompressed* gc = new GameStateCompressed;
  GameStateCompressed* g_compressedNoDiff;
  GameState* g_undiff1 = new GameState;
  GameState* g_undiff2 = new GameState;
  GameState* g_diffed;
  GameState* g_resultDiffed;
  GameState* g_resultUndiffed;

  g_undiff1->data = createData( size, modeCreateData );
  g_undiff1->size = size;
  g_undiff1->id = 1;
  g_undiff1->diffed = false;
  
  std::cout << "---First generated Gamestate" << std::endl;
  printGameState( g_undiff1 );

  g_undiff2 = changeGameStateABit( g_undiff1, modeChangeData );
  std::cout << "---First gererated Gamestate with some changes ev. longer" << std::endl;
  printGameState( g_undiff2 );  

  if( !compareData( g_undiff1, g_undiff2 ) ) std::cout << " DATA not identical.. ok" << std::endl;

  g_diffed = g_manager->testDiff( g_undiff1, g_undiff2 );
  std::cout << "---Diffed Gamestate not compressed" << std::endl;
  printGameState( g_diffed );

  gc = g_manager->testCompress( g_diffed );
  std::cout << "---Diffed Gamestate compressed" << std::endl;
  printGameStateCompressed( gc );

  g_compressedNoDiff = g_manager->testCompress( g_undiff2 );
  std::cout << "---Same not Diffed Gamestate compressen" << std::endl;
  printGameStateCompressed( g_compressedNoDiff ); 

  g_resultDiffed = g_client->testDecompress( gc );
  std::cout << "---Diffed Gamestate of above uncompressed" << std::endl;
  printGameState( g_resultDiffed );

  std::cout << "---Diffed Gamestates before compressed and after uncompress comparsion" << std::endl;
  if ( compareGameStates( g_resultDiffed, g_diffed ) ) std::cout << "GAMESTATES IDENTICAL" << std::endl;
 
  g_resultUndiffed = g_client->testUndiff( g_undiff1, g_resultDiffed );
  std::cout << "---New Gamestate of pseudo Server compared with new gamestate that Client gets" << std::endl;
  if ( compareGameStates( g_resultUndiffed, g_undiff2 ) ) std::cout << "GAMESTATES IDENTICAL" << std::endl;

  return;  
}

bool testNCompressWithDiff( int n, int size, int modeCreateData, int modeChangeData ) { 

  GameStateClient* g_client;
  GameStateManager* g_manager;;
  
  GameStateCompressed* gc = new GameStateCompressed;
  GameState* g_undiff1 = new GameState;
  GameState* g_undiff2 = new GameState;
  GameState* g_diffed;
  GameState* g_resultDiffed;
  GameState* g_resultUndiffed;

  g_undiff1->data = createData( size, modeCreateData );
  g_undiff1->size = size;
  g_undiff1->id = 1;
  g_undiff1->diffed = false;
  //l = -1;
  g_undiff2 = changeGameStateABit( g_undiff1, modeChangeData );

  while( compareData2( g_undiff1, g_undiff2 ) ) {
    delete g_undiff2->data;
    g_undiff2 = changeGameStateABit( g_undiff1, modeChangeData );
  } 
  //l = -2;
  g_diffed = g_manager->testDiff( g_undiff1, g_undiff2 );
  gc = g_manager->testCompress( g_diffed );
  g_resultDiffed = g_client->testDecompress( gc );

  if ( !compareGameStates( g_resultDiffed, g_diffed ) ) return false;
  //l = -3;
  g_resultUndiffed = g_client->testUndiff( g_undiff1, g_resultDiffed );
  if ( !compareGameStates( g_resultUndiffed, g_undiff2 ) ) return false;
  //l = 1;
  /*if ( gc != NULL && gc->data != NULL )
    delete gc->data;
  //l = 2;
  //if ( gc != NULL ) 
    //delete gc;
  //l = 3;
  if ( g_undiff1 != NULL && g_undiff1->data != NULL )
    delete g_undiff1->data;
  //l = 4; 
  //if ( g_undiff1 != NULL )
   //delete g_undiff1;
  //l = 5;
  if ( g_undiff2 != NULL && g_undiff2->data )
    delete g_undiff2->data;
  //l = 6; 
  //if ( g_undiff2 != NULL )
    //delete g_undiff2;
  //l = 7;
  if ( g_diffed != NULL && g_diffed->data )
    //delete g_diffed->data;
  //l = 8; 
  //if ( g_diffed )
    //delete g_diffed;
  //l = 9;
  if ( g_resultDiffed != NULL && g_resultDiffed->data )
    delete g_resultDiffed->data;
  //l = 10; 
  //if ( g_resultDiffed )
    //delete g_resultDiffed;
  //l = 11;*/
  
  return true;
}

bool testNCompression( int n, int size, int mode ) { 
  GameStateClient* g_client;
  GameStateManager* g_manager;;
  
  GameState* g_new = new GameState;
  GameState* g_old = new GameState;
  GameStateCompressed* gc = new GameStateCompressed;
  
  g_old->data = createData( size, mode );
  g_old->size = size;
  g_old->id = 0;
  g_old->diffed = false;
  
  gc = g_manager->testCompress( g_old );

  g_new = g_client->testDecompress( gc );

  if ( !compareGameStates( g_new, g_old ) ) return false;
  
  
  if ( g_new != NULL && g_new->data != NULL )
    delete g_new->data;

  if ( g_old != NULL && g_old->data != NULL )
    delete g_old->data;

  if ( gc != NULL && gc->data )
    delete gc->data;

  return true;
}

void printClientObjectMapping( ConnectionManager* cmanager, int clients ) {
  std::map<int, int>::iterator iter;
  std::map<int, int> clientsmap = cmanager->testGetClientsShip();   
  for( iter = clientsmap.begin(); iter != clientsmap.end(); iter++ ) {
    std::cout << "clientID: " << iter->first << "\t-> objectID: " << iter->second << std::endl;
  }
  return;
}

bool is( int a[], int b, int size ) {
  for ( int i=0; i<size; i++ ) {
    if ( a[i] == b ) return true;
  }
  return false;
}

void testClientObjectMapping( int clients ) {
  ConnectionManager* cmanager = new ConnectionManager();
  int shift = 2;
  std::cout << "create a map length [clients]" << std::endl;
  for ( int i=0; i<clients; i++ ) {
    cmanager->testAddClientsShipID( i, i+shift );
  }
  printClientObjectMapping( cmanager, clients );
  
  std::cout << "get random client's ship id" << std::endl;
  int id;
  for ( int i=0; i<(clients/3); i++ ) {
    id = rand()%clients;
    std::cout << "client: " << id << "\t-> ship: " << cmanager->testGetClientsShipID( id ) << std::endl;  
  }

  std::cout <<"get random ship's client id" << std::endl;
  for ( int i=0; i<(clients/3); i++ ) {
    id = (rand()%clients)+shift;
    std::cout << "ship:   " << id << "\t-> client: " << cmanager->testGetObjectsClientID( id ) << std::endl;  
  }

  std::cout << "delete random client from map" << std::endl;
  int deleted[clients/3];
  for ( int i=0; i<(clients/3); i++ ) {
    id = rand()%clients;
    if ( !is( deleted, id, clients/3 ) ) {
      std::cout << "delete client " << id << std::endl;
      cmanager->testDeleteClientsIDReg( id ); 
    }
    deleted[i] = id;
  }
  std::cout << "resulting list:" << std::endl;
  printClientObjectMapping( cmanager, clients-(clients/3));
  
  std::cout << "delete random object from map" << std::endl;
  int jap = 0;
  while( jap < 3 ) {
    id = (rand()%clients) + shift;
    if ( !is( deleted, id, clients/3 ) ) {
      std::cout << "delete object: " << id << std::endl;
      cmanager->testDeleteObjectIDReg( id );
      jap++;
    }
  }
  std::cout << "resulting list:" << std::endl;
  printClientObjectMapping( cmanager, clients-(clients/3)-3);
}

bool addClientTest( ENetEvent* event, ClientInformation*& head ) {
  ClientInformation *temp = head->insertBack(new ClientInformation);
  if(temp->prev()->head) {
    temp->prev()->setID(0);
    temp->setID(1);
  }
  else
    temp->setID(temp->prev()->getID()+1);
  temp->setPeer(event->peer);
  std::cout << "added client id: " << temp->getID() << std::endl;

  temp->setSynched(true);
  return true;
}

void printClientInformationBox( ClientInformation* box ) {
  std::cout << "ClientList: id: " << box->getID() << "\t";
  std::cout << "g_id: " << box->getGamestateID() << " \t";
  std::cout << "synched: " << box->getSynched() << "\t";
  std::cout << "is head: " << box->head << std::endl;
}

void printClientInformationList( ClientInformation* list ) {
  printClientInformationBox( list );
  list = list->next();
  
  while( list != 0 ) {
    printClientInformationBox( list );
    list = list->next();
  }
  return;
}

void testClientInformation( int numberOfClients ) {
  ClientInformation* head = new ClientInformation( true );
  ConnectionManager* connectionManager;
  ENetEvent event;
  
  for ( int i=0; i<numberOfClients; i++ ) {
    if ( !addClientTest( &event, head ) ) {
      std::cout << "addClientTest didn't work with: " << i << std::endl;
    }
  }
  std::cout << "(now id should be synched, since that works and this is test of list, this step is left out)" << std::endl;

  printClientInformationList( head );

  std::cout << "remove some clients" << std::endl;
  if ( head->removeClient( numberOfClients/3 ) ) std::cout << "client " << numberOfClients/3 << " removed" << std::endl;
  else std::cout << "could not remove client: " << numberOfClients/3 << std::endl;
  if ( head->removeClient( numberOfClients ) ) std::cout << "client " << numberOfClients << " removed" << std::endl;
  else std::cout << "could not remove client: " << numberOfClients << std::endl;
  if ( head->removeClient( 1 ) ) std::cout << "client " << 1 << " removed" << std::endl;
  else std::cout << "could not remove client: " << 1 << std::endl;
  if ( head->removeClient( 1 ) ) std::cout << "client " << 1 << " removed a second time" << std::endl;
  else std::cout << "could not remove client: " << 1 << std::endl;
  if ( head->removeClient( numberOfClients + 100 ) ) std::cout << "client " << numberOfClients + 100 << " removed a second time" << std::endl;
  else std::cout << "could not remove client: " << numberOfClients + 100 << std::endl;

  printClientInformationList( head );
  
  std::cout << "try to find some clients with findClient(..., false)" << std::endl;
  ClientInformation* temp = head->findClient( 2 );
  printClientInformationBox( temp );
  temp = head->findClient( numberOfClients/3 );
  printClientInformationBox( temp );
  temp = head->findClient( 0 );
  printClientInformationBox( temp );
  temp = head->findClient( 8 );
  printClientInformationBox( temp );

  std::cout << "find the same, output should be identical with above but with findClient(..., TRUE)" << std::endl;
  temp = head->findClient( 2, true );
  printClientInformationBox( temp );
  temp = head->findClient( numberOfClients/3, true );
  printClientInformationBox( temp );
  temp = head->findClient( 0, true );
  printClientInformationBox( temp );
  temp = head->findClient( 8, true );
  printClientInformationBox( temp );

  std::cout << "test setGamestateID" << std::endl;
  temp->setGamestateID( 8 );
  printClientInformationBox( temp );

  std::cout << "test insertAfter() and insertBefore()" << std::endl;
  ClientInformation* newInfo = new ClientInformation;
  ClientInformation* toool = new ClientInformation;
  newInfo->setGamestateID( 200 );
  newInfo->setID( numberOfClients+2);
  newInfo->setSynched( true );
  newInfo->setPeer( NULL );
  toool->setGamestateID( 199 );
  toool->setID( numberOfClients+1);
  toool->setSynched( true );
  toool->setPeer( NULL );

  //never use the same ClientInformation box in this situation
  //-> results in endless loop
  temp->insertAfter( newInfo );
  temp->insertBefore( toool );

  printClientInformationList( head );
  return;
}

//### following stuff is to test buffer, took from PacketBufferTestExt.cc ###

void write(PacketBuffer *test){
  ENetEvent event;
  ENetPacket *packet;
  if(test->isEmpty())
    std::cout << "buffer is empty" << std::endl;
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

void testPacketBuffer() {
  PacketBuffer test = PacketBuffer();
  boost::thread thrd1(boost::bind(&write, &test));
  boost::thread thrd2(boost::bind(&read, &test));

  thrd1.join();
  thrd2.join();
  return;
}

//### end packetbuffer test stuff ###

void testSynchronisable() {
  Synchronisable* synchtest;
  unsigned char* data = new unsigned char[100];
  for( int i=0; i<100; i++ ) {
    data[i] = i%10;
  }
  //synchtest->registerVar( (const void*)data, 100, network::DATA );
}

//#########################################################################
//### this stuff is only a loop to chose one of the prev test functions ###
//#########################################################################
void displayModes() {
  std::cout << "mode datalength: length of data array to create" << std::endl;
  std::cout << "mode Data:" << std::endl;
  std::cout << "\t-1 -> array[length] with numbers length%255" << std::endl;
  std::cout << "\t-2 -> array[length] with numbers length%255, every %98 is != 0" << std::endl;
  std::cout << "\t-3 -> array[length] with random numbers (-126:127) no modulo zeros" << std::endl;
  std::cout << "\t-4 -> array[length] with random numbers (0:127) no modulo zeros" << std::endl;
  std::cout << "---------------------------------------------------------------------------------" << std::endl;
  std::cout << "mode Change:" << std::endl;
  std::cout << "\t-1 -> every %10 == 0 index is different from original" << std::endl;
  std::cout << "\t-2 -> every %(rand()%(length/11)) is different from original" << std::endl;
  std::cout << "\t-3 -> every %10 == 0 index is different and randomly longer till 2xlonger" << std::endl;
  std::cout << "\t-4 -> random differences and randomly longer" << std::endl;
  std::cout << "\t-5 -> only half as long and ever %10 == 0 index is different" << std::endl;
}
/**
int main( int argc, char* argv[] ) {
  int a,b,c,n;
  std::string dec = "nothing";
  std::cout << "############### START TEST (quit q) ###############" << std::endl;
  std::cout << "possible tests: " << std::endl;
  std::cout << "displayModes:\t\t modes" << std::endl;
  std::cout << "testCompression:\t tc [datalength] [mode Data]" << std::endl;
  std::cout << "testNCompression:\t tnc [#of loops] [datalength] [mode Data]" << std::endl;
  std::cout << "testDifferentiation:\t td [datalength] [mode Data] [mode Change]" << std::endl;
  std::cout << "testCompressWithDiff:\t tcd [datalength] [mode Data] [mode Change]" << std::endl;
  std::cout << "testNCompressWithDiff:\t tncd [#of loops] [datalength] [mode Data] [mode Change]" << std::endl;
  std::cout << "testClientObjectMapping: tcom [#clients]" << std::endl;
  std::cout << "testClientInformation:\t tci [#clients] (call with >10)" << std::endl;
  std::cout << "testPacketBuffer:\t tbuf (comment address assignements in PacketBuffer.cc!)" << std::endl;
  while ( dec.compare("q") != 0 ) {
    std::cin >> dec;
    if ( dec.compare("tc") == 0 ) {
      std::cin >> a; std::cin >> b;
      testCompression( a, b );
    }
    else if ( dec.compare("td") == 0 ) {
      std::cin>> a; std::cin >> b; std::cin >> c;
      testDifferentiation( a, b, c );
    }
    else if ( dec.compare("tcd") == 0 ) {
      std::cin>> a; std::cin >> b; std::cin >> c;
      testCompressWithDiff( a, b, c );
    }
    else if ( dec.compare("modes") == 0 )
      displayModes();
    else if ( dec.compare("tcom") == 0 ) {
      std::cin>> a;
      testClientObjectMapping( a );
    }
    else if ( dec.compare("tci") == 0 ) {
      std::cin >> a;
      testClientInformation( a );
    } 
    else if ( dec.compare("tbuf") == 0 ) {
      testPacketBuffer();
    } 
    else if ( dec.compare("tncd") == 0 ) {
      std::cin >> n; std::cin >> a; std::cin >> b; std::cin >> c;
      for ( int i=1; i<=n; i++ ) {  
        std::cout << i << " s" << a << " ";      
        //std::cout << "start loop test " << i << std::endl;
        if ( !testNCompressWithDiff( i, a, b, c ) ) {
          std::cout << "#COMPARSION ERROR->VERYVERY BAD" << std::endl;
          break;
        }
      }
      std::cout << "end loop comparsion test" << std::endl;
    }
    else if ( dec.compare("tnc") == 0 ) {
      std::cin >> n; std::cin >> a; std::cin >> b;
      for ( int i=1; i<=n; i++ ) {  
        std::cout << i << " s" << a << " ";      
        //std::cout << "start loop test " << i << std::endl;
        if ( !testNCompression( i, a, b ) ) {
          std::cout << "#COMPARSION ERROR->VERYVERY BAD" << std::endl;
          break;
        }
      }
      std::cout << "end loop comparsion test" << std::endl;
    }
    else std::cout << "invalid input" << std::endl;  
    std::cout << "################## END ONE TURN ##################@" << std::endl;
  }
  return 0;
}*/

int main() {
  std::cout << "############### START TEST (quit q) ###############" << std::endl;
  testSynchronisable();
}

//#########################################################################
//### the prev loop is only to chose one prev prev test functions       ###
//#########################################################################
