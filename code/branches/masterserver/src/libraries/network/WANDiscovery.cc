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
    /* create master server communications object */
    this->msc = MasterServerComm();

    /* initialize it and see if it worked */
    if( msc.initialize() )
      COUT(1) << "Error: could not initialize master server communications!\n";

    /* connect and see if it worked */
    if( msc.connect( MS_ADDRESS, 1234 ) )
      COUT(1) << "Error: could not connect to master server!\n";
  }

  WANDiscovery::~WANDiscovery()
  { 
    /* clear server list */
    this->servers_.clear();  
  }

  /* callback for the network reply poller */
  /* NOTE implement protocol-specific part here. */
  int rhandler( char *addr, ENetEvent *ev )
  { 
    /* handle incoming data */
    /* if a list entry arrives add to list */
    if( !strncmp( ev->packet->data, MSPROTO_SERVERLIST_ITEM,
      MSPROTO_SERVERLIST_ITEM_LEN ) )
    { 
      /* create server structure from that item */
      ServerInformation toadd;

      /* fill in data */
      toadd->setName( std::string(ev->packet->data + 
        MSPROTO_SERVERLIST_ITEM_LEN) );
      toadd->setIP( std::string(ev->packet->data + 
        MSPROTO_SERVERLIST_ITEM_LEN) );

      /* add to list */
      this->servers_.add( toadd );
    }
    else if( !strncmp( ev->packet->data, MSPROTO_SERVERLIST_END,
      MSPROTO_SERVERLIST_END_LEN ) )
    { return 1; }

    /* done handling, return all ok code 0 */
    return 0;
  }
 
  void WANDiscovery::discover()
  {
    /* clear list */
    this->servers_.clear();

    /* send request to server */
    msc.sendRequest( MSPROTO_CLIENT " " MSPROTO_REQ_LIST );

    /* deal with replies */
    while( !msc.pollForReply( rhandler ) ) 
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
