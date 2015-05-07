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

#include "PeerList.h"

#include <cstdio>
#include "util/Output.h"
#include "network/packet/ServerInformation.h"

namespace orxonox
{
  PeerList::PeerList() { }
  PeerList::~PeerList() { }

  int
  PeerList::addPeer( ENetPeer *toadd )
  { /* error correction */
    if( toadd == NULL )
    { orxout(internal_error, context::master_server) << "PeerList::addPeer: empty peer given." << endl;
      return -1;
    }

    /* if all went well, push to back of list */
    this->peerlist.push_back( toadd );
    return 0;
  }

  bool sub_compAddr( ENetAddress addr1, ENetAddress addr2 )
  {
    for( int i = 0; i < 16; ++i )
      if( addr1.host.addr[i] < addr2.host.addr[i] )
        return -i;
      else if( addr1.host.addr[i] > addr2.host.addr[i] )
        return i;

    return 0;
  }


  bool
  PeerList::remPeerByAddr( ENetAddress addr )
  { /* get an iterator */
    std::list<ENetPeer *>::iterator i;

    /* loop through list elements */
    for( i = peerlist.begin(); i != peerlist.end(); ++i )
      if( !sub_compAddr((*i)->address, addr ) )
      { /* found this name, remove and quit */
        this->peerlist.remove( *i );
        return true;
      }

    /* not found */
    return false;
  }

  ENetPeer *
  PeerList::findPeerByAddr( ENetAddress addr )
  { /* get an iterator */
    std::list<ENetPeer *>::iterator i;

    /* loop through list elements */
    for( i = peerlist.begin(); i != peerlist.end(); ++i )
      if( !sub_compAddr((*i)->address, addr ) )
        /* found this name, remove and quit */
        return *i;

    /* not found */
    return NULL;
  }

  int
  PeerList::count(){
    return this->peerlist.size();
  }

}
