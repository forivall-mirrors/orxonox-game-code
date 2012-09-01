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

#ifndef _MultiStateEngine_H__
#define _MultiStateEngine_H__

#include "OrxonoxPrereqs.h"

#include <vector>
#include "Engine.h"

namespace orxonox
{
    class _OrxonoxExport MultiStateEngine : public Engine
    {
        public:
            enum EngineState
            {
                Idle    = 1,
                Normal  = 2,
                Brake   = 4,
                Boost   = 8
            };

            MultiStateEngine(BaseObject* creator);
            virtual ~MultiStateEngine();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void run(float dt);

            virtual void addToSpaceShip(SpaceShip* ship);

            void addEffectContainer(EffectContainer* effect);
            EffectContainer* getEffectContainer(unsigned int index) const;

            void setDefEngSndNormal(const std::string& engineSound);
            const std::string& getDefEngSndNormal();
            void setDefEngSndBoost(const std::string& engineSound);
            const std::string& getDefEngSndBoost();

        private:
            void registerVariables();

            int state_;
            int oldState_;
            LuaState* lua_;
            std::vector<EffectContainer*> effectContainers_;
            WorldSound* defEngineSndNormal_;
            WorldSound* defEngineSndBoost_;
    };
}

#endif /* _MultiStateEngine_H__ */
