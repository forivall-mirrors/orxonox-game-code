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

#ifndef __Util_Singleton_H__
#define __Util_Singleton_H__

#include "UtilPrereqs.h"
#include <cassert>

namespace orxonox
{
    /**
    @brief
        Base for singleton classes.

        Usage:
        Inherit publicly from Singleton<MyClass> and provide access to
        MyClass::singletonPtr_s.
        This can easily be done with a friend declaration.
    */
    template <class T>
    class Singleton
    {
    public:
        //! Returns a reference to the singleton instance
        static T& getInstance()
        {
            assert(T::singletonPtr_s != 0);
            return *T::singletonPtr_s;
        }

        //! Update method called by ClassSingletonManager (if used)
        void preUpdateSingleton(const Clock& time) { static_cast<T*>(T::singletonPtr_s)->preUpdate(time); }
        //! Empty update method for the static polymorphism
        void preUpdate(const Clock& time) { }
        //! Update method called by ClassSingletonManager (if used)
        void postUpdateSingleton(const Clock& time) { static_cast<T*>(T::singletonPtr_s)->postUpdate(time); }
        //! Empty update method for the static polymorphism
        void postUpdate(const Clock& time) { }

    protected:
        //! Constructor sets the singleton instance pointer
        Singleton()
        {
            assert(T::singletonPtr_s == 0);
            T::singletonPtr_s = static_cast<T*>(this);
        }

        //! Constructor resets the singleton instance pointer
        ~Singleton()
        {
            assert(T::singletonPtr_s != 0);
            T::singletonPtr_s = 0;
        }

    private:
        Singleton(const Singleton& rhs); //!< Don't use (undefined)
    };
}

#endif /* __Util_Singleton_H__ */
