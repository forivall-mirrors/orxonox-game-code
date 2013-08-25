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

#ifndef _EffectContainer_H__
#define _EffectContainer_H__

#include "OrxonoxPrereqs.h"

#include <vector>
#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport EffectContainer : public BaseObject
    {
        public:
            EffectContainer(Context* context);
            virtual ~EffectContainer();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void setLuaState(LuaState* state, const std::string& functionName);

            void setCondition(const std::string& condition);
            inline const std::string& getCondition() const
                { return this->condition_; }

            void addEffect(WorldEntity* effect);
            WorldEntity* getEffect(unsigned int index) const;

            inline std::vector<WorldEntity*>::const_iterator getEffectsBegin()
                { return this->effects_.begin(); }
            inline std::vector<WorldEntity*>::const_iterator getEffectsEnd()
                { return this->effects_.end(); }

            void updateCondition();

        private:
            std::string               condition_;
            std::string               functionName_;
            LuaState*                 lua_;
            std::vector<WorldEntity*> effects_;
    };
}

#endif /* _EffectContainer_H__ */
