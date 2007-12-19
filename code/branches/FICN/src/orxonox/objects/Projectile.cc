#include "Projectile.h"
#include "../core/CoreIncludes.h"
#include "Explosion.h"
#include "Model.h"

namespace orxonox
{
    CreateFactory(Projectile);

    Projectile::Projectile(SpaceShip* owner)
    {
        RegisterObject(Projectile);

        this->owner_ = owner;

        SetConfigValue(lifetime_, 10.0);
        SetConfigValue(speed_, 2000.0);

        this->billboard_.setBillboardSet("Examples/Flare", ColourValue(1.0, 1.0, 0.5), 1);
        this->attachObject(this->billboard_.getBillboardSet());
        this->scale(0.5);

        if (this->owner_)
        {
            this->setStatic(false);
            this->setOrientation(this->owner_->getOrientation());
            this->setPosition(this->owner_->getPosition());
            this->translate(Vector3(55, 0, 0), Ogre::Node::TS_LOCAL);
            this->setVelocity(Vector3(1, 0, 0) * this->speed_);
        }

        this->destroyTimer_.setTimer(this->lifetime_, false, this, &Projectile::destroyObject);
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::tick(float dt)
    {
        WorldEntity::tick(dt);

        float radius;
        for (Iterator<Model> it = ObjectList<Model>::start(); it; ++it)
        {
            if ((*it) != this->owner_)
            {
                radius = it->getScale().x * 3.0;

                if (this->getPosition().squaredDistance(it->getPosition()) <= (radius*radius))
                {
                    Explosion* eplosion = new Explosion(this);
                    delete this;
                    return;
                }
            }
        }
    }

    void Projectile::destroyObject()
    {
        delete this;
    }
}
