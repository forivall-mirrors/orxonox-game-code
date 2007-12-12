#ifndef _OBJECT_SKYBOX_H__
#define _OBJECT_SKYBOX_H__

#include "BaseObject.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
    class Skybox : public BaseObject
    {
        public:
            Skybox();
            virtual ~Skybox();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif
