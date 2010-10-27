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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <enet/enet.h>

int main( int argc, char *argv[] )
{
  /* initialize Enet */
  if (enet_initialize () != 0)
  { fprintf (stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }
  atexit (enet_deinitialize);

  /* client handle */
  ENetHost *client;

  /* initiate the client */
  client = enet_host_create( NULL /* create a client host */,
      1,
      2, /* allow up 2 channels to be used, 0 and 1 */
      0, 
      0 ); 

  /* see if it worked */
  if (client == NULL)
  { fprintf (stderr, 
      "An error occurred while trying to create an ENet client host.\n");
    exit (EXIT_FAILURE);
  }

  ENetAddress address;
  ENetEvent event;
  ENetPeer *peer;

  /* Connect to some.server.net:1234. */
  enet_address_set_host (& address, argv[1] );
  address.port = 1234;

  /* Initiate the connection, allocating the two channels 0 and 1. */
  peer = enet_host_connect(client, &address, 2, 0);    

  if (peer == NULL )
  { fprintf( stderr, 
        "No available peers for initiating an ENet connection.\n");
    exit (EXIT_FAILURE);
  }

  /* Wait up to 5 seconds for the connection attempt to succeed. */
  if (enet_host_service (client, & event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT)
  { fprintf( stdout, "Connection to some.server.net:1234 succeeded." ); 
    char *theinput = (char *)calloc( 100,1 );
    while( true )
    { 
      fgets( theinput, 90, stdin );
      if( !strncmp( theinput, "quit", 4 ) )
        break;

      /* send the data to the friend */
      /* Create a reliable packet of size 7 containing "packet\0" */
      ENetPacket * packet = enet_packet_create( theinput, 
          strlen( theinput ) + 1, 
          ENET_PACKET_FLAG_RELIABLE);

      /* Send the packet to the peer over channel id 0. */
      enet_peer_send (peer, 0, packet);

      /* One could just use enet_host_service() instead. */
      enet_host_flush( client );
      if( packet ) free( packet );
    }
  }
  else
  {
    /* Either the 5 seconds are up or a disconnect event was */
    /* received. Reset the peer in the event the 5 seconds   */
    /* had run out without any significant event.            */
    enet_peer_reset (peer);
    fprintf( stdout, "Connection to some.server.net:1234 failed." );
    exit(EXIT_FAILURE);
  }

  enet_host_destroy(client);
}
