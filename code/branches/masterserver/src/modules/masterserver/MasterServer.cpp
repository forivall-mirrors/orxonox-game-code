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

#include "MasterServer.h"
#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "core/CorePrereqs.h"

namespace orxonox 
{
  /* singleton stuff */
  ManageScopedSingleton( MasterServer, ScopeID::Root, false );

  /***** EVENTS *****/
  /* connect event */
  int 
  eventConnect( ENetEvent *event )
  { /* check for bad parameters */
    if( !event )
    { fprintf( stderr, "No event given.\n" );
      return -1;
    }

    /* output debug info */
    printf( "A new client connected from %x:%u.\n", 
        (event->peer->address.host),
        event->peer->address.port );

    /* game server or client connection? */
    /* -> decide in protocol */
    /* game server */
    /* add to game server list */
    /*  */

    /* client */
    /* add to client list */

    /* NOTE this seems to be some weird snipped from the tutorial as peer.data
     * is a uint_32 of some kind and hence shouldn't be assigned a c string? :S 
     */
    /* Store any relevant client information here. */
    /* event->peer->data = "Client information"; */
    /* /NOTE */
    return 0;
  }

  /* disconnect event */
  int 
  eventDisconnect( ENetEvent *event )
  { /* check for bad parameters */
    if( !event )
    { fprintf( stderr, "No event given.\n" );
      return -1;
    }

    /* output that the disconnect happened, to be removed at a later time. */
    printf( "%s disconnected.\n", (char*)event->peer->data );

    /* remove the server from the list it belongs to */

    /* Reset the peer's client information. */
    event->peer->data = NULL;
    return 0;
  }

  /* data event */
  int 
  eventData( ENetEvent *event )
  { /* output what's in the packet (to be removed later) */
    if( !event || !(event->packet) || !(event->peer) || !(event->channelID) )
    { fprintf( stderr, "No complete event given.\n" );
      return -1;
    }

    /* output debug info about the data that has come, to be removed */
    //printf( "A packet of length %u containing %s was received from %s on channel %u.\n",
    //event->packet->dataLength,
    //event->packet->data,
    //event->peer->data,
    //event->channelID );

    /* game server or client connection? */
    /* game server */
    /* parse data */
    /* start actions */
    /* and send reply */
    /* client */
    /* parse data */
    /* start actions */
    /* and send reply */

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy( event->packet );
    return 0;
  }

  /**** MAIN ROUTINE *****/
  int 
  MasterServer::run()
  {
    COUT(0) << "omg, i got baschtl'd!\n";

    /***** ENTER MAIN LOOP *****/
    ENetEvent *event = (ENetEvent *)calloc(sizeof(ENetEvent), sizeof(char));
    if( event == NULL )
    { fprintf( stderr, "Could not create ENetEvent structure, exiting.\n" );
      exit( EXIT_FAILURE );
    }

    /* create an iterator for the loop */
    while( enet_host_service( this->server, event, 1000 ) > 0 )
    { /* check what type of event it is and react accordingly */
      switch (event->type)
      { /* new connection */
        case ENET_EVENT_TYPE_CONNECT: 
          eventConnect( event ); break;

          /* disconnect */
        case ENET_EVENT_TYPE_DISCONNECT: 
          eventDisconnect( event ); break;

          /* incoming data */
        case ENET_EVENT_TYPE_RECEIVE: eventData( event ); break;
        default: break;
      }
    }

    /* free the event */
    if( event ) free( event );

    /* done */
    return 0;
  } 

  /* constructor */
  MasterServer::MasterServer()
  {
    /***** INITIALIZE NETWORKING *****/
    if( enet_initialize () != 0)
    { fprintf( stderr, "An error occurred while initializing ENet.\n");
      exit( EXIT_FAILURE );
    }

    /* register deinitialization */
    atexit( enet_deinitialize );

    /* Bind the server to the default localhost and port ORX_MSERVER_PORT */
    this->address.host = ENET_HOST_ANY;
    this->address.port = ORX_MSERVER_PORT;

    /* create a host with the above settings (the last two 0 mean: accept 
     * any input/output bandwidth */
    this->server = enet_host_create( &this->address, ORX_MSERVER_MAXCONNS, 
        ORX_MSERVER_MAXCHANS, 0, 0 );     

    /* see if creation worked */
    if( !this->server )
    { fprintf( stderr, 
        "An error occurred while trying to create an ENet server host.\n" );
    exit( EXIT_FAILURE );
    }

    /***** INITIALIZE GAME SERVER LIST *****/
    this->mainlist = new ServerList();
    if( this->mainlist == NULL )
    { fprintf( stderr, "Error creating server list.\n" );
      exit( EXIT_FAILURE );
    }

    /***** INITIALIZE PEER LIST *****/
    this->peers = new PeerList();

    /* run the main method */
    run();
  }

  /* destructor */
  MasterServer::~MasterServer()
  {
    /***** CLEANUP PROCESS *****/
    /* terminate all networking connections */
    enet_host_destroy( this->server );

    /* clear the list of connected game servers */
    /* clear the list of connected game clients */

    /* free all used memory */

  }

/* end of namespace */
}