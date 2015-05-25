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
 *      Oliver Scheuss
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#ifndef _NetworkFunctionIncludes_H__
#define _NetworkFunctionIncludes_H__

#include "NetworkPrereqs.h"

#include <boost/preprocessor/cat.hpp>
#include <boost/static_assert.hpp>

#include "NetworkFunction.h"

namespace orxonox
{
    #define registerStaticNetworkFunction( functionPointer ) \
        static void* BOOST_PP_CAT( NETWORK_FUNCTION_, __UNIQUE_NUMBER__ ) = registerStaticNetworkFunctionFct( functionPointer, #functionPointer );

    #define registerMemberNetworkFunction( class, function ) \
        static void* BOOST_PP_CAT( NETWORK_FUNCTION_##class, __UNIQUE_NUMBER__ ) = registerMemberNetworkFunctionFct<class>( &class::function, #class "_" #function);

    // call it with functionPointer, clientID, args
    #define callStaticNetworkFunction( functionPointer, ...) \
        { \
            NetworkFunctionPointer p1; \
            copyPtr( functionPointer, p1 ); \
            FunctionCallManager::addCallStatic(NetworkFunctionStatic::getFunction(p1)->getNetworkID(), __VA_ARGS__); \
        }

    // call it with class, function, objectID, clientID, args
    #define callMemberNetworkFunction( class, function, objectID, ...) \
        { \
            NetworkFunctionPointer p1; \
            copyPtr( &class::function, p1 ); \
            FunctionCallManager::addCallMember(NetworkMemberFunctionBase::getFunction(p1)->getNetworkID(), objectID, __VA_ARGS__); \
        }

    template<class T> inline void* registerStaticNetworkFunctionFct( T ptr, const std::string& name )
    {
        BOOST_STATIC_ASSERT( sizeof(T)<=sizeof(NetworkFunctionPointer) ); // if this fails your compiler uses bigger pointers for static functions than defined above
        NetworkFunctionPointer destptr;
        copyPtr( ptr, destptr );
        new NetworkFunctionStatic( createFunctor(ptr), name, destptr );
        return 0;
    }

    template<class T, class PT> inline void* registerMemberNetworkFunctionFct( PT ptr, const std::string& name )
    {
        BOOST_STATIC_ASSERT( sizeof(PT)<=sizeof(NetworkFunctionPointer) ); // if this fails your compiler uses bigger pointers for a specific kind of member functions than defined above
        NetworkFunctionPointer destptr;
        copyPtr( ptr, destptr );
        new NetworkMemberFunction<T>( createFunctor(ptr), name, destptr );
        return 0;
    }

}

#endif /* _NetworkFunctionIncludes_H__ */
