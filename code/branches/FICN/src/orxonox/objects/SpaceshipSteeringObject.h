#ifndef _OBJECT_SPACESHIPSTEERING_H__
#define _OBJECT_SPACESHIPSTEERING_H__

#include "BaseObject.h"
#include "util/tinyxml/tinyxml.h"

namespace orxonox
{
    class SpaceshipSteeringObject : public BaseObject
    {
        public:
            SpaceshipSteeringObject();
            virtual ~SpaceshipSteeringObject();


            void loadParams(TiXmlElement* xmlElem);

        private:


    };
}

#endif
