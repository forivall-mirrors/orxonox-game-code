#include <string>
#include <sstream>

#include "WorldEntity.h"
#include "../core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(WorldEntity);

    Ogre::SceneManager* WorldEntity::sceneManager_s = 0;
    unsigned int WorldEntity::worldEntityCounter_s = 0;
    int WorldEntity::num_s = 0;

    WorldEntity::WorldEntity()
    {
        RegisterObject(WorldEntity);

        if (WorldEntity::sceneManager_s)
            WorldEntity::sceneManager_s->setAmbientLight( ColourValue( 1, 1, 1 ) ); // remove this

        if (WorldEntity::sceneManager_s)
        {
            std::ostringstream name;
            name << (WorldEntity::worldEntityCounter_s++);
            this->setName("WorldEntity" + name.str());
            node_ = WorldEntity::sceneManager_s->getRootSceneNode()->createChildSceneNode(this->getName());
        }

        this->bStatic_ = false;
        this->velocity_ = Vector3(0, 0, 0);
    }

    WorldEntity::~WorldEntity()
    {
    }

    void WorldEntity::tick(float dt)
    {
        if (!this->bStatic_)
        {
            this->translate(this->velocity_);
        }
    }
}
