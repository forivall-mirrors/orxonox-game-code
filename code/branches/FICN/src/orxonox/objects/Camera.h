#ifndef _OBJECT_CAMERA_H__
#define _OBJECT_CAMERA_H__

#include "BaseObject.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
    class Camera : public BaseObject
    {
        public:
            Camera();
            virtual ~Camera();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif
