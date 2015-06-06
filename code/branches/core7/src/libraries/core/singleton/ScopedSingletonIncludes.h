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

/**
    @file
    @ingroup SingletonScope
    @brief Definition of the ManageScopedSingleton macro.
*/

#ifndef __ScopedSingletonIncludes_H__
#define __ScopedSingletonIncludes_H__

#include "core/CorePrereqs.h"

#include "ScopedSingletonWrapper.h"
#include "core/module/StaticallyInitializedInstance.h"

/**
    @brief Creates an orxonox::ScopedSingletonWrapper for an orxonox::Singleton and registers it with orxonox::ScopeManager.
    @param className The name of the singleton class
    @param scope The scope in which the singleton should exist
    @param allowedToFail If true, the singleton is allowed to fail and thus a try-catch block is used when creating the singleton.

    If this macro is called for a singleton, it is wrapped in a ScopedSingletonWrapper and registered with ScopeManager
    and will thus be created if its scope becomes active and destroyed if is deactivated.


    Usually a singleton gets created automatically when it is first used, but it will never
    be destroyed (unless the singleton explicitly deletes itself). To allow controlled
    construction and destruction, the singleton can be put within a virtual scope. This is
    done by registering the singleton class with orxonox::ScopeManager. To
    do so, the ManageScopedSingleton() macro has to be called:

    @code
    ManageScopedSingleton(TestSingleton, ScopeID::Graphics, false); // muste be called in a source (*.cc) file
    @endcode

    @b Important: If you call ManageScopedSingleton(), you don't have to initialize singletonPtr_s anymore,
    because that's already done by the macro.

    Now the singleton TestSingleton gets automatically created if the scope Graphics becomes
    active and also gets destroyed if the scope is deactivated.

    Note that not all singletons must register with a scope, but it's recommended.

*/
#define ManageScopedSingleton(className, scope, allowedToFail) \
    className* className::singletonPtr_s = NULL; \
    static ScopedSingletonWrapper& className##ScopedSingletonWrapper \
        = (new orxonox::SI_SSW(new ClassScopedSingletonWrapper<className, allowedToFail>(#className), scope))->getWrapper()

namespace orxonox
{
    class _CoreExport StaticallyInitializedScopedSingletonWrapper : public StaticallyInitializedInstance
    {
        public:
            StaticallyInitializedScopedSingletonWrapper(ScopedSingletonWrapper* wrapper, ScopeID::Value scope)
                : StaticallyInitializedInstance(StaticInitialization::SCOPED_SINGLETON_WRAPPER)
                , wrapper_(wrapper)
                , scope_(scope)
            {}
            ~StaticallyInitializedScopedSingletonWrapper() { delete wrapper_; }

            virtual void load();
            virtual void unload();

            inline ScopedSingletonWrapper& getWrapper()
                { return *this->wrapper_; }

        private:
            ScopedSingletonWrapper* wrapper_;
            ScopeID::Value scope_;
    };

    typedef StaticallyInitializedScopedSingletonWrapper SI_SSW;
}

#endif /* __ScopedSingletonIncludes_H__ */
