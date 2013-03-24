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

// Inspired by boost::intrusive_ptr by Peter Dimov

/**
    @file
    @ingroup Object SmartPtr
    @brief Definition of WeakPtr<T>, wraps a pointer to an object.

    @anchor WeakPtrExample

    A WeakPtr wraps a pointer to an object. If the object gets deleted, the WeakPtr becomes
    NULL. This can be used to store pointers to objects without knowing when they will be
    destroyed.

    WeakPtr works only with objects that are derived from orxonox::OrxonoxClass, because
    WeakPtr is intrusive and registers itself in the stored object, to get a notification if
    the object is being deleted.

    Example:
    @code
    MyClass* object = new MyClass();                    // create an instance of MyClass

    WeakPtr<MyClass> pointer = object;                  // create a WeakPtr and assign the object

    if (pointer)                                        // checks if pointer is not NULL (which is true)
        pointer->someFunction();                        // calls MyClass::someFunction()

    object->destroy();                                  // calls destroy() which deletes the object

    if (pointer)                                        // checks if pointer is not NULL (which is now false)
        pointer->someFunction();                        // this will not be executed
    @endcode
    In this example we assumed that MyClass is derived of OrxonoxClass (otherwise it couldn't
    be used with a WeakPtr).

    A callback can be registerd with the WeakPtr that will be called if the object gets deleted.
    @code
    void myCallback()                                   // definition of the callback function
    {
        orxout() << "Object destroyed" << endl;
    }

    MyClass* object = new MyClass();                    // create an instance of MyClass

    WeakPtr<MyClass> pointer = object;                  // create a WeakPtr and assign the object

    pointer.setCallback(createFunctor(&myCallback));    // defines a callback

    object->destroy();                                  // calls destroy() which deletes the object. prints "Object destroyed" to the console
    @endcode
*/

#ifndef _WeakPtr_H__
#define _WeakPtr_H__

#include "core/CorePrereqs.h"

#include <cassert>

#include "core/class/OrxonoxClass.h"
#include "core/command/Functor.h"

namespace orxonox
{
    /**
        @brief WeakPtr wraps a pointer to an object, which becomes NULL if the object is deleted.

        @see See @ref WeakPtrExample "this description" for more information and an example.
    */
    template <class T>
    class WeakPtr : public DestructionListener
    {
        public:
            /// Constructor: Initializes the weak pointer with a null pointer.
            inline WeakPtr() : pointer_(0), base_(0), callback_(0)
            {
            }

            /// Constructor: Used to explicitly initialize the weak pointer with a null pointer
            inline WeakPtr(int) : pointer_(0), base_(0), callback_(0)
            {
            }

            /// Constructor: Initializes the weak pointer with a pointer to an object.
            inline WeakPtr(T* pointer) : pointer_(pointer), base_(pointer), callback_(0)
            {
                this->registerAsDestructionListener(this->base_);
            }

            /// Copy-constructor
            inline WeakPtr(const WeakPtr& other) : pointer_(other.pointer_), base_(other.base_), callback_(0)
            {
                this->registerAsDestructionListener(this->base_);
            }

            /// Copy-constructor for weak pointers to objects of another class.
            template <class O>
            inline WeakPtr(const WeakPtr<O>& other) : pointer_(other.get()), base_(other.base_), callback_(0)
            {
                this->registerAsDestructionListener(this->base_);
            }

            /// Destructor
            inline virtual ~WeakPtr()
            {
                this->unregisterAsDestructionListener(this->base_);
            }

            /// Used to assign a null pointer.
            inline WeakPtr& operator=(int)
            {
                WeakPtr(0).swap(*this);
                return *this;
            }

            /// Assigns a new pointer.
            inline WeakPtr& operator=(T* pointer)
            {
                WeakPtr(pointer).swap(*this);
                return *this;
            }

            /// Assigns the wrapped pointer of another WeakPtr.
            inline WeakPtr& operator=(const WeakPtr& other)
            {
                WeakPtr(other).swap(*this);
                return *this;
            }

