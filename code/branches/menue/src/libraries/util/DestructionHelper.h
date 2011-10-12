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

#ifndef __DestructionHelper_H__
#define __DestructionHelper_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    /** Deletes an object and resets the pointer
    @param object
        Pointer to an object. Handing over NULL is safe.
    */
    template <class T>
    void safeObjectDelete(T** object)
    {
        delete *object;
        *object = NULL;
    }

    /** Utility class that helps to create a special kind of destructor that
        also executes if the destruction fails.
        This helps a lot to improve exception handling. <br>
        <br>
        Example: <br>
        @code
        class MyClass
        {
        public:
            MyClass() : destructionHelper_(this)
            {
                // Do something that might throw
            }
            ~MyClass()
            {
                // Keep empty!
            }
            void destroy()
            {
                // Place your destruction code here instead
            }

        private:
            orxonox::DestructionHelper<MyClass> destructionHelper_;
        };
        @endcode
    */
    template <class T>
    class DestructionHelper
    {
    public:
        DestructionHelper(T* object)
            : object_(object)
        {
        }

        ~DestructionHelper()
        {
            object_->destroy();
        }

    private:
        DestructionHelper(const DestructionHelper&); //!< Don't use (undefined)

        T* object_;
    };
}

#endif /* __DestructionHelper_H__ */
