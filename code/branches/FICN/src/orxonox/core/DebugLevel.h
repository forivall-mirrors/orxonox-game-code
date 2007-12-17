#ifndef _DebugLevel_H__
#define _DebugLevel_H__

#include "OrxonoxClass.h"

namespace orxonox
{
    class DebugLevel : public OrxonoxClass
    {
        public:
            DebugLevel();
            static int getSoftDebugLevel();

        private:
            int softDebugLevel_s;
            static DebugLevel* pointer_s;
            static bool bCreatingDebugLevelObject_s;
    };
}

#endif
