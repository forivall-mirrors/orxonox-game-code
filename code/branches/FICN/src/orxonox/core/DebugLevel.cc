#include "CoreIncludes.h"
#include "DebugLevel.h"
#include "Debug.h"

namespace orxonox
{
    int DebugLevel::softDebugLevel_s;

//    CreateFactory(DebugLevel);

    DebugLevel::DebugLevel()
    {
        RegisterObject(DebugLevel);
        SetConfigValue(softDebugLevel_s, 2);
    }

    int DebugLevel::getSoftDebugLevel()
    {
        return DebugLevel::softDebugLevel_s;
    }
}

int getSoftDebugLevel()
{
    return orxonox::DebugLevel::getSoftDebugLevel();
}
