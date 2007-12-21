#include "Explosion.h"
#include "../Orxonox.h"

namespace orxonox
{
    CreateFactory(Explosion);

    Explosion::Explosion(WorldEntity* owner)
    {
        RegisterObject(Explosion);

        this->particle_ = 0;
        this->lifetime_ = 0.4;

        if (owner)
        {
            this->destroyTimer_.setTimer(this->lifetime_, false, this, &Explosion::destroyObject);

            Vector3 position = owner->getNode()->getWorldPosition();

            this->particle_ = new particle::ParticleInterface(Orxonox::getSingleton()->getSceneManager(), "explosion" + this->getName(), "Orxonox/treibwerk");
            this->particle_->getParticleSystem()->setParameter("local_space", "true");
            this->particle_->newEmitter();
            this->particle_->setPositionOfEmitter(0, Vector3::ZERO);
            this->particle_->setPositionOfEmitter(1, Vector3::ZERO);
            this->particle_->setColour(ColourValue(1.0, 0.8, 0.2));

            this->setPosition(position);
            this->scale(2);

            this->particle_->addToSceneNode(this->getNode());
        }
    }

    Explosion::~Explosion()
    {
        if (this->particle_)
        {
            this->particle_->detachFromSceneNode();
            delete this->particle_;
        }
    };

    void Explosion::destroyObject()
    {
        delete this;
    }
}
