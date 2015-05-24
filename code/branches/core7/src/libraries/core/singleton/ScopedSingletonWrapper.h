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
    @brief Definition of orxonox::ScopedSingletonWrapper, orxonox::ClassScopedSingletonWrapper, and the ManageScopedSingleton macro.

    ScopedSingletonWrapper is used to create and destroy Singletons that belong to
    a given Scope. For each one of these singletons, the macro ManageScopedSingleton()
    has to be called to register the wrapper with orxonox::ScopeManager.

    See @ref SingletonExample "this code" for an example.

    @see orxonox::Singleton
    @see orxonox::Scope
*/

#ifndef __ScopedSingletonWrapper_H__
#define __ScopedSingletonWrapper_H__

#include "core/CorePrereqs.h"

#include <cassert>
#include "util/Exception.h"
#include "util/Singleton.h"
#include "Scope.h"

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
    static ClassScopedSingletonWrapper<className, scope, allowedToFail> className##ScopedSingletonWrapper(#className)

namespace orxonox
{
    /**
        @brief Base class of ClassScopedSingletonWrapper.
    */
    class _CoreExport ScopedSingletonWrapper
    {
        public:
            /// Constructor: Initializes all the values
            ScopedSingletonWrapper(const std::string& className, ScopeID::Value scope)
                : className_(className)
                , scope_(scope)
            { }
            virtual ~ScopedSingletonWrapper() { }

        protected:
            const std::string className_;   ///< The name of the scoped singleton class that is managed by this object
            const ScopeID::Value scope_;    ///< The scope of the singleton that is managed by this object
    };

    /**
        @anchor ClassScopedSingletonWrapper

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
    class ClassScopedSingletonWrapper : public ScopedSingletonWrapper, public ScopeListener
    {
    public:
        //! Constructor: Initializes the singleton pointer and passes the scope to ScopedSingletonWrapper and ScopeListener
        ClassScopedSingletonWrapper(const std::string& className)
            : ScopedSingletonWrapper(className, scope)
            , ScopeListener(scope)
            , singletonPtr_(NULL)
        {
        }

        ~ClassScopedSingletonWrapper()
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

        //! Destroys the singleton instance - overloaded for Destroyable, calls Destroyable::destroy()
        void destroy(Destroyable*)
        {
            singletonPtr_->destroy();
        }
        //! Destroys the singleton instance - overloaded for all other pointers, calls delete
        void destroy(void*)
        {
            delete singletonPtr_;
        }

    private:
        T* singletonPtr_;   ///< Unique instance of the singleton class @a T
    };

    /**
        @brief This class partially spezializes ClassScopedSingletonWrapper for classes @a T that are allowed to fail.
        @param T The managed singleton class
        @param scope The scope in which the singleton @a T should be active

        Because @a T could fail when being created, this partial spezialization of ClassScopedSingletonWrapper
        uses a try-catch block to handle exceptions.

        See @ref ClassScopedSingletonWrapper for a full documentation of the basis template.
    */
    template <class T, ScopeID::Value scope>
    class ClassScopedSingletonWrapper<T, scope, true> : public ScopedSingletonWrapper, public ScopeListener
    {
    public:
        //! Constructor: Initializes the singleton pointer and passes the scope to ScopedSingletonWrapper and ScopeListener
        ClassScopedSingletonWrapper(const std::string& className)
            : ScopedSingletonWrapper(className, scope)
            , ScopeListener(scope)
            , singletonPtr_(NULL)
        {
        }

        ~ClassScopedSingletonWrapper()
        {
        }

        //! Called if the Scope of the Singleton gets active (creates the instance)
        void activated()
        {
            assert(singletonPtr_ == NULL);
            try
                { singletonPtr_ = new T(); }
            catch (const InitialisationAbortedException& ex)
                { orxout(internal_error) << ex.getDescription() << endl; }
            catch (...)
                { orxout(internal_error) << "Singleton creation failed: " << Exception::handleMessage() << endl; }
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

        //! Destroys the singleton instance - overloaded for Destroyable, calls Destroyable::destroy()
        void destroy(Destroyable*)
        {
            singletonPtr_->destroy();
        }
        //! Destroys the singleton instance - overloaded for void*, calls delete
        void destroy(void*)
        {
            delete singletonPtr_;
        }

    private:
        T* singletonPtr_;   ///< Unique instance of the singleton class @a T
    };
}

#endif /* __ScopedSingletonWrapper_H__ */
