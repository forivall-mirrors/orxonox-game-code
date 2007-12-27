#ifndef _Projectile_H__
#define _Projectile_H__

#include "WorldEntity.h"
#include "BillboardSet.h"
#include "SpaceShip.h"
#include "Timer.h"

namespace orxonox
{
    class Projectile : public WorldEntity
    {
        public:
            Projectile(SpaceShip* owner = 0);
            virtual ~Projectile();
            void setConfigValues();
            void destroyObject();
            virtual void tick(float dt);

        private:
            SpaceShip* owner_;
            BillboardSet billboard_;
            float speed_;
            float lifetime_;
            Timer<Projectile> destroyTimer_;
    };
}

#endif /* _Projectile_H__ */
