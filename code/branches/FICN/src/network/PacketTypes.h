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

#ifndef NETWORK_PACKETTYPES_H
#define NETWORK_PACKETTYPES_H

namespace network{
enum packet_id {
  ACK,
  MOUSE,
  KEYBOARD,
  CHAT,
  GAMESTATE ,
  CLASSID
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
    bool diffed;
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
    bool diffed;
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

  struct mouse {
    int id;
    double x;
    double y;
  };

  struct keyboard {
    int id;
    char press;
  };
	//only in this class, not PacketGenerator, used as pattern to put incoming
	//bytes inside
  struct chat {
    int id;
    const char* message;
  };

}

#endif
