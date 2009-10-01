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

#ifndef __Util_ScopedSingleton_H__
#define __Util_ScopedSingleton_H__

#include "UtilPrereqs.h"
#include <cassert>

#include "Scope.h"

namespace orxonox
{
    class Clock;
    /**
    @brief
        Base for scoped singleton classes.
        A Scoped singleton creates itself if the scope is active and getInstance() is called.
        Destroys itself if the scope is deactivated.

        Usage:
        Inherit publicly from ScopedSingleton<MyClass, scope> and provide access to
        MyClass::singletonPtr_s.
        This can easily be done with a friend declaration.

        See @ref UtilPrereqs.h for a list of scopes (ScopeID::Value).
    */
    template <class T, ScopeID::Value scope>
    class ScopedSingleton : public ScopeListener
    {
        public:
            //! Returns a reference to the singleton instance
            static T& getInstance()
            {
                assert(Scope<scope>::isActive());

                if (!T::singletonPtr_s)
                    T::singletonPtr_s = new T();

                return *T::singletonPtr_s;
            }

            //! Update method for singletons like the ingame console
            virtual void updated(const Clock& time) { static_cast<T*>(this)->update(time); }
            //! Empty update method for the static polymorphism
            void update(const Clock& time) { }

        protected:
            //! Constructor sets the singleton instance pointer
            ScopedSingleton() : ScopeListener(scope)
            {
                assert(T::singletonPtr_s == 0);
                T::singletonPtr_s = static_cast<T*>(this);
            }

            //! Constructor resets the singleton instance pointer
            ~ScopedSingleton()
            {
                assert(T::singletonPtr_s != 0);
                T::singletonPtr_s = 0;
            }

        private:
            //! Called if the Scope of this Singleton gets active (no instance should be active then)
            void activated()
            {
                // The ScopedSingleton shouldn't be active bevor the scope is activated -> always assertion failed
                assert(false);
            }

            //! Called if the Scope of this Singleton gets deactivated (destroys the instance)
            void deactivated()
            {
                if (T::singletonPtr_s)
                {
                    T::singletonPtr_s->destroy();
                    T::singletonPtr_s = 0;
                }
            }
    };
}

#endif /* __Util_ScopedSingleton_H__ */
