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
            ~Projectile();
            void destroyObject();

        private:
            SpaceShip* owner_;
            BillboardSet billboard_;
            float speed_;
            float lifetime_;
            Timer<Projectile> destroyTimer_;
    };
}

#endif
