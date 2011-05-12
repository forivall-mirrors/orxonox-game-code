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

#include "ServerList.h"

namespace orxonox
{ 
  ServerList::ServerList()
  { /* create a new list */ }

  ServerList::~ServerList()
  { /* delete the list */
    serverlist.clear();
  }

  int 
  ServerList::addServer( packet::ServerInformation toadd )
  { this->serverlist.push_back( toadd ); 
    return 0;
  }

  bool 
  ServerList::delServerByName( std::string name )
  { 
    /* get an iterator */
    std::list<packet::ServerInformation>::iterator i;

    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i ) 
      if( (*i).getServerName() == name )
      { /* found this name, remove and quit */
        this->serverlist.erase( i );
        return true;
      }
    return false;
  }

  bool ServerList::delServerByAddress( std::string address )
  { 
    /* get an iterator */
    std::list<packet::ServerInformation>::iterator i;

    /* loop through list elements */
    for( i=serverlist.begin(); i != serverlist.end(); ++i ) 
      if( (*i).getServerIP() == address )
      { /* found this name, remove and quit */
        this->serverlist.erase( i );
        return true;
      }
    return false;
  }


  /* sort by name */
  bool sub_compare_names( packet::ServerInformation no1, 
    packet::ServerInformation no2 )
  { return no1.getServerName() > no2.getServerName(); }

  void ServerList::sortByName()
  { 
    this->serverlist.sort( sub_compare_names ); 
  }
  
  /* sort by ping */
  bool sub_compare_pings( packet::ServerInformation no1, 
    packet::ServerInformation no2 )
  { 
    /* TODO */
    return no1.getServerName() > no2.getServerName();
  }

  void ServerList::sortByPing()
  {
    this->serverlist.sort( sub_compare_pings ); 
  }

}
