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
 *      Oliver Scheuss (original)
 *
 */

#include "WANDiscovery.h"

#include <enet/enet.h>
#include <cstring>

#include "core/CoreIncludes.h"


namespace orxonox
{
  WANDiscovery::WANDiscovery()
  {
    /* debugging output */
    orxout(verbose, context::master_server) << "Creating WANDiscovery." << endl;
  
    /* register object in orxonox */
    RegisterObject(WANDiscovery);

    /* check for the masterserver address option in orxonox.ini */
    this->setConfigValues();

    /* initialize it and see if it worked */
    if( msc.initialize() )
      orxout(internal_error, context::master_server) << "Could not initialize master server communications!" << endl;

    /* connect and see if it worked */
    if( msc.connect( this->msaddress.c_str(), ORX_MSERVER_PORT ) )
      orxout(internal_error, context::master_server) << "Could not connect to master server at " 
        << this->msaddress << endl;

    /* debugging output */
    orxout(verbose, context::master_server) << "Initialization of WANDiscovery complete." << endl;
  }

  void WANDiscovery::setConfigValues()
  {
    /* update msaddress string from orxonox.ini config file, if it 
     * has changed. 
     */
    SetConfigValue( msaddress, "master.orxonox.net");
  } 

  WANDiscovery::~WANDiscovery()
  { 
    /* clear server list */
    this->servers_.clear();  
  }

  /* callback for the network reply poller */
  int WANDiscovery::rhandler( char *addr, ENetEvent *ev )
  { 
    /* error recognition */
    if( !ev || !ev->packet || !ev->packet->data )
    { orxout(internal_warning, context::master_server) << "Bad arguments received in WANDiscovery's reply handler." << endl;
      return 0;
    }

    /* handle incoming data */
    /* if a list entry arrives add to list */
    if( !strncmp( (char*)ev->packet->data, MSPROTO_SERVERLIST_ITEM,
      MSPROTO_SERVERLIST_ITEM_LEN ) )
    { 
      /* create server structure from that item */
      packet::ServerInformation toadd;

      /* fill in data, -1 for the index: index should be length -1 */
      toadd.setServerName( std::string((char*)ev->packet->data + 
        MSPROTO_SERVERLIST_ITEM_LEN+1) );
      toadd.setServerIP( std::string((char*)ev->packet->data + 
        MSPROTO_SERVERLIST_ITEM_LEN+1) );

      /* add to list */
      this->servers_.push_back( toadd );
    }
    else if( !strncmp( (char*)ev->packet->data, MSPROTO_SERVERLIST_END,
      MSPROTO_SERVERLIST_END_LEN ) )
    { 
      /* this is the only case where 2 should be returned,
       * as 1 is used to signal that we're done receiving
       * the list
       */
      return 2; 
    }

    /* done handling, return all ok code 0 */
    return 1;
  }
 
  void WANDiscovery::discover()
  {
    /* clear list */
    this->servers_.clear();

    /* send request to server */
    this->msc.sendRequest( MSPROTO_CLIENT " " MSPROTO_REQ_LIST );

    /* poll for replies */
    int i = WANDISC_MAXTRIES;
    while( i > 0 )
    {
      /* poll for reply and act according to what was received */
      switch( this->msc.pollForReply( this, 500 ) )
      { case 0: /* no event occured, decrease timeout */
          --i; break;
        case 1: /* got a list element, continue */ 
          break;
        case 2: /* done. */
          i = 0; break;
      }
    }
  }

  std::string WANDiscovery::getServerListItemName(unsigned int index)
  {
    /* if the index is out of range, return empty */
    if( index >= this->servers_.size() )
      return BLANKSTRING;
    else
      /* else return the name of the server */
      return this->servers_[index].getServerName();
  }

  std::string WANDiscovery::getServerListItemIP(unsigned int index)
  {
    /* if the index is out of range, return empty */
    if( index >= this->servers_.size() )
      return BLANKSTRING;
    else
      /* else return the IP of the server */
      return this->servers_[index].getServerIP();
  }


} // namespace orxonox
