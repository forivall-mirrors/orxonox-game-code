#ifndef INERTIAL_NODE_H
#define INERTIAL_NODE_H

#include "OgrePrerequisites.h"

#include "orxonox_prerequisites.h"


namespace orxonox {

  class InertialNode
  {
  public:
    InertialNode(Ogre::SceneNode*, Ogre::Vector3);
    ~InertialNode();

    InertialNode* createChildNode();

    InertialNode* getParentNode();

    Ogre::SceneNode* getSceneNode();

    Ogre::Vector3 getSpeed();

    Ogre::Vector3 getWorldSpeed();

    void addSpeed(Ogre::Vector3);

  protected:
    InertialNode(InertialNode*, Ogre::Vector3 speed = Ogre::Vector3::ZERO);
    void addChild(InertialNode*);

  public:

  protected:
    InertialNode **childList_;
    int childListSize_;
    int childListIndex_;

    Ogre::SceneNode *node_;
    Ogre::Vector3 speed_;

    InertialNode* parentNode_;

  };

}

#endif /* INERTIAL_NODE_H */
