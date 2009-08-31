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

#include "MultiStateEngine.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "objects/worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    static const float FORWARD_EFFECT_VELOCITY_THRESHOLD = 20;

    static const unsigned char STATE_ACTIVE  = 1;
    static const unsigned char STATE_FORWARD = 2;
    static const unsigned char STATE_BOOST   = 4;
    static const unsigned char STATE_BRAKE   = 8;

    CreateFactory(MultiStateEngine);

    MultiStateEngine::MultiStateEngine(BaseObject* creator) : Engine(creator)
    {
        RegisterObject(MultiStateEngine);

        this->state_ = 0;

        this->registerVariables();
    }

    MultiStateEngine::~MultiStateEngine()
    {
        if (this->isInitialized() && !this->getShip())
        {
            // We have no ship, so the effects are not attached and won't be destroyed automatically
            for (std::list<WorldEntity*>::const_iterator it = this->activeEffects_.begin(); it != this->activeEffects_.end(); ++it)
                delete (*it);
            for (std::list<WorldEntity*>::const_iterator it = this->forwardEffects_.begin(); it != this->forwardEffects_.end(); ++it)
                delete (*it);
            for (std::list<WorldEntity*>::const_iterator it = this->boostEffects_.begin(); it != this->boostEffects_.end(); ++it)
                delete (*it);
            for (std::list<WorldEntity*>::const_iterator it = this->brakeEffects_.begin(); it != this->brakeEffects_.end(); ++it)
                delete (*it);
        }
    }

    void MultiStateEngine::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MultiStateEngine, XMLPort, xmlelement, mode);

        XMLPortObject(MultiStateEngine, WorldEntity, "active",  addActiveEffect,  getActiveEffect,  xmlelement, mode);
        XMLPortObject(MultiStateEngine, WorldEntity, "forward", addForwardEffect, getForwardEffect, xmlelement, mode);
        XMLPortObject(MultiStateEngine, WorldEntity, "boost",   addBoostEffect,   getBoostEffect,   xmlelement, mode);
        XMLPortObject(MultiStateEngine, WorldEntity, "brake",   addBrakeEffect,   getBrakeEffect,   xmlelement, mode);
    }

    void MultiStateEngine::registerVariables()
    {
        registerVariable(this->state_, VariableDirection::ToServer);
    }

    void MultiStateEngine::tick(float dt)
    {
        if (this->getShip())
        {
            if (this->getShip()->hasLocalController())
            {
                this->setObjectMode(ObjectDirection::Bidirectional);

                const Vector3& direction = this->getDirection();
                const Vector3& velocity = this->getShip()->getLocalVelocity();

                bool forward = (direction.z < 0 && velocity.z < -FORWARD_EFFECT_VELOCITY_THRESHOLD);
                bool boost = (this->getShip()->getBoost() && forward);
                bool brake = (direction.z > 0 && velocity.z < 0);
                bool active = (direction != Vector3::ZERO && !brake);

                if (active)
                    this->state_ |= STATE_ACTIVE;
                else
                    this->state_ &= ~STATE_ACTIVE;

                if (forward)
                    this->state_ |= STATE_FORWARD;
                else
                    this->state_ &= ~STATE_FORWARD;

                if (boost)
                    this->state_ |= STATE_BOOST;
                else
                    this->state_ &= ~STATE_BOOST;

                if (brake)
                    this->state_ |= STATE_BRAKE;
                else
                    this->state_ &= ~STATE_BRAKE;
            }

            if (GameMode::isMaster())
            {
                for (std::list<WorldEntity*>::const_iterator it = this->activeEffects_.begin(); it != this->activeEffects_.end(); ++it)
                    (*it)->setMainState(this->state_ & STATE_ACTIVE);
                for (std::list<WorldEntity*>::const_iterator it = this->forwardEffects_.begin(); it != this->forwardEffects_.end(); ++it)
                    (*it)->setMainState(this->state_ & STATE_FORWARD);
                for (std::list<WorldEntity*>::const_iterator it = this->boostEffects_.begin(); it != this->boostEffects_.end(); ++it)
                    (*it)->setMainState(this->state_ & STATE_BOOST);
                for (std::list<WorldEntity*>::const_iterator it = this->brakeEffects_.begin(); it != this->brakeEffects_.end(); ++it)
                    (*it)->setMainState(this->state_ & STATE_BRAKE);
            }
        }

        SUPER(MultiStateEngine, tick, dt);
    }

    void MultiStateEngine::addToSpaceShip(SpaceShip* ship)
    {
        Engine::addToSpaceShip(ship);

        if (!ship)
            return;

        for (std::list<WorldEntity*>::const_iterator it = this->activeEffects_.begin(); it != this->activeEffects_.end(); ++it)
            ship->attach(*it);
        for (std::list<WorldEntity*>::const_iterator it = this->forwardEffects_.begin(); it != this->forwardEffects_.end(); ++it)
            ship->attach(*it);
        for (std::list<WorldEntity*>::const_iterator it = this->boostEffects_.begin(); it != this->boostEffects_.end(); ++it)
            ship->attach(*it);
        for (std::list<WorldEntity*>::const_iterator it = this->brakeEffects_.begin(); it != this->brakeEffects_.end(); ++it)
            ship->attach(*it);
    }

    void MultiStateEngine::addActiveEffect(WorldEntity* effect)
    {
        this->activeEffects_.push_back(effect);
        if (this->getShip())
            this->getShip()->attach(effect);
    }

    void MultiStateEngine::addForwardEffect(WorldEntity* effect)
    {
        this->forwardEffects_.push_back(effect);
        if (this->getShip())
            this->getShip()->attach(effect);
    }

    void MultiStateEngine::addBoostEffect(WorldEntity* effect)
    {
        this->boostEffects_.push_back(effect);
        if (this->getShip())
            this->getShip()->attach(effect);
    }

    void MultiStateEngine::addBrakeEffect(WorldEntity* effect)
    {
        this->brakeEffects_.push_back(effect);
        if (this->getShip())
            this->getShip()->attach(effect);
    }

    WorldEntity* MultiStateEngine::getActiveEffect(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<WorldEntity*>::const_iterator it = this->activeEffects_.begin(); it != this->activeEffects_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    WorldEntity* MultiStateEngine::getForwardEffect(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<WorldEntity*>::const_iterator it = this->forwardEffects_.begin(); it != this->forwardEffects_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    WorldEntity* MultiStateEngine::getBoostEffect(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<WorldEntity*>::const_iterator it = this->boostEffects_.begin(); it != this->boostEffects_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    WorldEntity* MultiStateEngine::getBrakeEffect(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<WorldEntity*>::const_iterator it = this->brakeEffects_.begin(); it != this->brakeEffects_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }
}
