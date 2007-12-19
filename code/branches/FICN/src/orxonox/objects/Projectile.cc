#include "Projectile.h"
#include "../core/CoreIncludes.h"

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
            this->setVelocity(Vector3(1, 0, 0) * this->speed_);
        }

        this->destroyTimer_.setTimer(this->lifetime_, false, this, &Projectile::destroyObject);
    }

    Projectile::~Projectile()
    {
    }

    void Projectile::destroyObject()
    {
        delete this;
    }
}
