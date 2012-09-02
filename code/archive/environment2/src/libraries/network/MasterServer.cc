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
  /* helpers */
  static void 
  helper_output_debug( ENetEvent *event, char *addrconv )
  {
    COUT(4) << "A packet of length" 
      << event->packet->dataLength
      << " containing "
      << (const char*)event->packet->data
      << " was received from "
      << addrconv 
      << " on channel "
      << event->channelID << "\n";
  }

  void
  MasterServer::helper_sendlist( ENetEvent *event )
  {
    /* get an iterator */
    std::list<packet::ServerInformation>::iterator i;

    /* packet holder */
    ENetPacket *reply;

    /* loop through list elements */
    for( i = mainlist.serverlist.begin(); i 
        != mainlist.serverlist.end(); ++i ) 
    {
      /* send this particular server */
      /* build reply string */
      char *tosend = (char *)calloc( (*i).getServerIP().length() 
          + MSPROTO_SERVERLIST_ITEM_LEN + 2,1 );
      if( !tosend ) 
      { COUT(2) << "Masterserver.cc: Memory allocation failed.\n";
        continue;
      } 
      sprintf( tosend, "%s %s", MSPROTO_SERVERLIST_ITEM, 
          (*i).getServerIP().c_str() );

      /* create packet from it */
      reply = enet_packet_create( tosend,
          strlen( tosend ) + 1, 
          ENET_PACKET_FLAG_RELIABLE);

      /* Send the reply to the peer over channel id 0. */
      enet_peer_send( event->peer, 0, reply );

      /* One could just use enet_host_service() instead. */
      enet_host_flush( this->server );

      /* free the tosend buffer */
      free( tosend );
    } 

    /* create end-of-list packet */
    reply = enet_packet_create( MSPROTO_SERVERLIST_END,
        MSPROTO_SERVERLIST_END_LEN + 1,
        ENET_PACKET_FLAG_RELIABLE );

    /* send end-of-list packet */
    enet_peer_send( event->peer, 0, reply );

    /* One could just use enet_host_service() instead. */
    enet_host_flush( this->server );
  }




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

    /* output that the disconnect happened */
    COUT(4) << (char*)event->peer->data << " disconnected.\n";

    /* create string from peer data */
    std::string name = std::string( (char*)event->peer->data );

    /* remove the server from the list it belongs to */
    this->mainlist.delServerByName( name );

    /* Reset the peer's client information. */
    if( event->peer->data ) free( event->peer->data );

    /* done */
    return 0;
  }

  /* data event */
  int 
  MasterServer::eventData( ENetEvent *event )
  { /* validate packet */
    if( !event || !(event->packet) || !(event->peer) )
    { COUT(2) << "No complete event given.\n";
      return -1;
    }
     
    /* generate address in readable form */
    char *addrconv = (char *) calloc( 50, 1 );
    enet_address_get_host_ip( &(event->peer->address), addrconv, 49 );

    /* output debug info about the data that has come */
    helper_output_debug( event, addrconv );

    /* GAME SERVER OR CLIENT CONNECTION? */
    if( !strncmp( (char *)event->packet->data, MSPROTO_GAME_SERVER, 
      MSPROTO_GAME_SERVER_LEN ) )
    { /* Game server */

      if( !strncmp( (char *)event->packet->data 
        + MSPROTO_GAME_SERVER_LEN+1, 
        MSPROTO_REGISTER_SERVER, MSPROTO_REGISTER_SERVER_LEN ) )
      { /* register new server */
        mainlist.addServer( packet::ServerInformation( event ) );
        
        /* tell people we did so */
        COUT(2) << "Added new server to list: " << 
          packet::ServerInformation( event ).getServerIP() << "\n";
      }

      else if( !strncmp( (char *)event->packet->data
        + MSPROTO_GAME_SERVER_LEN+1,
        MSPROTO_SERVERDC, MSPROTO_SERVERDC_LEN ) )
      {
        /* create string from peer data */
        std::string name = std::string( addrconv );

        /* remove the server from the list it belongs to */
        this->mainlist.delServerByAddress( name );

        /* tell the user */
        COUT(2) << "Removed server " << name << " from list.\n";
      }

      /* TODO add hook for disconnect here */
    }
    else if( !strncmp( (char *)event->packet->data, MSPROTO_CLIENT, 
      MSPROTO_CLIENT_LEN) )
    { /* client */
      if( !strncmp( (char *)event->packet->data + MSPROTO_CLIENT_LEN+1,
        MSPROTO_REQ_LIST, MSPROTO_REQ_LIST_LEN ) )
        /* send server list */
        helper_sendlist( event );
    }
    else
    { /* bad message, don't do anything. */ } 

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

    /* TODO schedule pings for servers somewhere here */
    
    /* create an iterator for the loop */
    enet_host_service( this->server, event, 100 );

    /* check what type of event it is and react accordingly */
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

    /* set the quit flag to false */
    this->quit = false;

    /* Bind the server to the default localhost and port ORX_MSERVER_PORT */
    this->address.host = ENET_HOST_ANY;
    this->address.port = ORX_MSERVER_PORT;

    /* create a host with the above settings (the last two 0 mean: accept 
     * any input/output bandwidth */
    this->server = enet_host_create( &this->address, ORX_MSERVER_MAXCONNS, 
        ORX_MSERVER_MAXCHANS, 0, 0 );
    assert(this->server);

    /* see if creation worked */
    if( !this->server )
    { COUT(1) << 
        "An error occurred while trying to create an ENet server host.\n";
      exit( EXIT_FAILURE );
    }

    /***** INITIALIZE GAME SERVER AND PEER LISTS *****/
    this->peers = new PeerList();

    /* tell people we're now initialized */
    COUT(0) << "MasterServer initialized, waiting for connections.\n";
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
