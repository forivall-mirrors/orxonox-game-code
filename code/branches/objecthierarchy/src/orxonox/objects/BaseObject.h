#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "../core/CoreIncludes.h"

namespace orxonox
{
    class BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();
    };
}

#endif
