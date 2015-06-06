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
    @brief Definition of orxonox::ScopedSingletonWrapper and orxonox::ClassScopedSingletonWrapper.

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

namespace orxonox
{
    /**
        @brief Base class of ClassScopedSingletonWrapper.
    */
    class _CoreExport ScopedSingletonWrapper : public ScopeListener
    {
        public:
            /// Constructor: Initializes all the values
            ScopedSingletonWrapper(const std::string& className)
                : className_(className)
            { }
            virtual ~ScopedSingletonWrapper() { }

        protected:
            const std::string className_;   ///< The name of the scoped singleton class that is managed by this object
    };

    /**
        @anchor ClassScopedSingletonWrapper

        @brief Manages a scoped singleton
        @param T The managed singleton class
        @param allowedToFail If true, a specialization of this template is used, that uses try-catch blocks to handle possible failures.

        This class inherits from ScopeListener and is registered for a scope in ScopeManager and thus its functions
        activated() and deactivated() are called whenever the Scope changes its state.

        If the Scope is activated, a new instance of @a T (which must be a singleton) is created.
        If the Scope is deactivated, the singleton is destroyed.

        @see Singleton
    */
    template <class T, bool allowedToFail>
    class ClassScopedSingletonWrapper : public ScopedSingletonWrapper
    {
    public:
        ClassScopedSingletonWrapper(const std::string& className)
            : ScopedSingletonWrapper(className)
            , singletonPtr_(NULL)
        {
        }

        ~ClassScopedSingletonWrapper()
        {
            if (singletonPtr_)
                this->destroy(singletonPtr_);
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

        Because @a T could fail when being created, this partial spezialization of ClassScopedSingletonWrapper
        uses a try-catch block to handle exceptions.

        See @ref ClassScopedSingletonWrapper for a full documentation of the basis template.
    */
    template <class T>
    class ClassScopedSingletonWrapper<T, true> : public ScopedSingletonWrapper
    {
    public:
        ClassScopedSingletonWrapper(const std::string& className)
            : ScopedSingletonWrapper(className)
            , singletonPtr_(NULL)
        {
        }

        ~ClassScopedSingletonWrapper()
        {
            if (singletonPtr_)
                this->destroy(singletonPtr_);
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