            /// Assigns the wrapped pointer of a WeakPtr of another class
            template <class O>
            inline WeakPtr& operator=(const WeakPtr<O>& other)
            {
                WeakPtr(other).swap(*this);
                return *this;
            }

            /// Returns the wrapped pointer as @c T*
            inline T* get() const
            {
                return this->pointer_;
            }

            /// Returns the wrapped pointer as @c OrxonoxClass*
            inline OrxonoxClass* getBase() const
            {
                return this->base_;
            }

            /// Implicitly converts the WeakPtr to a pointer of type @c T*
            inline operator T*() const
            {
                return this->pointer_;
            }

            /// Overloaded operator, returns a pointer to the stored object.
            inline T* operator->() const
            {
                assert(this->pointer_ != 0);
                return this->pointer_;
            }

            /// Overloaded operator, returns a reference to the stored object.
            inline T& operator*() const
            {
                assert(this->pointer_ != 0);
                return *this->pointer_;
            }

            /// Returns true if the wrapped pointer is NULL.
            inline bool operator!() const
            {
                return (this->pointer_ == 0);
            }

            /// Swaps the contents of two weak pointers.
            inline void swap(WeakPtr& other)
            {
                this->unregisterAsDestructionListener(this->base_);
                other.unregisterAsDestructionListener(other.base_);

                {
                    T* temp = this->pointer_;
                    this->pointer_ = other.pointer_;
                    other.pointer_ = temp;
                }
                {
                    OrxonoxClass* temp = this->base_;
                    this->base_ = other.base_;
                    other.base_ = temp;
                }

                this->registerAsDestructionListener(this->base_);
                other.registerAsDestructionListener(other.base_);
            }

            /// Resets the weak pointer (equivalent to assigning a NULL pointer).
            inline void reset()
            {
                WeakPtr().swap(*this);
            }

            /// Registers a callback that will be executed if the stored object is destroyed.
            inline void setCallback(const FunctorPtr& callback)
            {
                this->callback_ = callback;
            }

            /// Returns the registered callback.
            inline const FunctorPtr& getCallback() const
            {
                return this->callback_;
            }

        private:
            /// Will be called by OrxonoxClass::~OrxonoxClass() if the stored object is deleted. Resets the wrapped pointer and executes the callback.
            inline void objectDeleted()
            {
                this->base_ = 0;
                this->pointer_ = 0;
                if (this->callback_)
                    (*this->callback_)();
            }

            T* pointer_;            ///< The wrapped pointer to an object of type @a T
            OrxonoxClass* base_;    ///< The wrapped pointer, casted up to OrxonoxClass (this is needed because with just a T* pointer, WeakPtr couln't be used with forward declarations)
            FunctorPtr callback_;   ///< This callback will be executed if the stored object is deleted
    };

    /// Swaps the contents of two weak pointers.
    template <class T>
    void swap(WeakPtr<T>& a, WeakPtr<T>& b)
    {
        a.swap(b);
    }

    /// Uses a static_cast to cast a pointer of type U* to a pointer of type T* and returns it in a new WeakPtr<T>.
    template <class T, class U>
    WeakPtr<T> static_pointer_cast(const WeakPtr<U>& p)
    {
        return static_cast<T*>(p.get());
    }

    /// Uses a const_cast to cast a pointer of type U* to a pointer of type T* and returns it in a new WeakPtr<T>.
    template <class T, class U>
    WeakPtr<T> const_pointer_cast(const WeakPtr<U>& p)
    {
        return const_cast<T*>(p.get());
    }

    /// Uses a dynamic_cast to cast a pointer of type U* to a pointer of type T* and returns it in a new WeakPtr<T>.
    template <class T, class U>
    WeakPtr<T> dynamic_pointer_cast(const WeakPtr<U>& p)
    {
        return orxonox_cast<T*>(p.get());
    }
}

#endif /* _WeakPtr_H__ */
