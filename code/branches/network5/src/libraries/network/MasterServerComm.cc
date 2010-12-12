/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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
 *      Sandro 'smerkli' Merkli
 *   Co-authors:
 *      ...
 *
 */

#include "MasterServerComm.h"

namespace orxonox
{
 
  MasterServerComm::MasterServerComm()
  { /* nothing anymore, everything's been outsourced to 
     * the initialize method to facilitate debugging
     */
  } 

  int MasterServerComm::initialize()
  {
    /* initialize Enet */
    if (enet_initialize () != 0)
    { COUT(1) << "An error occurred while initializing ENet.\n";
      return 1;
    }

    /* initialize the event holder */
    this->event = (ENetEvent *)calloc( sizeof(ENetEvent), 1 );
    
    /* install atexit handler for enet */
    atexit( enet_deinitialize );

    /* initiate the client */
    this->client = enet_host_create( NULL /* create a client host */,
        1,
        2, /* allow up 2 channels to be used, 0 and 1 */
        0, 
        0 ); 

    /* see if it worked */
    if (this->client == NULL)
    { COUT(1) << "An error occurred while trying to create an ENet client host.\n";
      return 1;
    }

    return 0;
  }

  MasterServerComm::~MasterServerComm()
  {
    /* destroy the enet facilities */
    enet_host_destroy(this->client);
  }

  int MasterServerComm::connect( const char *address, unsigned int port )
  {
    /* Connect to address:port. */
    enet_address_set_host( &this->address, address );
    this->address.port = port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    this->peer = enet_host_connect(this->client, &this->address, 2, 0);    

    if (this->peer == NULL )
    { COUT(2) << "ERROR: No available peers for initiating an ENet connection.\n";
      return -1;
    }

    /* Wait up to 2 seconds for the connection attempt to succeed. */
    if (enet_host_service (this->client, this->event, 2000) > 0 &&
        this->event->type == ENET_EVENT_TYPE_CONNECT )
      COUT(3) << "Connection to master server succeeded.\n";
    else
    {
      enet_peer_reset (this->peer);
      fprintf( stdout, "Connection to %s failed.", address );
      COUT(2) << "ERROR: connection to " << address << " failed.\n";
      return -1;
    }

    /* all fine */
    return 0;
  }

  /* NOTE this is to be reimplemented soon to return 
   * a structure containing 
   * - addrconv
   * - the event
   * so we can also make callbacks from objects
   */
  int MasterServerComm::pollForReply( int (*callback)( char*, ENetEvent* ),
    int delayms )
  { 
    /* see whether anything happened */
    /* WORK MARK REMOVE THIS OUTPUT */
    COUT(2) << "polling masterserver...\n";

    /* address buffer */
    char *addrconv = NULL;
    int retval = 0;

    /* enet_host_service returns 0 if no event occured */
    /* just newly set below test to >0 from >= 0, to be tested */
    if( enet_host_service( this->client, this->event, delayms ) > 0 )
    { 
      /* check what type of event it is and react accordingly */
      switch (this->event->type)
      { /* new connection, not supposed to happen. */
        case ENET_EVENT_TYPE_CONNECT: break;

        /* disconnect */
        case ENET_EVENT_TYPE_DISCONNECT: /* ?? */ break;

        /* incoming data */
        case ENET_EVENT_TYPE_RECEIVE: 
          addrconv = (char *) calloc( 50, 1 );
          if( !addrconv ) 
          { COUT(2) << "MasterServerComm.cc: Could not allocate memory!\n";
            break;
          }

          /* resolve IP */
          enet_address_get_host_ip( &(this->event->peer->address), 
            addrconv, 49 );

          /* DEBUG */
          COUT(3) << "MasterServer Debug: A packet of length " 
            << this->event->packet->dataLength 
            << " containing " << this->event->packet->data
            << " was received from " << addrconv 
            << " on channel " << this->event->channelID;
          /* END DEBUG */

          /* call the supplied callback, if any. */
          if( (*callback) != NULL )
            retval = (*callback)( addrconv, (this->event) );

          /* clean up */
          enet_packet_destroy( event->packet );
          if( addrconv ) 
            free( addrconv );

          break;
        default: break;
      }

      /* event handled, return 0 */
      return retval;
    }

    /* show that no event occured */
    return 0;
  }

  int MasterServerComm::sendRequest( const char *data )
  {
    /* send the data to the friend */
    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket * packet = enet_packet_create( data, 
        strlen( data ) + 1, 
        ENET_PACKET_FLAG_RELIABLE);

    /* Send the packet to the peer over channel id 0. */
    enet_peer_send (this->peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    enet_host_flush( this->client );
   
    /* free the packet */
    enet_packet_destroy( packet );

    /* all done. */
    return 0;
  }

  int MasterServerComm::sendRequest( std::string data )
  {
    /* send the data to the friend */
    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket * packet = enet_packet_create( data.c_str(), 
        data.length() + 1, 
        ENET_PACKET_FLAG_RELIABLE);

    /* Send the packet to the peer over channel id 0. */
    enet_peer_send (this->peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    enet_host_flush( this->client );
    if( packet ) free( packet );

    /* all done. */
    return 0;
  }

}
