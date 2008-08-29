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

#ifndef _Projectile_H__
#define _Projectile_H__

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "tools/Timer.h"
#undef SUPER_INTRUSIVE
#include "core/Super.h"

namespace orxonox
{
    class _OrxonoxExport Projectile : public WorldEntity
    {
        public:
            virtual ~Projectile();
            void setConfigValues();
            void speedChanged();
            void destroyObject();
            virtual void tick(float dt);
            virtual void testfunction();

            static float getSpeed()
                { return Projectile::speed_s; }

        protected:
            Projectile(SpaceShip* owner = 0);
            SpaceShip* owner_;

        private:
            std::string explosionTemplateName_;
            std::string smokeTemplateName_;
        protected:
            static float speed_s;
            float speed_;
        private:
            float lifetime_;
            float damage_;
            Timer<Projectile> destroyTimer_;
    };

    // Partially specialized template (templatehack is now specialized too)
    template <class T>
    struct SuperFunctionCondition<0, 0, T>
    {
        // Checks if class U isA baseclass and sets the functionpointer if the check returned true
        static void check()
        {
            std::cout << "check superfunction \"testfunction\" in " << ClassIdentifier<T>::getIdentifier()->getName() << std::endl;

            T* temp = 0;
            SuperFunctionCondition<0, 0, T>::apply(temp);

            std::cout << "done" << std::endl;

            // Calls the condition of the next super-function
            SuperFunctionCondition<0 + 1, 0, T>::check();
        }

        static void apply(void* temp)
        {
            std::cout << ClassIdentifier<T>::getIdentifier()->getName() << " is not a Projectile" << std::endl;
            // nop
        }

        static void apply(Projectile* temp)
        {
            std::cout << ClassIdentifier<T>::getIdentifier()->getName() << " is a Projectile" << std::endl;
            ClassIdentifier<T>* identifier = ClassIdentifier<T>::getIdentifier();

/*
            SuperFunctionCaller_testfunction* superFunctionCaller = 0;
            // Search for an existing caller within all direct children
            for (std::set<const Identifier*>::iterator it = identifier->getDirectChildrenIntern().begin(); it != identifier->getDirectChildrenIntern().end(); ++it)
                if (((ClassIdentifier<T>*)(*it))->superFunctionCaller_testfunction_)
                    superFunctionCaller = ((ClassIdentifier<T>*)(*it))->superFunctionCaller_testfunction_;
            // Check if we've found an existing caller - if not, create a new one
            if (!superFunctionCaller)
                superFunctionCaller = new SuperFunctionClassCaller_testfunction<T>;
*/
            // Iterate through all children and assign the caller
            for (std::set<const Identifier*>::iterator it = identifier->getDirectChildrenIntern().begin(); it != identifier->getDirectChildrenIntern().end(); ++it)
            {
                if (!((ClassIdentifier<T>*)(*it))->superFunctionCaller_testfunction_)
                {
                    std::cout << "adding functionpointer to " << ((ClassIdentifier<T>*)(*it))->getName() << std::endl;
//                    ((ClassIdentifier<T>*)(*it))->superFunctionCaller_testfunction_ = superFunctionCaller;
                    ((ClassIdentifier<T>*)(*it))->superFunctionCaller_testfunction_ = new SuperFunctionClassCaller_testfunction<T>;
                }
            }
        }
    };
}

#endif /* _Projectile_H__ */
