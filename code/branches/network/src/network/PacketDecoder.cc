/*
 * Class contains functions to determine and decode incomming packages
 * ->don't read this without the class PacketGenerator, since they belong together
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
	cout << "clientId: " << client << endl; //control cout, not important, just debugging info
	int id = (int)*packet->data; //the first 4 bytes are always the enet packet id
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
	case GAMESTATE:
		gstate( packet );
		return true;
		break;
	}
	return false;
}

//following are the decode functions for the data of the packets

void PacketDecoder::acknowledgement( ENetPacket* packet )
{
	ack* a = new ack;
	*a = *(ack*)packet->data; //press pattern of ack on new data
	printAck( a ); //debug info
}

void PacketDecoder::mousem( ENetPacket* packet )
{
	mouse* mouseMove = new mouse;
	//copy data of packet->data to new struct
	*mouseMove = *(mouse*)packet->data; 
	printMouse( mouseMove ); //debug info
}

void PacketDecoder::keystrike( ENetPacket* packet )
{
	keyboard* key = new keyboard;
	*key = *(keyboard*)packet->data; //see above
	printKey( key ); //debug info
}

void PacketDecoder::chatMessage( ENetPacket* packet )
{
	chat* chatting = new chat;
	chatting->id = (int)*packet->data; //first copy id into new struct
	//since the chat message is a char*, allocate the memory needed
	char* reserve = new char[packet->dataLength-4]; 
	//copy the transmitted bytestream into the new generated char*,
	//note the lenght of the message is represented as "packet->dataLength-sizeof( int )" 
	memcpy( &reserve[0], packet->data+sizeof(int), packet->dataLength-sizeof(int) );
	//put pointer of chatting struct to the begining of the new generated char*
	chatting->message = reserve;
	printChat( chatting ); //debug info
}

void PacketDecoder::gstate( ENetPacket* packet )
{
	GameState* currentState = new GameState;
	//since it's not alowed to use void* for pointer arithmetic
	unsigned char* data = (unsigned char*)packet->data;
	//copy the gamestate id into the struct, which is located at second place data+sizeof( int )
	memcpy( (void*)&(currentState->id), (const void*)(data+sizeof( int )), sizeof( int ) );
	//copy the size of the gamestate data into the new gamestate struct, located at 3th
	//position of the data stream, data+2*sizeof( int )
	memcpy( (void*)&(currentState->size), (const void*)(data+2*sizeof( int )), sizeof( int) );
	//since data is not allocated, because it's just a pointer, allocate it with size of gamestatedatastream
	currentState->data = (unsigned char*)(malloc( currentState->size ));
	//copy the gamestate data
	memcpy( (void*)(currentState->data), (const void*)(data+3*sizeof( int )), currentState->size );
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

void PacketDecoder::printGamestate( GameState* data )
{
	cout << "id of gamestate:   " << data->id << endl;
	cout << "size of gamestate: " << data->size << endl;
}
