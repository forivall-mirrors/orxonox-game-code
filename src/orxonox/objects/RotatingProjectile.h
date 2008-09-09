#ifndef _RotatingProjectile_H__
#define _RotatingProjectile_H__

#include "OrxonoxPrereqs.h"
#include "util/Math.h"
#include "BillboardProjectile.h"

namespace orxonox
{
    class _OrxonoxExport RotatingProjectile : public BillboardProjectile
    {
        public:
            RotatingProjectile(SpaceShip* owner = 0);
            virtual ~RotatingProjectile();
            void setConfigValues();
            void colourChanged();
            virtual void tick(float dt);
            virtual void changedVisibility();

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
