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
