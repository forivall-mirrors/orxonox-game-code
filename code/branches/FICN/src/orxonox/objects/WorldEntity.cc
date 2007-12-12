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

        this->bStatic_ = true;
        this->velocity_ = Vector3(0, 0, 0);
        this->acceleration_ = Vector3(0, 0, 0);
        this->rotationAxis_ = Vector3(0, 1, 0);
        this->rotationRate_ = 0;
        this->momentum_ = 0;
    }

    WorldEntity::~WorldEntity()
    {
    }

    void WorldEntity::tick(float dt)
    {
        if (!this->bStatic_)
        {
            this->velocity_ += (dt * this->acceleration_);
            this->translate(dt * this->velocity_);

            this->rotationRate_ += (dt * this->momentum_);
            this->rotate(this->rotationAxis_, dt * this->rotationRate_);
        }
    }
}
