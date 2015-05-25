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
#include "core/module/StaticallyInitializedInstance.h"

#define registerStaticNetworkFunction( functionPointer ) \
    static orxonox::NetworkFunctionBase& BOOST_PP_CAT( NETWORK_FUNCTION_, __UNIQUE_NUMBER__ ) \
        = (new orxonox::SI_NF(orxonox::registerStaticNetworkFunctionFct( functionPointer, #functionPointer )))->getFunction()

#define registerMemberNetworkFunction( class, function ) \
    static orxonox::NetworkFunctionBase& BOOST_PP_CAT( NETWORK_FUNCTION_##class, __UNIQUE_NUMBER__ ) \
        = (new orxonox::SI_NF(orxonox::registerMemberNetworkFunctionFct<class>( &class::function, #class "_" #function)))->getFunction()

namespace orxonox
{
    class _NetworkExport StaticallyInitializedNetworkFunction : public StaticallyInitializedInstance
    {
        public:
            StaticallyInitializedNetworkFunction(NetworkFunctionBase* function) : function_(function) {}

            virtual void load();
            virtual void unload();

            inline NetworkFunctionBase& getFunction()
                { return *this->function_; }

        private:
            NetworkFunctionBase* function_;
    };

    typedef StaticallyInitializedNetworkFunction SI_NF;

    template<class PT>
    inline NetworkFunctionBase* registerStaticNetworkFunctionFct(PT ptr, const std::string& name)
    {
        BOOST_STATIC_ASSERT(sizeof(PT) <= sizeof(NetworkFunctionPointer)); // if this fails your compiler uses bigger pointers for static functions than defined above
        NetworkFunctionPointer destptr;
        copyPtr(ptr, destptr);
        return new NetworkFunctionStatic(createFunctor(ptr), name, destptr);
    }

    template<class T, class PT>
    inline NetworkFunctionBase* registerMemberNetworkFunctionFct(PT ptr, const std::string& name)
    {
        BOOST_STATIC_ASSERT(sizeof(PT) <= sizeof(NetworkFunctionPointer)); // if this fails your compiler uses bigger pointers for a specific kind of member functions than defined above
        NetworkFunctionPointer destptr;
        copyPtr(ptr, destptr);
        return new NetworkMemberFunction<T>(createFunctor(ptr), name, destptr);
    }

    _NetworkExport uint32_t getNetworkIdForPointer(const NetworkFunctionPointer& pointer);

    // call it with functionPointer, clientID, args
    template<class PT>
    void callStaticNetworkFunction(PT ptr, uint32_t clientID, const MultiType& mt1 = MultiType::Null, const MultiType& mt2 = MultiType::Null, const MultiType& mt3 = MultiType::Null, const MultiType& mt4 = MultiType::Null, const MultiType& mt5 = MultiType::Null)
    {
        NetworkFunctionPointer destptr;
        copyPtr(ptr, destptr);
        FunctionCallManager::addCall(getNetworkIdForPointer(destptr), OBJECTID_UNKNOWN, clientID, mt1, mt2, mt3, mt4, mt5);
    }

    // call it with class::function, objectID, clientID, args
    template<class PT>
    void callMemberNetworkFunction(PT ptr, uint32_t objectID, uint32_t clientID, const MultiType& mt1 = MultiType::Null, const MultiType& mt2 = MultiType::Null, const MultiType& mt3 = MultiType::Null, const MultiType& mt4 = MultiType::Null, const MultiType& mt5 = MultiType::Null)
    {
        NetworkFunctionPointer destptr;
        copyPtr(ptr, destptr);
        FunctionCallManager::addCall(getNetworkIdForPointer(destptr), objectID, clientID, mt1, mt2, mt3, mt4, mt5);
    }

    template<class PT>
    inline void copyPtr(PT ptr, NetworkFunctionPointer& destptr)
    {
        if (sizeof(NetworkFunctionPointer) - sizeof(PT) > 0)
            memset((uint8_t*)&destptr + sizeof(PT), 0, sizeof(NetworkFunctionPointer) - sizeof(PT));
        PT p2 = ptr;
        memcpy(&destptr, &p2, sizeof(PT));
    }
}

#endif /* _NetworkFunctionIncludes_H__ */
