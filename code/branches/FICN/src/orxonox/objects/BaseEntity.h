#ifndef _OBJECT_ENTITY_H__
#define _OBJECT_ENTITY_H__

#include "WorldEntity.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
  class BaseEntity : public WorldEntity
  {
    public:
      BaseEntity();
      virtual ~BaseEntity();

      
      void loadParams(TiXmlElement* xmlElem);
      bool create();
    private:
      void registerAllVariables();
      
    // membervariables to be synchronised
      std::string name_;
      std::string mesh_;
    // other membervariables
      Ogre::SceneNode *sceneNode_;
      bool valid;
  };
}

#endif
