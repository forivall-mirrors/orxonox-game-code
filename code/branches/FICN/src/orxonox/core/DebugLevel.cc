#include "CoreIncludes.h"
#include "DebugLevel.h"
#include "Debug.h"

namespace orxonox
{
    DebugLevel* DebugLevel::pointer_s = 0;
    bool DebugLevel::bCreatingDebugLevelObject_s = false;

    DebugLevel::DebugLevel()
    {
        RegisterRootObject(DebugLevel);
        SetConfigValue(softDebugLevel_s, 2);
    }

    int DebugLevel::getSoftDebugLevel()
    {
        if (!pointer_s && !bCreatingDebugLevelObject_s)
        {
            bCreatingDebugLevelObject_s = true;
            pointer_s = new DebugLevel;
            bCreatingDebugLevelObject_s = false;
        }

        if (bCreatingDebugLevelObject_s)
            return 4;

        return pointer_s->softDebugLevel_s;
    }
}

int getSoftDebugLevel()
{
    return orxonox::DebugLevel::getSoftDebugLevel();
}
