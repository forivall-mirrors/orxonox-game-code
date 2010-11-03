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
  MasterServer::eventConnect( ENetEvent *event )
  { /* check for bad parameters */
    if( !event )
    { COUT(2) << "MasterServer::eventConnect: No event given.\n" ;
      return -1;
    }

    /* convert address to string. */
    char *addrconv = (char *) calloc( 50, 1 );
    enet_address_get_host_ip( &(event->peer->address), addrconv, 49 );

    /* output debug info */
    COUT(4) << "A new client connected from " 
      << addrconv 
      << " on port " 
      << event->peer->address.port << "\n";

    /* game server or client connection? */
    /* -> decide in protocol */
    /* game server */
    /* add to game server list */
    /*  */

    /* client */
    /* add to client list */

    /* store string form of address here */
    event->peer->data = addrconv; 

    /* all fine. */
    return 0;
  }

  /* disconnect event */
  int 
  MasterServer::eventDisconnect( ENetEvent *event )
  { /* check for bad parameters */
    if( !event )
    { COUT(2) << "No event given.\n";
      return -1;
    }

    /* output that the disconnect happened, to be removed at a later time. */
    COUT(4) << (char*)event->peer->data << " disconnected.\n";

    /* remove the server from the list it belongs to */

    /* Reset the peer's client information. */
    if( event->peer->data ) free( event->peer->data );
    return 0;
  }

  /* data event */
  int 
  MasterServer::eventData( ENetEvent *event )
  { /* output what's in the packet (to be removed later) */
    if( !event || !(event->packet) || !(event->peer) )
    { COUT(2) << "No complete event given.\n";
      return -1;
    }
     
    /* generate address in readable form */
    char *addrconv = (char *) calloc( 50, 1 );
    enet_address_get_host_ip( &(event->peer->address), addrconv, 49 );

    /* DEBUG */
    /* output debug info about the data that has come, to be removed */
    COUT(4) << "A packet of length" 
      << event->packet->dataLength
      << " containing "
      << event->packet->data
      << " was received from "
      << addrconv 
      << " on channel "
      << event->channelID << "\n";

    /* send some packet back for testing */
    /* TESTING */

    /* Create a reliable reply of size 7 containing "reply\0" */
    ENetPacket * reply = enet_packet_create ("reply", 
        strlen ("reply") + 1, 
        ENET_PACKET_FLAG_RELIABLE);

    /* Send the reply to the peer over channel id 0. */
    enet_peer_send( event->peer, 0, reply );

    /* One could just use enet_host_service() instead. */
    enet_host_flush( this->server );

    /* /TESTING */

    /* game server or client connection? */
    /* game server */
    /* parse data */
    /* start actions */
    /* and send reply */
    /* client */
    /* parse data */
    /* start actions */
    /* and send reply */

    /* delete addrconv */
    if( addrconv ) free( addrconv );

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy( event->packet );
    return 0;
  }


  /**** MAIN ROUTINE *****/
  int 
  MasterServer::run()
  {
    /***** ENTER MAIN LOOP *****/
    ENetEvent *event = (ENetEvent *)calloc(sizeof(ENetEvent), sizeof(char));
    if( event == NULL )
    { 
      COUT(1) << "Could not create ENetEvent structure, exiting.\n";
      exit( EXIT_FAILURE );
    }

    /* tell people we're now initialized and blocking. */
    COUT(0) << "MasterServer initialized, waiting for connections.\n";

    /* create an iterator for the loop */
    while( enet_host_service( this->server, event, 1000 ) >= 0 )
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
    { COUT(1) << "An error occurred while initializing ENet.\n";
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
    { COUT(1) << 
        "An error occurred while trying to create an ENet server host.\n";
      exit( EXIT_FAILURE );
    }

    /***** INITIALIZE GAME SERVER AND PEER LISTS *****/
    this->mainlist = new ServerList();
    this->peers = new PeerList();
    if( this->mainlist == NULL || this->peers == NULL )
    { COUT(1) << "Error creating server or peer list.\n";
      exit( EXIT_FAILURE );
    }

    /* run the main method */
    run();
  }

  /* destructor */
  MasterServer::~MasterServer()
  {
    /***** CLEANUP PROCESS *****/
    /* terminate all networking connections */
    enet_host_destroy( this->server );

    /* free all used memory */
    /* clear the list of connected game servers */
    /* clear the list of connected game clients */

  }

/* end of namespace */
}
