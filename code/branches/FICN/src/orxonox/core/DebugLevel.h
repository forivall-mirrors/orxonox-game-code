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
            static int softDebugLevel_s;
    } myDebugObject;
}

#endif
