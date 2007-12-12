#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "../core/IdentifierIncludes.h"

namespace orxonox
{
    class BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();
			virtual void loadParams(TiXmlElement* xmlElem) {}
            
    };
}

#endif
