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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

//
// C++ Interface: PacketTypes
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _PacketTypes_H__
#define _PacketTypes_H__

#include "NetworkPrereqs.h"

namespace network
{
  enum packet_id {
    ACK,
    COMMAND,
    CHAT,
    GAMESTATE ,
    CLASSID,
    WELCOME,
    CONNECT,
  };


  /**
  * This struct defines a gamestate:
  * size: total size of the data in *data
  * data: pointer to the data allocated in the memory
  */
  struct GameState{
    int id;
    int size;                       //!< total size of data
    // new ---- change functions
    int base_id;                    // if gamestate is diffed this is the id of the old gamestate (base)
    bool diffed;
    bool complete;                  // this determines, wheter all objects are in this gamestate (server) or only few objects (client)
    unsigned char *data;            //!< pointer to data
  };

  /**
  * this struct defines a gamestate:
  * compsize is the size of the compressed data
  * normsize is the size of the uncompressed data
  * data are the gamestates
  */
  struct GameStateCompressed{
    int id;
    int compsize;                   //!< size of compressed data
    int normsize;                   //!< size of uncompressed data
    // new ----- change functions
    int base_id;                  // if gamestate is diffed this is the id of the old gamestate (base)
    bool diffed;
    bool complete;                  // this determines, wheter all objects are in this gamestate (server) or only few objects (client)
    unsigned char *data;            //!< gamestate data
  };

  struct classid{
    int id;
    int length;
    int clid;
    const char *message;
  };


  struct ack {
    int id;
    int a;
  };

  //only in this class, not PacketGenerator, used as pattern to put incoming
  //bytes inside
  struct chat {
    int id;
    const char* message;
  };

  struct welcome {
    int id;
    int clientID;
    int shipID;
    bool allowed;
  };
  
  struct connectRequest{
    int id;
  };
}

#endif /* _PacketTypes_H__ */
