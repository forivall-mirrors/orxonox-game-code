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

#ifndef MASTER_SERVER_PROTO
#define MASTER_SERVER_PROTO

/* master server address (to be moved elsewhere later) */
#define MS_ADDRESS "129.132.3.227"

#define MSPROTO_CLIENT "CL"
#define MSPROTO_CLIENT_LEN 2 

#define MSPROTO_REQ_LIST "REQ:LIST"
#define MSPROTO_REQ_LIST_LEN 8




#define MSPROTO_GAME_SERVER "GS"
#define MSPROTO_GAME_SERVER_LEN 2

#define MSPROTO_REGISTER_SERVER "REG:SER"
#define MSPROTO_REGISTER_SERVER_LEN 7

#define MSPROTO_SERVERLIST_ITEM "SI"
#define MSPROTO_SERVERLIST_ITEM_LEN 2
#define MSPROTO_SERVERLIST_END "SL_END"
#define MSPROTO_SERVERLIST_END_LEN 6






#define ORX_MSERVER_PORT 1234


#define ORX_MSERVER_MAXCONNS 32
#define ORX_MSERVER_MAXCHANS 2




#endif /* MASTER_SERVER_PROTO */
