#include "PacketManager.h"
#include "enet/enet.h"
#include <iostream>
#include <list>
#include <string>
#include <cstring>

using namespace std;
using namespace network;

PacketGenerator::PacketGenerator() {}

ENetPacket* PacketGenerator::acknowledgement( int state, int reliable )
{
	cout << "generating new acknowledgement" << endl;
	ack* ackreq = new ack;
	ackreq->id = ACK;
	ackreq->a = state;
	
	ENetPacket *packet = enet_packet_create( ackreq , sizeof( *ackreq ), reliable );
	
	return packet;
}

ENetPacket* PacketGenerator::mousem( double x, double y, int reliable )
{
	cout << "generating new mouse" << endl;
	mouse* mousemove = new mouse;
	mousemove->id = MOUSE;
	mousemove->x = x;
	mousemove->y = y;
	
	ENetPacket *packet = enet_packet_create( mousemove , sizeof( *mousemove ), reliable );
	
	return packet;
}

ENetPacket* PacketGenerator::keystrike( char press, int reliable )
{
	cout << "generating new keyboard" << endl;
	keyboard* key = new keyboard;
	key->id = KEYBOARD;
	key->press = press;
	
	ENetPacket *packet = enet_packet_create( key , sizeof( *key ), reliable );
	
	return packet;
}

ENetPacket* PacketGenerator::chatMessage( const char* message, int reliable )
{
	int* trans = new int[sizeof(int) + strlen(message) + 1];
	*trans = CHAT;
	memcpy( &trans[1], (const void*)message, strlen( message ) + 1);
	ENetPacket *packet = enet_packet_create( trans , sizeof( int ) + strlen( message ) + 1, reliable );
	
	return packet;
}

