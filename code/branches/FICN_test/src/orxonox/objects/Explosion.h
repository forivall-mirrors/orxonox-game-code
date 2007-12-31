#ifndef _Explosion_H__
#define _Explosion_H__

#include "../OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "../tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport Explosion : public WorldEntity
    {
        public:
            Explosion(WorldEntity* owner = 0);
            virtual ~Explosion();
            void destroyObject();

        private:
            Timer<Explosion> destroyTimer_;
            float lifetime_;
            ParticleInterface* particle_;
    };
    ExportClass(Explosion, Orxonox);
}

#endif /* _Explosion_H__ */
