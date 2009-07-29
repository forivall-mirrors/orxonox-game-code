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

namespace orxonox
{
    /**
    @brief
        Base for singleton classes.

        Usage:
        Inherit publicly from Singleton<MyClass> and provide access to
        MyClass::singletonRef_s.
        This can be done with a friend declaration.
    */
    template <class T>
    class Singleton
    {
    public:
        //! Returns a reference to the singleton instance
        static T& getInstance()
        {
            assert(T::singletonRef_s != NULL);
            return *T::singletonRef_s;
        }

    protected:
        // Constructor sets the singleton instance pointer
        Singleton()
        {
            assert(T::singletonRef_s == NULL);
            T::singletonRef_s = static_cast<T*>(this);
        }
        // Constructor resets the singleton instance pointer
        ~Singleton()
        {
            assert(T::singletonRef_s != NULL);
            T::singletonRef_s = NULL;
        }

    private:
        Singleton(const Singleton& rhs); //!< Don't use (undefined)
    };
}

#endif /* __Util_Singleton_H__ */
