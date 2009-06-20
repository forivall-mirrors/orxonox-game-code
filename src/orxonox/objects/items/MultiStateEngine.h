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

#ifndef _MultiStateEngine_H__
#define _MultiStateEngine_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include "Engine.h"

namespace orxonox
{
    class _OrxonoxExport MultiStateEngine : public Engine
    {
        public:
            MultiStateEngine(BaseObject* creator);
            virtual ~MultiStateEngine();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void tick(float dt);

            virtual void addToSpaceShip(SpaceShip* ship);

            void addActiveEffect(WorldEntity* effect);
            void addForwardEffect(WorldEntity* effect);
            void addBoostEffect(WorldEntity* effect);
            void addBrakeEffect(WorldEntity* effect);

            WorldEntity* getActiveEffect(unsigned int index) const;
            WorldEntity* getForwardEffect(unsigned int index) const;
            WorldEntity* getBoostEffect(unsigned int index) const;
            WorldEntity* getBrakeEffect(unsigned int index) const;

        private:
            unsigned char state_;
            std::list<WorldEntity*> activeEffects_;
            std::list<WorldEntity*> forwardEffects_;
            std::list<WorldEntity*> boostEffects_;
            std::list<WorldEntity*> brakeEffects_;
    };
}

#endif /* _MultiStateEngine_H__ */
