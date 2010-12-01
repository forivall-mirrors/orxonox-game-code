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
 *      Fabian 'x3n' Landau (original)
 *   Co-authors:
 *      Sandro 'smerkli' Merkli (adaptions to WAN)
 *      ...
 *
 */

#include "WANDiscovery.h"

#include <enet/enet.h>
#include <cstring>

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"


namespace orxonox
{
  ManageScopedSingleton(WANDiscovery, ScopeID::Root, true);

  WANDiscovery::WANDiscovery()
  {
    COUT(4) << "Creating WANDiscovery.\n";

    /* initialize it and see if it worked */
    if( msc.initialize() )
      COUT(2) << "Error: could not initialize master server communications!\n";

    /* connect and see if it worked */
    if( msc.connect( MS_ADDRESS, 1234 ) )
      COUT(2) << "Error: could not connect to master server at " 
        << MS_ADDRESS << std::endl;

    COUT(4) << "Initialization of WANDiscovery complete.\n";
  }

  WANDiscovery::~WANDiscovery()
  { 
    /* clear server list */
    this->servers_.clear();  
  }

  /* callback for the network reply poller */
  int rhandler( char *addr, ENetEvent *ev )
  { 
    /* error recognition */
    if( !ev || !ev->packet || !ev->packet->data )
    { COUT(2) << "Bad arguments received in WANDiscovery's reply handler.\n";
      return 0;
    }

    /* handle incoming data */
    /* if a list entry arrives add to list */
    if( !strncmp( (char*)ev->packet->data, MSPROTO_SERVERLIST_ITEM,
      MSPROTO_SERVERLIST_ITEM_LEN ) )
    { 
      /* create server structure from that item */
      packet::ServerInformation toadd;

      /* fill in data */
      toadd.setServerName( std::string((char*)ev->packet->data + 
        MSPROTO_SERVERLIST_ITEM_LEN) );
      toadd.setServerIP( std::string((char*)ev->packet->data + 
        MSPROTO_SERVERLIST_ITEM_LEN) );

      /* add to list */
      WANDiscovery::getInstance().servers_.push_back( toadd );
    }
    else if( !strncmp( (char*)ev->packet->data, MSPROTO_SERVERLIST_END,
      MSPROTO_SERVERLIST_END_LEN ) )
    { 
      /* this is the only case where 1 should be returned,
       * as 1 is used to signal that we're done receiving
       * the list
       */
      return 1; 
    }

    /* done handling, return all ok code 0 */
    return 0;
  }
 
  void WANDiscovery::discover()
  {
    /* clear list */
    this->servers_.clear();

    /* send request to server */
    this->msc.sendRequest( MSPROTO_CLIENT " " MSPROTO_REQ_LIST );

    /* poll for replies */
    /* TODO add some timeout here so we don't wait indefinitely */
    while( !((this->msc).pollForReply( rhandler )) )
      /* nothing */;

    /* done receiving. */
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
