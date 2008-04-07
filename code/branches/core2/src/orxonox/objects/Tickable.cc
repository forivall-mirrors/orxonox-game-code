#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "Tickable.h"

namespace orxonox
{
    ConsoleCommandShortcutExtern(slomo, AccessLevel::Offline).setDefaultValue(0, 1.0);
    ConsoleCommandShortcutExtern(setTimeFactor, AccessLevel::Offline).setDefaultValue(0, 1.0);

    static float timefactor = 1.0;
    void slomo(float factor)
    {
        setTimeFactor(factor);
    }
    void setTimeFactor(float factor)
    {
        timefactor = factor;
    }
    float getTimeFactor()
    {
        return timefactor;
    }

    /**
        @brief Constructor: Registers the object in the Tickable-list
    */
    Tickable::Tickable()
    {
        RegisterRootObject(Tickable);
    }
}
