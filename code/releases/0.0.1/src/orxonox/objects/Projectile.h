#ifndef _Projectile_H__
#define _Projectile_H__

#include "../OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "../tools/BillboardSet.h"
#include "../tools/Timer.h"

namespace orxonox
{
    class SpaceShip; // Forward declaration

    class _OrxonoxExport Projectile : public WorldEntity
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
    ExportClass(Projectile, Orxonox);
}

#endif /* _Projectile_H__ */
