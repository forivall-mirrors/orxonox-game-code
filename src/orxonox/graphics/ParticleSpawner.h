/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ParticleSpawner_H__
#define _ParticleSpawner_H__

#include "OrxonoxPrereqs.h"

#include "tools/Timer.h"
#include "ParticleEmitter.h"

namespace orxonox
{
    class _OrxonoxExport ParticleSpawner : public ParticleEmitter
    {
        public:
            ParticleSpawner(BaseObject* creator);
            virtual ~ParticleSpawner();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            inline void stop(bool bDestroy)
                { this->bForceDestroy_ = bDestroy; this->stopParticleSpawner(); }
            inline void spawn()
                { this->bSuppressStart_ = false; this->startParticleSpawner(); }

            void configure(float lifetime = 0, float startdelay = 0, float destroydelay = 0, bool autodestroy = true);

            inline void setAutoStart(bool autostart)
                { this->bAutostart_ = autostart; this->bSuppressStart_ = !autostart; }
            inline bool getAutoStart() const
                { return this->bAutostart_; }

            inline void setDestroyAfterLife(bool destroy)
                { this->bAutoDestroy_ = destroy; }
            inline bool getDestroyAfterLife() const
                { return this->bAutoDestroy_; }

            inline void setLoop(bool loop)
                { this->bLoop_ = loop; }
            inline bool getLoop() const
                { return this->bLoop_; }

            inline void setLifetime(float lifetime)
                { this->lifetime_ = lifetime; this->startParticleSpawner(); }
            inline float getLifetime() const
                { return this->lifetime_; }

            inline void setStartdelay(float startdelay)
                { this->startdelay_ = startdelay; this->startParticleSpawner(); }
            inline float getStartdelay() const
                { return this->startdelay_; }

            inline void setDestroydelay(float destroydelay)
                { this->destroydelay_ = destroydelay; this->startParticleSpawner(); }
            inline float getDestroydelay() const
                { return this->destroydelay_; }

        private:
            void startParticleSpawner();
            void fireParticleSpawner();
            void stopParticleSpawner();
            void destroyParticleSpawner();

            Timer timer_;

            bool  bSuppressStart_;
            bool  bAutostart_;
            bool  bForceDestroy_;
            bool  bAutoDestroy_;
            bool  bLoop_;
            float startdelay_;
            float lifetime_;
            float destroydelay_;
    };
}

#endif /* _ParticleSpawner_H__ */
