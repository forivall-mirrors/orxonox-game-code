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
            Projectile();
            virtual ~Projectile();
            void setConfigValues();
            void destroyObject();
            virtual void tick(float dt);
            void setOwner(SpaceShip* owner = 0);

        protected:
            BillboardSet billboard_;
            float speed_;

        private:
            SpaceShip* owner_;
            float lifetime_;
            Timer<Projectile> destroyTimer_;
    };
    ExportClass(Projectile, Orxonox);
}

#endif /* _Projectile_H__ */
