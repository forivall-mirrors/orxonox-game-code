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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpPlatformTimer.cc
    @brief This platform will explode if you wait for too long time.
*/

#include "JumpPlatformTimer.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "graphics/ParticleSpawner.h"
#include "JumpFigure.h"

namespace orxonox
{
    RegisterClass(JumpPlatformTimer);

    JumpPlatformTimer::JumpPlatformTimer(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformTimer);

        particleSpawner_ = NULL;

        setProperties(3.0);

    }

    JumpPlatformTimer::~JumpPlatformTimer()
    {

    }

    void JumpPlatformTimer::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformTimer, XMLPort, xmlelement, mode);

        XMLPortParam(JumpPlatformTimer, "effectPath", setEffectPath, getEffectPath, xmlelement, mode);
        XMLPortParam(JumpPlatformTimer, "effectStartTime", setEffectStartTime, getEffectStartTime, xmlelement, mode);
    }

    void JumpPlatformTimer::tick(float dt)
    {
        SUPER(JumpPlatformTimer, tick, dt);

        time_ -= dt;
        if (time_ < effectStartTime_ && particleSpawner_ == NULL)
        {

            particleSpawner_ = new ParticleSpawner(getContext());
            particleSpawner_->setSource(effectPath_);
            particleSpawner_->setLoop(false);
            particleSpawner_->setLOD(LODParticle::Low);
            particleSpawner_->setLifetime(effectStartTime_);

            attach(particleSpawner_);
        }
    }

    void JumpPlatformTimer::setProperties(float time)
    {
        time_ = time;
    }

    bool JumpPlatformTimer::isActive(void)
    {
        return time_ > 0.0;
    }

    void JumpPlatformTimer::touchFigure()
    {
        if (isActive())
        {
            figure_->JumpFromPlatform(this);
        }
    }
}
