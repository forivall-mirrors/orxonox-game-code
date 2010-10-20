#ifndef _MasterServer_
#define _MasterServer_

/* orxonox includes */
#include <enet/enet.h>
#include "packet/Chat.h"
#include "packet/ClassID.h"
#include "packet/DeleteObjects.h"
#include "packet/FunctionIDs.h"
#include "packet/Gamestate.h"
#include "packet/Welcome.h"

/* my includes */
#include <ServerList.h>

/* c compatibility */
#include <cstdio>

#define ORX_MSERVER_PORT 1234
#define ORX_MSERVER_MAXCONNS 32
#define ORX_MSERVER_MAXCHANS 2

#endif /* _MasterServer_ */
