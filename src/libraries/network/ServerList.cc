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
  ServerList::addServer( packet::ServerInformation toadd,
    ENetPeer *peer )
  {
    ServerListElem toAdd;
    toAdd.ServerInfo = toadd;
    toAdd.peer = peer;

    this->serverlist.push_back( toAdd );
    return 0;
  }

  bool
  ServerList::delServerByName( std::string name )
  {
    /* get an iterator */
    std::list<ServerListElem>::iterator i;

    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i )
      if( (*i).ServerInfo.getServerName() == name )
      { /* found this name, remove and quit */
        this->serverlist.erase( i );
        return true;
      }
    return false;
  }

  bool ServerList::delServerByAddress( std::string address )
  {
    /* get an iterator */
    std::list<ServerListElem>::iterator i;

    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i )
      if( (*i).ServerInfo.getServerIP() == address )
      { /* found this name, remove and quit */
        this->serverlist.erase( i );
        return true;
      }
    return false;
  }

  /* SEARCHING */
  ServerListSearchResult
  ServerList::findServerByAddress( std::string address )
  {
    /* get an iterator */
    std::list<ServerListElem>::iterator i;

    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i )
      if( (*i).ServerInfo.getServerIP() == address )
      { /* found the target, return it */
        ServerListSearchResult res = { (*i), true };
        return res;
      }

    /* no success */
    ServerListSearchResult res = { (*i), false };
    return res;
  }

  ServerListSearchResult
  ServerList::findServerByName( std::string name )
  {
    /* get an iterator */
    std::list<ServerListElem>::iterator i;

    /* iterate, return when name found */
    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i )
      if( (*i).ServerInfo.getServerName() == name )
      {
        ServerListSearchResult res = { (*i), true };
        return res;
      }

    /* no luck, return a struct that tells the caller so */
    ServerListSearchResult res = { (*i), false };
    return res;
  }

  /* SORTING */
  /* sort by name */
  bool sub_compare_names( ServerListElem no1,
    ServerListElem no2 )
  { return no1.ServerInfo.getServerName() > no2.ServerInfo.getServerName(); }

  void ServerList::sortByName()
  {
    this->serverlist.sort( sub_compare_names );
  }

  /* sort by ping */
  bool sub_compare_pings( ServerListElem no1,
    ServerListElem no2 )
  {
    return no1.ServerInfo.getServerRTT() > no2.ServerInfo.getServerRTT();
  }

  void ServerList::sortByPing()
  {
    this->serverlist.sort( sub_compare_pings );
  }

  bool ServerList::setNameByAddress( std::string address, std::string name  ){
    /* get an iterator */
    std::list<ServerListElem>::iterator i;

    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i )
      if( (*i).ServerInfo.getServerIP() == address )
      { /* found this adress, rename and quit */
        (*i).ServerInfo.setServerName( name );
        return true;
      }
    return false;
  };

  bool ServerList::setClientsByAddress( std::string address, int clientNumber ){
    /* get an iterator */
    std::list<ServerListElem>::iterator i;

    /* loop through list elements */
    for( i = serverlist.begin(); i != serverlist.end(); ++i )
      if( (*i).ServerInfo.getServerIP() == address )
      { /* found this adress, rename and quit */
        (*i).ServerInfo.setClientNumber( clientNumber );
        return true;
      }
    return false;
  };

}
