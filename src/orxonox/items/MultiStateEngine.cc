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
#include "sound/WorldSound.h"

namespace orxonox
{
    static const float FORWARD_EFFECT_VELOCITY_THRESHOLD = 0;
    static const float MAX_VELOCITY_NORMAL = 111;
    static const float MAX_VELOCITY_BOOST = 221;

    CreateFactory(MultiStateEngine);

    MultiStateEngine::MultiStateEngine(Context* context) : Engine(context)
    {
        RegisterObject(MultiStateEngine);

        if (GameMode::isMaster())
        {
            this->defEngineSndNormal_ = new WorldSound(this->getContext());
            this->defEngineSndBoost_  = new WorldSound(this->getContext());
            this->defEngineSndNormal_->setLooping(true);
            this->defEngineSndBoost_->setLooping(true);
            this->lua_ = new LuaState();
        }
        else
        {
            this->defEngineSndBoost_ = 0;
            this->defEngineSndNormal_ = 0;
            this->lua_ = 0;
        }
        this->state_ = 0;
        this->oldState_ = 0;

        this->setSyncMode(ObjectDirection::Bidirectional);
        this->registerVariables();
    }

    MultiStateEngine::~MultiStateEngine()
    {
        if (this->isInitialized())
        {
            if (!this->getShip())
            {
                // We have no ship, so the effects are not attached and won't be destroyed automatically
                for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
                    for (std::vector<WorldEntity*>::const_iterator it2 = (*it)->getEffectsBegin(); it2 != (*it)->getEffectsBegin(); ++it2)
                        (*it2)->destroy();
                if (this->defEngineSndNormal_)
                    this->defEngineSndNormal_->destroy();
                if (this->defEngineSndBoost_)
                    this->defEngineSndBoost_->destroy();
            }
            if (this->lua_)
                delete this->lua_;
        }
    }

    void MultiStateEngine::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MultiStateEngine, XMLPort, xmlelement, mode);
        XMLPortObject(MultiStateEngine, EffectContainer, "",  addEffectContainer,  getEffectContainer,  xmlelement, mode);
        XMLPortParam(MultiStateEngine, "defEngineSndNormal",  setDefEngSndNormal,  getDefEngSndNormal,  xmlelement, mode);
        XMLPortParam(MultiStateEngine, "defEngineSndBoost",  setDefEngSndBoost,  getDefEngSndBoost,  xmlelement, mode);
    }

    void MultiStateEngine::registerVariables()
    {
        registerVariable(this->state_, VariableDirection::ToServer);
    }

    void MultiStateEngine::run(float dt)
    {
        if (this->getShip())
        {
            const Vector3& velocity = this->getShip()->getLocalVelocity();

            if (this->getShip()->hasLocalController())
            {
                const Vector3& direction = this->getSteering();
                bool forward = (direction.z < 0.0 && velocity.z < -FORWARD_EFFECT_VELOCITY_THRESHOLD);

                this->state_ = 0;
                if (this->getShip()->isBoosting() && forward)
                    this->state_ = Boost;
                else if (forward && !this->state_) // this->state_ == Boost
                    this->state_ = Normal;
                else if (direction.z > 0.0 && velocity.z < 0.0)
                    this->state_ = Brake;
                else
                    this->state_ = Idle;

                if (this->state_ == Idle && this->getSpeedAdd() > 0)
                    this->state_ = Normal;
            }

            if (GameMode::isMaster())
            {
                int changes = this->state_ | this->oldState_;

                float pitch = velocity.length();
                if (this->state_ & Normal)
                    defEngineSndNormal_->setPitch(clamp(pitch/MAX_VELOCITY_NORMAL + 1, 0.5f, 2.0f));
                if (this->state_ & Boost)
                    defEngineSndBoost_->setPitch(clamp(pitch/MAX_VELOCITY_BOOST + 1, 0.5f, 2.0f));

                if (changes & Idle)
                {
                    lua_pushboolean(this->lua_->getInternalLuaState(), this->state_ & Idle);
                    lua_setglobal(this->lua_->getInternalLuaState(), "idle");
                }
                if (changes & Normal)
                {
                    lua_pushboolean(this->lua_->getInternalLuaState(), this->state_ & Normal);
                    lua_setglobal(this->lua_->getInternalLuaState(), "normal");
                    if (this->state_ & Normal)
                        defEngineSndNormal_->play();
                    else
                        defEngineSndNormal_->stop();
                }
                if (changes & Brake)
                {
                    lua_pushboolean(this->lua_->getInternalLuaState(), this->state_ & Brake);
                    lua_setglobal(this->lua_->getInternalLuaState(), "brake");
                }
                if (changes & Boost)
                {
                    lua_pushboolean(this->lua_->getInternalLuaState(), this->state_ & Boost);
                    lua_setglobal(this->lua_->getInternalLuaState(), "boost");
                    if (this->state_ & Boost)
                        defEngineSndBoost_->play();
                    else
                        defEngineSndBoost_->stop();
                }

                this->oldState_ = this->state_;

                // Update all effect conditions
                for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
                    (*it)->updateCondition();
            }
        }

        Engine::run(dt);
    }

    void MultiStateEngine::addToSpaceShip(SpaceShip* ship)
    {
        Engine::addToSpaceShip(ship);

        if (!ship)
            return;

        if( this->defEngineSndNormal_ )
            this->getShip()->attach(defEngineSndNormal_);
        if( this->defEngineSndBoost_ )
            this->getShip()->attach(defEngineSndBoost_);

        for (std::vector<EffectContainer*>::const_iterator it = this->effectContainers_.begin(); it != this->effectContainers_.end(); ++it)
            for (std::vector<WorldEntity*>::const_iterator it2 = (*it)->getEffectsBegin(); it2 != (*it)->getEffectsEnd(); ++it2)
                this->getShip()->attach(*it2);
    }

    void MultiStateEngine::addEffectContainer(EffectContainer* effect)
    {
        if (effect == NULL)
            return;
        effect->setLuaState(this->lua_, 'f' + multi_cast<std::string>(this->effectContainers_.size()));
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

    void MultiStateEngine::setDefEngSndNormal(const std::string &engineSound)
    {
        if( defEngineSndNormal_ )
            defEngineSndNormal_->setSource(engineSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& MultiStateEngine::getDefEngSndNormal()
    {
        if( defEngineSndNormal_ )
            return defEngineSndNormal_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void MultiStateEngine::setDefEngSndBoost(const std::string &engineSound)
    {
        if( defEngineSndBoost_ )
            defEngineSndBoost_->setSource(engineSound);
        else
            assert(0);
    }

    const std::string& MultiStateEngine::getDefEngSndBoost()
    {
        if( this->defEngineSndBoost_ )
            return defEngineSndBoost_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }
}
