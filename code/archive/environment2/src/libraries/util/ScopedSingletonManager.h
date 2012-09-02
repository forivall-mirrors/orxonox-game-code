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
    @brief Definition of orxonox::ScopedSingletonManager, orxonox::ClassScopedSingletonManager, and the ManageScopedSingleton macro.

    ScopedSingletonManager is used to create and destroy Singletons that belong to
    a given Scope. For each one of these singletons, the macro ManageScopedSingleton()
    has to be called to register the singleton with orxonox::ScopedSingletonManager.

    See @ref SingletonExample "this code" for an example.

    @see orxonox::Singleton
    @see orxonox::Scope
*/

#ifndef __ScopedSingletonManager_H__
#define __ScopedSingletonManager_H__

#include "UtilPrereqs.h"

#include <cassert>
#include <map>
#include "util/Exception.h"
#include "util/Scope.h"
#include "util/Singleton.h"

/**
    @brief Registers an orxonox::Singleton with orxonox::ScopedSingletonManager.
    @param className The name of the singleton class
    @param scope The scope in which the singleton should exist
    @param allowedToFail If true, the singleton is allowed to fail and thus a try-catch block is used when creating the singleton.

    If this macro is called for a singleton, it is registered with ScopedSingletonManager
    and will thus be created if its scope becomes active and destroyed if is deactivated.
*/
#define ManageScopedSingleton(className, scope, allowedToFail) \
    className* className::singletonPtr_s = NULL; \
    static ClassScopedSingletonManager<className, scope, allowedToFail> className##ScopedSingletonManager(#className)

namespace orxonox
{
    class OrxonoxClass;

    /**
        @brief Base class of ClassScopedSingletonManager, implements some static functions
        used to dispatch calls to preUpdate and postUpdate to all instances of this class.
        It also keeps track of all existing ScopedSingletonManagers and stores them in a
        map, sorted by the scope they belong to.
    */
    class _UtilExport ScopedSingletonManager
    {
        public:
            /// Constructor: Initializes all the values
            ScopedSingletonManager(const std::string& className, ScopeID::Value scope)
                : className_(className)
                , scope_(scope)
            { }
            virtual ~ScopedSingletonManager() { }

            /// Adds a new instance of ScopedSingletonManager to the map.
            static void addManager(ScopedSingletonManager* manager);

            /// Calls preUpdate in all instances of ScopedSingletonManager that are registered in the map.
            template<ScopeID::Value scope>
            static void preUpdate(const Clock& time)
            {
                assert(Scope<scope>::isActive());
                for (ManagerMultiMap::iterator it = getManagersByScope().lower_bound(scope); it != getManagersByScope().upper_bound(scope); ++it)
                    it->second->preUpdate(time);
            }
            virtual void preUpdate(const Clock& time) = 0;

            /// Calls postUpdate in all instances of ScopedSingletonManager that are registered in the map.
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
            const std::string className_;   ///< The name of the scoped singleton class that is managed by this object
            const ScopeID::Value scope_;    ///< The scope of the singleton that is managed by this object
    };

    /**
        @anchor ClassScopedSingletonManager

        @brief Manages a scoped singleton for a given scope.
        @param T The managed singleton class
        @param scope The scope in which the singleton @a T should be active
        @param allowedToFail If true, a specialization of this template is used, that uses try-catch blocks to handle possible failures.

        This class inherits from ScopeListener for the given scope and thus its functions
        activated() and deactivated() are called whenever the Scope changes its state.

        If the Scope is activated, a new instance of @a T (which must be a singleton) is created.
        If the Scope is deactivated, the singleton is destroyed.

        @see Singleton
    */
    template <class T, ScopeID::Value scope, bool allowedToFail>
    class ClassScopedSingletonManager : public ScopedSingletonManager, public ScopeListener
    {
    public:
        //! Constructor: Initializes the singleton pointer and passes the scope to ScopedSingletonManager and ScopeListener
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

        //! Destroys the singleton instance - overloaded for OrxonoxClass, calls OrxonoxClass::destroy()
        void destroy(OrxonoxClass*)
        {
            singletonPtr_->destroy();
        }
        //! Destroys the singleton instance - overloaded for all other pointers, calls delete
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
        T* singletonPtr_;   ///< Unique instance of the singleton class @a T
    };

    /**
        @brief This class partially spezializes ClassScopedSingletonManager for classes @a T that are allowed to fail.
        @param T The managed singleton class
        @param scope The scope in which the singleton @a T should be active

        Because @a T could fail when being created, this partial spezialization of ClassScopedSingletonManager
        uses a try-catch block to handle exceptions.

        See @ref ClassScopedSingletonManager for a full documentation of the basis template.
    */
    template <class T, ScopeID::Value scope>
    class ClassScopedSingletonManager<T, scope, true> : public ScopedSingletonManager, public ScopeListener
    {
    public:
        //! Constructor: Initializes the singleton pointer and passes the scope to ScopedSingletonManager and ScopeListener
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

        //! Destroys the singleton instance - overloaded for OrxonoxClass, calls OrxonoxClass::destroy()
        void destroy(OrxonoxClass* ptr)
        {
            singletonPtr_->destroy();
        }
        //! Destroys the singleton instance - overloaded for void*, calls delete
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
        T* singletonPtr_;   ///< Unique instance of the singleton class @a T
    };
}

#endif /* __ScopedSingletonManager_H__ */
