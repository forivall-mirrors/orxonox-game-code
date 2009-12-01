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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "MultiStateEngine.h"

extern "C" {
#include <lua.h>
}

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/LuaState.h"
#include "core/XMLPort.h"
#include "worldentities/EffectContainer.h"
#include "worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    static const float FORWARD_EFFECT_VELOCITY_THRESHOLD = 20;

    CreateFactory(MultiStateEngine);

    MultiStateEngine::MultiStateEngine(BaseObject* creator) : Engine(creator)
    {
        RegisterObject(MultiStateEngine);

        this->lua_ = new LuaState();
        this->state_ = 0;

        this->registerVariables();
    }

    MultiStateEngine::~MultiStateEngine()
    {
        if (this->isInitialized() && !this->getShip())
        {
            // We have no ship, so the effects are not attached and won't be destroyed automatically
            for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
                for (std::vector<WorldEntity*>::const_iterator it2 = (*it)->getEffectsBegin(); it2 != (*it)->getEffectsBegin(); ++it2)
                    (*it2)->destroy();
            delete this->lua_;
        }
    }

    void MultiStateEngine::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MultiStateEngine, XMLPort, xmlelement, mode);
        XMLPortObject(MultiStateEngine, EffectContainer, "",  addEffectContainer,  getEffectContainer,  xmlelement, mode);
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
                this->setSyncMode(ObjectDirection::Bidirectional);

                const Vector3& direction = this->getDirection();
                const Vector3& velocity = this->getShip()->getLocalVelocity();

                bool forward = (direction.z < 0 && velocity.z < -FORWARD_EFFECT_VELOCITY_THRESHOLD);

                int newState = 0;
                if (this->getShip()->getBoost() && forward)
                    newState = Boost;
                else if (forward && !newState) // newState == Boost
                    newState = Normal;
                else if (direction.z > 0 && velocity.z < 0)
                    newState = Brake;
                else
                    newState = Idle;

                if (newState != this->state_)
                {
                    int changes = newState | this->state_;
                    if (changes & Idle)
                    {
                        lua_pushboolean(this->lua_->getInternalLuaState(), newState & Idle);
                        lua_setglobal(this->lua_->getInternalLuaState(), "idle");
                    }
                    if (changes & Normal)
                    {
                        lua_pushboolean(this->lua_->getInternalLuaState(), newState & Normal);
                        lua_setglobal(this->lua_->getInternalLuaState(), "normal");
                    }
                    if (changes & Brake)
                    {
                        lua_pushboolean(this->lua_->getInternalLuaState(), newState & Brake);
                        lua_setglobal(this->lua_->getInternalLuaState(), "brake");
                    }
                    if (changes & Boost)
                    {
                        lua_pushboolean(this->lua_->getInternalLuaState(), newState & Boost);
                        lua_setglobal(this->lua_->getInternalLuaState(), "boost");
                    }

                    // Update all effect conditions
                    for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
                        (*it)->updateCondition();

                    this->state_ = newState;
                }
            }

            if (GameMode::isMaster())
            {
            }
        }

        SUPER(MultiStateEngine, tick, dt);
    }

    void MultiStateEngine::addToSpaceShip(SpaceShip* ship)
    {
        Engine::addToSpaceShip(ship);

        if (!ship)
            return;

        for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
            for (std::vector<WorldEntity*>::const_iterator it2 = (*it)->getEffectsBegin(); it2 != (*it)->getEffectsEnd(); ++it2)
                this->getShip()->attach(*it2);
    }

    void MultiStateEngine::addEffectContainer(EffectContainer* effect)
    {
        if (effect == NULL)
            return;
        effect->setLuaState(this->lua_, "f" + multi_cast<std::string>(this->effectContainers_.size()));
        this->effectContainers_.push_back(effect);
        if (this->getShip())
        {
            for (std::vector<WorldEntity*>::const_iterator it = effect->getEffectsBegin(); it != effect->getEffectsBegin(); ++it)
                this->getShip()->attach(*it);
        }
    }

    EffectContainer* MultiStateEngine::getEffectContainer(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
        {
            if (i == index)
                return (*it);
        }
        return NULL;
    }
}
