#include "GameStateManager.h"
#include "Synchronisable.h"
#include "GameStateClient.h"
#include "NetworkPrereqs.h"
#include "PacketTypes.h"
#include "iostream"
#include "core/CoreIncludes.h"
#include "time.h"

using namespace network;

void printData( unsigned char* data, int length ) {
  for ( int i=0; i<length; i++ )
    std::cout << (int)data[i] << " ";
  std::cout << std::endl;
}

void printGameStateCompressed( GameStateCompressed* gc ) {
  std::cout << "=================================================" << std::endl;
  std::cout << "GameStateCompressed id:\t\t" << gc->id << std::endl;
  std::cout << "GameStateCompressed normsize:\t" << gc->normsize << std::endl;
  std::cout << "GameStateCompressed compsize:\t" << gc->compsize << std::endl;
  std::cout << "GameStateCompressed diffed:\t" << gc->diffed << std::endl;
  //std::cout << "GameState data:\t" << gc->data << std::endl;
  std::cout << "GameStateCompressed compressing rate:\t" << 100.0-((100.0/(gc->normsize))*(gc->compsize)) << "%" << std::endl;
  //std::cout << "=================================================" << std::endl;
  return;
}

bool compareData( GameState* g1, GameState* g2 ) {
  if ( g1->id != g2->id ) {
    std::cout << "GameStates are not comparable -> not same id" << std::endl;
    return 1;
  }
  else if ( g1->size != g2->size ) {
    std::cout << "GameStates are not the same size!!" << std::endl;
    std::cout << g1->size << " != " << g2->size << std::endl;
  }
  int length = g1->size;
  for ( int i=0; i<length; i++ ) {
    if ( g1->data[i] != g2->data[i] ) {
      std::cout << "data of both GameStates are not identical" << std::endl;
      return false;
    }
  }
  std::cout << "GameStates are identical" << std::endl;
  return true;
}

bool compareGameStates( GameState* g1, GameState* g2 ) {
  if ( g1->id != g2->id ) {
    std::cout << "GameState id's not identical" << std::endl;
    return false;
  }
  else if( g1->size != g2->size ) {
    std::cout << "GameState sizes are not identical" << std::endl;
    return false;
  }
  else if ( g1->diffed != g2->diffed ) {
    std::cout << "GameState diffed params not identical" << std::endl;
    return false;
  }
  else if ( !compareData( g1, g2 ) ) {
    std::cout << "GameState data are not identical" << std::endl;
    return false;
  }
  std::cout << "==>GameStates are identical (GameStateCompare)" << std::endl;
  return true;
}

void printGameState( GameState* gstate ) {
  std::cout << "=================================================" << std::endl;
  std::cout << "GameState id:\t\t" << gstate->id << std::endl;
  std::cout << "GameState size:\t\t" << gstate->size << std::endl;
  std::cout << "GameState diffed:\t" << gstate->diffed << std::endl;
  //std::cout << "GameState data:\t" << gstate->data << std::endl;
  //std::cout << "=================================================" << std::endl;
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
    for ( int i=0; i<length; i++ ) {
      data[i] = (char)(i%255);
    }
  }
  else if ( mode == 4 ) {
    for ( int i=0; i<length; i++ ){
      data[i] = (char)(rand()%255);
    }
  }
  else if ( mode == 5 ) {
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
    b->data = new unsigned char[a->size];
    b->size = a->size;
    for ( int i=0; i<length; i++ ) {
      if ( i%10 == 0 ) b->data[i] = rand()%255;
      else b->data[i] = a->data[i];
    }
  }
  else if ( mode == 2 ) {
    b->data = new unsigned char[a->size];
    b->size = a->size;
    for ( int i=0; i<length; i++ ) {
      if ( i%5 == 0 ) b->data[i] = rand()%255;
      else b->data[i] = a->data[i];
    }
  }
  else if ( mode == 3 ) {
    int s = a->size + (a->size)/3;
    b->data = new unsigned char[s];
    b->size = s;
    for ( int i=0; i<length; i++ ) {
      if ( i%10 == 0 ) b->data[i] = rand()%255;
      else b->data[i] = a->data[i];
    }
    for( int i=length; i<s; i++ ) {
      b->data[i] = rand()%255;
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
  GameStateManager* g_manager;;
  
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

  if( !compareData( g_undiff1, g_undiff2 ) ) std::cout << " BUT THAT'S HOW IT HAS TO BE" << std::endl;

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
  compareGameStates( g_resultDiffed, g_diffed );
 
  g_resultUndiffed = g_client->testUndiff( g_undiff1, g_resultDiffed );
  std::cout << "---New Gamestate of pseudo Server compared with new gamestate that Client gets" << std::endl;
  compareGameStates( g_resultUndiffed, g_undiff2 );

  return;  
}

int main( int argc, char* argv[] ) {
  int a,b,c;
  std::string dec = "nothing";
  std::cout << "############### START TEST (quit q) ###############" << std::endl;
  while ( dec.compare("q") != 0 ) {
    std::cout << "possible tests: testcompression [datalength] [mode]" << std::endl;
    std::cout << "testdiff [datalength] [mode Data] [mode Change]" << std::endl;
    std::cout << "tcd [datalength] [mode Data] [mode Change]" << std::endl;
    std::cin >> dec;
    if ( dec.compare("testcompression") == 0 ) {
      std::cin >> a; std::cin >> b;
      testCompression( a, b );
    }
    else if ( dec.compare("testdiff") == 0 ) {
      std::cin>> a; std::cin >> b; std::cin >> c;
      testDifferentiation( a, b, c );
    }
    else if ( dec.compare("tcd") == 0 ) {
      std::cin>> a; std::cin >> b; std::cin >> c;
      testCompressWithDiff( a, b, c );
    }
  }
  return 0;
}
/**
int main() {
  std::cout << "############### START TEST (quit q) ###############" << std::endl;
  testCompressWithDiff( 5000, 5, 1 );
}*/
