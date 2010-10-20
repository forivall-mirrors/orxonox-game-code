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

#ifndef _MasterServer_
#define _MasterServer_

/* orxonox includes */
#include <enet/enet.h>
#include <network/packet/Chat.h>
#include <network/packet/ClassID.h>
#include <network/packet/DeleteObjects.h>
#include <network/packet/FunctionIDs.h>
#include <network/packet/Gamestate.h>
#include <network/packet/Welcome.h>

/* my includes */
#include "ServerList.h"

/* c compatibility */
#include <cstdio>

#define ORX_MSERVER_PORT 1234
#define ORX_MSERVER_MAXCONNS 32
#define ORX_MSERVER_MAXCHANS 2

#endif /* _MasterServer_ */
