#include "core/CoreIncludes.h"
#include "Timer.h"

namespace orxonox
{
    /**
        @brief Constructor: Sets the default-values.
    */
    TimerBase::TimerBase()
    {
        RegisterRootObject(TimerBase);

        this->interval_ = 0;
        this->bLoop_ = false;
        this->bActive_ = false;

        this->time_ = 0;
    }
}
