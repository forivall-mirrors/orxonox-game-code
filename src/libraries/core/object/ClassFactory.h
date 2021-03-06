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

/**
    @file
    @ingroup Object Factory
    @brief Definition and implementation of the ClassFactory class

    The ClassFactory is able to create new objects of a specific class which creates objects.
*/

#ifndef _ClassFactory_H__
#define _ClassFactory_H__

#include "core/CorePrereqs.h"

#include <string>

#include "util/Output.h"

namespace orxonox
{
    // ###########################
    // ###       Factory       ###
    // ###########################
    /// Base-class of all factories.
    class _CoreExport Factory
    {
        public:
            virtual ~Factory() {}
            virtual Identifiable* fabricate(Context* context) = 0;
    };

    // ###############################
    // ###      ClassFactory       ###
    // ###############################
    /// The ClassFactory is the base-class of all class-spezific factories
    template <class T>
    class ClassFactory : public Factory
    {
    };

    // ###############################
    // ###   ClassFactoryNoArgs    ###
    // ###############################
    /// The ClassFactoryNoArgs is able to create new objects of a specific class that require no constructor arguments.
    template <class T>
    class ClassFactoryNoArgs : public ClassFactory<T>
    {
        public:
            /**
                @brief Creates and returns a new object of class T.
                @return The new object
            */
            inline Identifiable* fabricate(Context* context)
            {
                return static_cast<Identifiable*>(new T());
            }
    };

    // ###############################
    // ### ClassFactoryWithContext ###
    // ###############################
    /// The ClassFactoryWithContext is able to create new objects of a specific class that require a context as constructor argument
    template <class T>
    class ClassFactoryWithContext : public ClassFactory<T>
    {
        public:
            /**
                @brief Creates and returns a new object of class T.
                @return The new object
            */
            inline Identifiable* fabricate(Context* context)
            {
                return static_cast<Identifiable*>(new T(context));
            }
    };
}

#endif /* _ClassFactory_H__ */
