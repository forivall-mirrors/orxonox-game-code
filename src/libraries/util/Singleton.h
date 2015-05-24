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
    @defgroup SingletonScope Singletons and Scope
    @ingroup Util
*/

/**
    @file
    @ingroup SingletonScope
    @brief Definition of the Singleton template that is used as base class for classes that allow only one instance.

    @anchor SingletonExample

    Classes that inherit from orxonox::Singleton follow the singleton pattern and thus
    allow only one instance of the class to exist. This istance is stored in a static
    variable called @c singletonPtr_s. orxonox::Singleton will access this variable, but
    it must be implemented in the deriving class.

    Example:
    @code
    class TestSingleton : public Singleton<TestSingleton>   // inherit from Singleton, pass the own class as template argument
    {
        friend class Singleton<TestSingleton>;              // friend declaration so Singleton can access singletonPtr_s

        public:
            TestSingleton();                                // public constructor because we may want to manage this singleton
                                                            //     with an orxonox::ScopedSingletonWrapper
            virtual ~TestSingleton();                       // public destructor

            void testFunction();                            // put your functions here

        private:
            int testValue_;                                 // put your variables here

            static TestSingleton* singletonPtr_s;           // static singleton instance pointer, used by the Singleton template
    };
    @endcode

    And don't forget to initialize the static singleton pointer in the source (*.cc) %file:
    @code
    TestSingleton* TestSingleton::singletonPtr_s = NULL;
    @endcode

    If a class inherits from orxonox::Singleton, it also inherits its functions. The most important
    function is orxonox::Singleton::getInstance() which returns a reference to the only instance
    of the singleton.

    Example:
    @code
    TestSingleton::TestSingleton()                          // implement the constructor
    {
        this->testValue_ = 15;
    }

    void TestSingleton::testFunction()                      // implement testFunction
    {
        orxout() << "My value is " << this->testValue_ << endl;
    }

    TestSingleton::getInstance().testFunction();            // prints "My value is 15"
    @endcode
*/

#ifndef __Util_Singleton_H__
#define __Util_Singleton_H__

#include "UtilPrereqs.h"

#include <cstring>
#include <typeinfo>

#include "OrxAssert.h"

namespace orxonox
{
    /**
    @brief
        Base for singleton classes.

        Usage:
        Inherit publicly from Singleton<MyClass> and provide access to MyClass::singletonPtr_s.
        This can easily be done with a friend declaration.

        See @ref SingletonExample "this example" for an exemplary implementation.
    */
    template <class T>
    class Singleton
    {
    public:
        //! Returns a reference to the singleton instance
        static T& getInstance()
        {
            OrxVerify(T::singletonPtr_s != NULL, "T=" << typeid(T).name());
            return *T::singletonPtr_s;
        }

        //! Tells whether the singleton has been created
        static bool exists()
        {
            return (T::singletonPtr_s != NULL);
        }

    protected:
        //! Constructor sets the singleton instance pointer
        Singleton()
        {
            OrxVerify(T::singletonPtr_s == NULL, "T=" << typeid(T).name());
            T::singletonPtr_s = static_cast<T*>(this);
        }

        //! Destructor resets the singleton instance pointer
        virtual ~Singleton()
        {
            OrxVerify(T::singletonPtr_s != NULL, "T=" << typeid(T).name());
            T::singletonPtr_s = NULL;
        }

    private:
        Singleton(const Singleton& rhs); //!< Don't use (undefined)
    };
}

#endif /* __Util_Singleton_H__ */
