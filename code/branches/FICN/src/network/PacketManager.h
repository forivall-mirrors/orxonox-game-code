#ifndef PACKETMANAGER_H_
#define PACKETMANAGER_H_

#include <enet/enet.h>
#include "GameStateManager.h"

//enum netowk generaly used to set the type ID of a packet
namespace network
{
enum packet_id {
	ACK,
	MOUSE,
	KEYBOARD,
	CHAT,
	GAMESTATE
};

/*
 * class to generate packets
 * 
 * @autor: Dumeni Manatschal
 * 
*/ 
class PacketGenerator
{
public:
	PacketGenerator();
	//call one of this functions out of an instance of PacketGenerator to create a packet
	ENetPacket* acknowledgement( int state, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* mousem( double x, double y, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* keystrike( char press, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* chatMessage( const char* message, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* gstate( GameState* states, int reliable = ENET_PACKET_FLAG_RELIABLE );
private:
	//used to set the bytes in the right order
	struct ack {
		int id;
		int a;
	};

	struct mouse {
		int id;
		double x;
		double y;
	};

	struct keyboard {
		int id;
		char press;
	};	
};

/*
 * class used to decode incoming packets
 * 
 * @autor: Dumeni Manatschal
 * 
*/
class PacketDecoder
{
public:
	PacketDecoder();
	//call this function to decode, it calls the right decoding function below
	bool elaborate( ENetPacket* packet, int clientId );
private:
	struct ack {
		int id;
		int a;
	};

	struct mouse {
		int id;
		double x;
		double y;
	};

	struct keyboard {
		int id;
		char press;
	};
	//only in this class, not PacketGenerator, used as pattern to put incoming
	//bytes inside
	struct chat {
		int id;
		const char* message;
	};
	
	void acknowledgement( ENetPacket* packet );
	void mousem( ENetPacket* packet );
	void keystrike( ENetPacket* packet );
	void chatMessage( ENetPacket* packet );
	void gstate( ENetPacket* packet );
	
  //process data
  //two functions are note yet implemented!
  //virtual void processGamestate(GameState *state);
  virtual void processChat( chat *data);
  //virtual void processAck( ack *data);
  
	//print functions
	void printAck( ack* data );
	void printMouse( mouse* data );
	void printKey( keyboard* data );
	void printChat( chat* data );
	void printGamestate( GameState* data );
};
}

#endif /*PACKETMANAGER_H_*/
