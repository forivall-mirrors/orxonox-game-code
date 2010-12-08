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

#ifndef _ServerList_
#define _ServerList_

#include <list>
#include <string>
#include <network/packet/ServerInformation.h>

/* methods necessary */
namespace orxonox 
{ 
  /** This class is keeps a list of game servers
   * and some info about them.
   */
  class ServerList 
  { public:
      /** constructor */
      ServerList();

      /** destructor */
      ~ServerList();


      /* BASIC MANIPULATION */
      /** \param toadd the server to add.
       * 
       * Add server to the game server list
       */
      int addServer( packet::ServerInformation toadd );

      /** \param name Name of the server to remove
       * 
       * Remove server by name 
       */
      bool delServerByName( std::string name );

      /** \param address IP address of the server to remove
       * 
       * Remove server by address
       */
      bool delServerByAddress( std::string address );


      /* SORTING (to be implemented) */

      /** sort by name  */
      void sortByName();
      
      /** sort by ping */
      void sortByPing();

      /** the list of servers for internal storage */
      std::list<packet::ServerInformation> serverlist;
    private:
  };
}

#endif /*_ServerList_*/
