#ifndef _OBJECT_ENTITY_H__
#define _OBJECT_ENTITY_H__

#include "BaseObject.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
    class Entity : public BaseObject
    {
        public:
            Entity();
            virtual ~Entity();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif
