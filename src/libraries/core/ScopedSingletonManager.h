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

#ifndef __ScopedSingletonManager_H__
#define __ScopedSingletonManager_H__

#include "CorePrereqs.h"

#include <cassert>
#include <map>
#include "util/Exception.h"
#include "util/Scope.h"
#include "util/Singleton.h"

#define ManageScopedSingleton(className, scope, allowedToFail) \
    className* className::singletonPtr_s = NULL; \
    static ClassScopedSingletonManager<className, scope, allowedToFail> className##ScopedSingletonManager(#className)

namespace orxonox
{
    class _CoreExport ScopedSingletonManager
    {
        public:
            ScopedSingletonManager(const std::string& className, ScopeID::Value scope)
                : className_(className)
                , scope_(scope)
            { }
            virtual ~ScopedSingletonManager() { }
            static void addManager(ScopedSingletonManager* manager);

            template<ScopeID::Value scope>
            static void preUpdate(const Clock& time)
            {
                assert(Scope<scope>::isActive());
                for (ManagerMultiMap::iterator it = getManagersByScope().lower_bound(scope); it != getManagersByScope().upper_bound(scope); ++it)
                    it->second->preUpdate(time);
            }
            virtual void preUpdate(const Clock& time) = 0;
            template<ScopeID::Value scope>
            static void postUpdate(const Clock& time)
            {
                assert(Scope<scope>::isActive());
                for (ManagerMultiMap::iterator it = getManagersByScope().lower_bound(scope); it != getManagersByScope().upper_bound(scope); ++it)
                    it->second->postUpdate(time);
            }
            virtual void postUpdate(const Clock& time) = 0;

            static std::map<std::string, ScopedSingletonManager*>& getManagers();
            typedef std::multimap<ScopeID::Value, ScopedSingletonManager*> ManagerMultiMap;
            static ManagerMultiMap& getManagersByScope();

        protected:
            const std::string className_;
            const ScopeID::Value scope_;
    };

    template <class T, ScopeID::Value scope, bool allowedToFail>
    class ClassScopedSingletonManager : public ScopedSingletonManager, public ScopeListener
    {
    public:
        ClassScopedSingletonManager(const std::string& className)
            : ScopedSingletonManager(className, scope)
            , ScopeListener(scope)
            , singletonPtr_(NULL)
        {
            ScopedSingletonManager::addManager(this);
        }

        ~ClassScopedSingletonManager()
        {
        }

        //! Called if the Scope of the Singleton gets active (creates the instance)
        void activated()
        {
            assert(singletonPtr_ == NULL);
            singletonPtr_ = new T();
        }

        //! Called if the Scope of this Singleton gets deactivated (destroys the instance)
        void deactivated()
        {
            assert(singletonPtr_ != NULL);
            this->destroy(singletonPtr_);
            singletonPtr_ = NULL;
        }

        void destroy(OrxonoxClass*)
        {
            singletonPtr_->destroy();
        }
        void destroy(void*)
        {
            delete singletonPtr_;
        }

        //! Called every frame by the ScopedSingletonManager
        void preUpdate(const Clock& time)
        {
            assert(Scope<scope>::isActive());
            // assuming T inherits Singleton<T>
            singletonPtr_->preUpdateSingleton(time);
        }

        //! Called every frame by the ScopedSingletonManager
        void postUpdate(const Clock& time)
        {
            assert(Scope<scope>::isActive());
            // assuming T inherits Singleton<T>
            singletonPtr_->postUpdateSingleton(time);
        }

    private:
        T* singletonPtr_;
    };

    template <class T, ScopeID::Value scope>
    class ClassScopedSingletonManager<T, scope, true> : public ScopedSingletonManager, public ScopeListener
    {
    public:
        ClassScopedSingletonManager(const std::string& className)
            : ScopedSingletonManager(className, scope)
            , ScopeListener(scope)
            , singletonPtr_(NULL)
        {
            ScopedSingletonManager::addManager(this);
        }

        ~ClassScopedSingletonManager()
        {
        }

        //! Called if the Scope of the Singleton gets active (creates the instance)
        void activated()
        {
            assert(singletonPtr_ == NULL);
            try
                { singletonPtr_ = new T(); }
            catch (const InitialisationAbortedException& ex)
                { COUT(3) << ex.getDescription() << std::endl; }
            catch (...)
                { COUT(1) << "Singleton creation failed: " << Exception::handleMessage() << std::endl; }
        }

        //! Called if the Scope of this Singleton gets deactivated (destroys the instance)
        void deactivated()
        {
            if (singletonPtr_ != NULL)
            {
                this->destroy(singletonPtr_);
                singletonPtr_ = NULL;
            }
        }

        void destroy(OrxonoxClass* ptr)
        {
            singletonPtr_->destroy();
        }
        void destroy(void* ptr)
        {
            delete singletonPtr_;
        }

        //! Called every frame by the ScopedSingletonManager
        void preUpdate(const Clock& time)
        {
            assert(Scope<scope>::isActive());
            // assuming T inherits Singleton<T>
            if (singletonPtr_ != NULL)
                singletonPtr_->preUpdateSingleton(time);
        }

        //! Called every frame by the ScopedSingletonManager
        void postUpdate(const Clock& time)
        {
            assert(Scope<scope>::isActive());
            // assuming T inherits Singleton<T>
            if (singletonPtr_ != NULL)
                singletonPtr_->postUpdateSingleton(time);
        }

    private:
        T* singletonPtr_;
    };
}

#endif /* __ScopedSingletonManager_H__ */
