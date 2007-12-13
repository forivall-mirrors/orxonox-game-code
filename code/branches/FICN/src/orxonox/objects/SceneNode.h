#ifndef _OBJECT_SCENENODE_H__
#define _OBJECT_SCENENODE_H__

#include "BaseObject.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
    class SceneNode : public BaseObject
    {
        public:
            SceneNode();
            virtual ~SceneNode();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif
