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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "EffectContainer.h"

extern "C" {
#include <lua.h>
}

#include "core/CoreIncludes.h"
#include "core/LuaState.h"
#include "core/XMLPort.h"
#include "worldentities/WorldEntity.h"

namespace orxonox
{
    CreateFactory(EffectContainer);

    EffectContainer::EffectContainer(Context* context)
        : BaseObject(context)
        , lua_(NULL)
    {
        RegisterObject(EffectContainer);
    }

    EffectContainer::~EffectContainer()
    {
    }

    void EffectContainer::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EffectContainer, XMLPort, xmlelement, mode);
        XMLPortParam(EffectContainer, "condition", setCondition, getCondition, xmlelement, mode);
        XMLPortObject(EffectContainer, WorldEntity, "", addEffect, getEffect, xmlelement, mode);
    }

    void EffectContainer::setCondition(const std::string& condition)
    {
        // TODO: Watch out for code injection!
        this->condition_ = condition;
        if (this->lua_)
        {
            this->lua_->doString(
                this->functionName_ + " = function() \n"
                "    return (" + condition + ") \n"
                "end"
            );
        }
    }

    void EffectContainer::setLuaState(LuaState* state, const std::string& functionName)
    {
        this->functionName_ = functionName;
        this->lua_ = state;
        this->setCondition(this->condition_);
    }

    void EffectContainer::addEffect(WorldEntity* effect)
    {
        this->effects_.push_back(effect);
    }

    WorldEntity* EffectContainer::getEffect(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::vector<WorldEntity*>::const_iterator it = this->effects_.begin(); it != this->effects_.end(); ++it)
            if (i == index)
                return (*it);
        return NULL;
    }

    void EffectContainer::updateCondition()
    {
        if (this->lua_)
        {
            lua_getglobal(this->lua_->getInternalLuaState(), this->functionName_.c_str());
            lua_call(this->lua_->getInternalLuaState(), 0, 1);
            bool result = (bool)lua_toboolean(this->lua_->getInternalLuaState(), -1);
            lua_pop(this->lua_->getInternalLuaState(), 1);
            for (std::vector<WorldEntity*>::const_iterator it = this->effects_.begin(); it != this->effects_.end(); ++it)
            {
                (*it)->setMainState(result);
            }
        }
    }
}
