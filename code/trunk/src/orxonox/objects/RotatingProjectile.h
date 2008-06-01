#ifndef _RotatingProjectile_H__
#define _RotatingProjectile_H__

#include "../OrxonoxPrereqs.h"
#include "util/Math.h"
#include "Projectile.h"

namespace orxonox
{
    class _OrxonoxExport RotatingProjectile : public Projectile/*, public network::Synchronisable*/
    {
        public:
            RotatingProjectile(SpaceShip* owner = 0);
            virtual ~RotatingProjectile();
            void setConfigValues();
            virtual void tick(float dt);
            virtual bool create(){return Projectile::create();}

        private:
            ColourValue colour_;
            BillboardSet rotatingBillboard1_;
            BillboardSet rotatingBillboard2_;
            Ogre::SceneNode* rotatingNode1_;
            Ogre::SceneNode* rotatingNode2_;
            float time_;
    };
}

#endif /* _RotatingProjectile_H__ */
