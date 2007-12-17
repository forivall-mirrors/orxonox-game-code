#ifndef _OBJECT_SCENENODE_H__
#define _OBJECT_SCENENODE_H__

#include "BaseObject.h"
#include "../../tinyxml/tinyxml.h"
#include "network/Synchronisable.h"

namespace orxonox
{
    class SceneNode : public BaseObject, public network::Synchronisable
    {
        public:
          SceneNode();
          virtual ~SceneNode();

            
          void loadParams(TiXmlElement* xmlElem);
          bool create();
        protected:
          void registerAllVariables();
        private:
          float x_, y_, z_;
          float sx_, sy_, sz_;
          float yaw_, pitch_, roll_;
          std::string name_;
          std::string nodeName_;

    };
}

#endif
