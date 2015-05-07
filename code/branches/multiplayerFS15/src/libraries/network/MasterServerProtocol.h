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

#ifndef _MasterServerProtocol_H__
#define _MasterServerProtocol_H__

/* master server address (to be moved elsewhere later) */
#define MS_ADDRESS "129.132.3.8"


/*** CLIENT COMMUNICATIONS ***/
/* Client token (shows that the party sending data is a client */
#define MSPROTO_CLIENT "CL"
#define MSPROTO_CLIENT_LEN 2

/* Request: Serverlist (requiest made from client to master server */
#define MSPROTO_REQ_LIST "REQ:LIST"
#define MSPROTO_REQ_LIST_LEN 8




/*** GAME SERVER COMMUNICATIONS ***/
/* Game server token (shows that the party sending data is a game server) */
#define MSPROTO_GAME_SERVER "GS"
#define MSPROTO_GAME_SERVER_LEN 2

/* Register game server at the master server, meaning request to be taken into$
 * the master servers game server list.
 */
#define MSPROTO_REGISTER_SERVER "REG:SER"
#define MSPROTO_REGISTER_SERVER_LEN 7

/* Server list item token */
#define MSPROTO_SERVERLIST_ITEM "SI"
#define MSPROTO_SERVERLIST_ITEM_LEN 2

/* server list end token */
#define MSPROTO_SERVERLIST_END "SL_END"
#define MSPROTO_SERVERLIST_END_LEN 6

/* ping request from server */
#define MSPROTO_PING_GAMESERVER "PING"
#define MSPROTO_PING_GAMESERVER_LEN 4

/* server disconnect */
#define MSPROTO_SERVERDC "DC"
#define MSPROTO_SERVERDC_LEN 2

/* ping reply */
#define MSPROTO_ACK "ACK"
#define MSPROTO_ACK_LEN 3

#define MSPROTO_SET_NAME "NAM"
#define MSPROTO_SET_NAME_LEN 3

#define MSPROTO_SET_CLIENTS "CLI"
#define MSPROTO_SET_CLIENTS_LEN 3

#define SERVER_NAME_MAXLEN 7

/* default master server port */
#define ORX_MSERVER_PORT 55557


/* some constants for the networking of the master server, not very important.
 * play with those if things don't work out well.$
 */
#define ORX_MSERVER_MAXCONNS 32
#define ORX_MSERVER_MAXCHANS 2


#endif /* _MasterServerProtocol_H__ */
