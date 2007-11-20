/*
 * Class contains functions to determine and decode incomming packages
 * 
 * Autor: Dumeni Manatschal
 * 
*/


#include "enet/enet.h"
#include "PacketManager.h"
#include <iostream>

using namespace std;
using namespace network;

PacketDecoder::PacketDecoder(){}

//call this function out of an instance of PacketDecoder
//it will determine the type id and call the right decode function
bool PacketDecoder::elaborate( ENetPacket* packet, int clientId )
{
	int client = clientId;
	cout << "clientId: " << client << endl;
	int id = (int)*packet->data;
	switch( id ) {
	case ACK:
		acknowledgement( packet );
		return true;
		break;
	case MOUSE:
		mousem( packet );
		return true;
		break;
	case KEYBOARD:
		keystrike( packet );
		return true;
		break;
	case CHAT:
		chatMessage( packet );
		return true;
		break;
	}
	return false;
}

//following are the decode functions for the data of the packets

void PacketDecoder::acknowledgement( ENetPacket* packet )
{
	ack* a = new ack;
	*a = *(ack*)packet->data;
	printAck( a );
}

void PacketDecoder::mousem( ENetPacket* packet )
{
	mouse* mouseMove = new mouse;
	*mouseMove = *(mouse*)packet->data;
	printMouse( mouseMove );
}

void PacketDecoder::keystrike( ENetPacket* packet )
{
	keyboard* key = new keyboard;
	*key = *(keyboard*)packet->data;
	printKey( key );
}

void PacketDecoder::chatMessage( ENetPacket* packet )
{
	chat* chatting = new chat;
	chatting->id = (int)*packet->data;
	char* reserve = new char[packet->dataLength-4];
	memcpy( &reserve[0], packet->data+sizeof(int), packet->dataLength-4 );
	chatting->message = reserve;
	printChat( chatting );
}

//these are some print functions for test stuff

void PacketDecoder::printAck( ack* data )
{
	cout << "data id: " << data->id << endl;
	cout << "data:    " << data->a << endl;
}

void PacketDecoder::printMouse( mouse* data )
{
	cout << "data id: " << data->id << endl;
	cout << "data:    " << data->x << " " << data->y << endl;
}

void PacketDecoder::printKey( keyboard* data )
{
	cout << "data id: " << data->id << endl;
	cout << "data:    " << (char)data->press << endl;
}

void PacketDecoder::printChat( chat* data )
{
	cout << "data id: " << data->id << endl;
	cout << "data:    " << data->message << endl;
}
