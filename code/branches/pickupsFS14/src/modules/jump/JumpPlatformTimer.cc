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

/**
    @file JumpPlatformTimer.cc
    @brief Implementation of the JumpPlatform class.
*/

#include "JumpPlatformTimer.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatformTimer);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpPlatformTimer::JumpPlatformTimer(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformTimer);

        particleSpawner = NULL;

        setProperties(3.0);

    }

    /**
    @brief
        Destructor.
    */
    JumpPlatformTimer::~JumpPlatformTimer()
    {

    }

    //xml port for loading sounds
    void JumpPlatformTimer::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformTimer, XMLPort, xmlelement, mode);

        XMLPortParam(JumpPlatformTimer, "effectPath", setEffectPath, getEffectPath, xmlelement, mode);
        XMLPortParam(JumpPlatformTimer, "effectStartTime", setEffectStartTime, getEffectStartTime, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpPlatformTimer::tick(float dt)
    {
        SUPER(JumpPlatformTimer, tick, dt);

        time_ -= dt;
        if (time_ < effectStartTime_ && particleSpawner == NULL)
        {

        	particleSpawner = new ParticleSpawner(getContext());
        	particleSpawner->setSource(effectPath_);
            particleSpawner->setLoop(false);
            particleSpawner->setLOD(LODParticle::Low);
            particleSpawner->setLifetime(effectStartTime_);

            attach(particleSpawner);
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
    	accelerateFigure();
    	if (isActive())
    	{
    		accelerateFigure();
    	}
    }
}
