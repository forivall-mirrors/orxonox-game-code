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

#include "ParticleSpawner.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/Executor.h"
#include "core/XMLPort.h"
#include "tools/ParticleInterface.h"

namespace orxonox
{
    CreateFactory(ParticleSpawner);

    ParticleSpawner::ParticleSpawner(BaseObject* creator) : ParticleEmitter(creator)
    {
        RegisterObject(ParticleSpawner);

        this->bAutostart_ = true;
        this->bSuppressStart_ = false;
        this->bAutoDestroy_ = true;
        this->bForceDestroy_ = false;
        this->bLoop_ = false;
        this->startdelay_ = 0;
        this->lifetime_ = 0;
        this->destroydelay_ = 0;

        this->startParticleSpawner();
    }

    ParticleSpawner::~ParticleSpawner()
    {
    }

    void ParticleSpawner::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ParticleSpawner, XMLPort, xmlelement, mode);

        XMLPortParam(ParticleSpawner, "autostart",    setAutoStart,        getAutoStart,        xmlelement, mode).defaultValues(true);
        XMLPortParam(ParticleSpawner, "autodestroy",  setDestroyAfterLife, getDestroyAfterLife, xmlelement, mode).defaultValues(false);
        XMLPortParam(ParticleSpawner, "loop",         setLoop,             getLoop,             xmlelement, mode).defaultValues(false);
        XMLPortParam(ParticleSpawner, "lifetime",     setLifetime,         getLifetime,         xmlelement, mode).defaultValues(0.0f);
        XMLPortParam(ParticleSpawner, "startdelay",   setStartdelay,       getStartdelay,       xmlelement, mode).defaultValues(0.0f);
        XMLPortParam(ParticleSpawner, "destroydelay", setDestroydelay,     getDestroydelay,     xmlelement, mode).defaultValues(0.0f);
    }

    void ParticleSpawner::processEvent(Event& event)
    {
        SUPER(ParticleSpawner, processEvent, event);

        ORXONOX_SET_EVENT(ParticleSpawner, "spawn", spawn, event);
    }

    void ParticleSpawner::configure(float lifetime, float startdelay, float destroydelay, bool autodestroy)
    {
        this->bAutoDestroy_ = autodestroy;
        this->startdelay_ = startdelay;
        this->lifetime_ = lifetime;
        this->destroydelay_ = destroydelay;
    }

    void ParticleSpawner::startParticleSpawner()
    {
        if (!this->particles_)
            return;

        this->setActive(false);

        if (this->bForceDestroy_ || this->bSuppressStart_)
            return;

        this->timer_.setTimer(this->startdelay_, false, this, createExecutor(createFunctor(&ParticleSpawner::fireParticleSpawner)));
    }

    void ParticleSpawner::fireParticleSpawner()
    {
        this->setActive(true);
        if (this->lifetime_ != 0)
            this->timer_.setTimer(this->lifetime_, false, this, createExecutor(createFunctor(&ParticleSpawner::stopParticleSpawner)));
    }

    void ParticleSpawner::stopParticleSpawner()
    {
        this->setActive(false);

        if (this->bAutoDestroy_ || this->bForceDestroy_)
        {
            this->setPosition(this->getWorldPosition());
            this->detachFromParent();

            if (!this->timer_.isActive() || this->timer_.getRemainingTime() > this->destroydelay_)
                this->timer_.setTimer(this->destroydelay_, false, this, createExecutor(createFunctor(&ParticleSpawner::destroyParticleSpawner)));
        }
        else if (this->bLoop_)
        {
            this->timer_.setTimer(this->destroydelay_, false, this, createExecutor(createFunctor(&ParticleSpawner::startParticleSpawner)));
        }
    }

    void ParticleSpawner::destroyParticleSpawner()
    {
        delete this;
    }
}
