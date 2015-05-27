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

#ifndef _PeerList_
#define _PeerList_

#include <list>
#include <string>
#include <enet/enet.h>

/* peer list */
namespace orxonox
{
  /** This class keeps a list of open connections
   * and some info about them.
   */
  class PeerList
  { public:
      /** constructor */
      PeerList();

      /** destructor */
      ~PeerList();

      /** \param toadd The peer to add
       * \return 0 for success, -1 for error.
       *
       * Add new peer to list
       */
      int addPeer( ENetPeer *toadd );

      /** \param addr Address to look for
       * \return if the peer was found or not
       *
       * Remove peer from list by address
       */
      bool remPeerByAddr( ENetAddress addr );

      /** \param addr The address to find by
       *
       * Find a connection by address
       */
      ENetPeer *findPeerByAddr( ENetAddress addr );

      /**
       * Count current peers
       */
       int count();

      /* NOTE: making this list public so it can easily
       * be iterated. This makes sense since iterating it
       * will happen all the time, and using getter methods
       * for the next in list would slow things down unnecessarily.
       */
      /** the list of servers for internal storage */
      std::list<ENetPeer *> peerlist;
    private:
      /* nothing so far. */
  };
}



#endif /* _PeerList_ */
