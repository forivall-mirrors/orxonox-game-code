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
    @defgroup SmartPtr SmartPtr<T> and WeakPtr<T>
    @ingroup Object
*/

/**
    @file
    @ingroup Object SmartPtr
    @brief Definition of SmartPtr<T>, wraps a pointer to an object and keeps it alive.

    @anchor SmartPtrExample

    orxonox::SmartPtr is an implementation of a smart pointer - it wraps a pointer to an
    object  and keeps this object alive until no SmartPtr points to this object anymore.
    In contrast to orxonox::SharedPtr, SmartPtr works only with classes that are derived
    from orxonox::Destroyable, because it's an intrusive implementation, meaning the
    reference counter is stored in the object itself.

    It's possible to use normal pointers and smart pointers to an object simultaneously.
    You don't have to use SmartPtr all the time, you can create a SmartPtr for an object
    at any time and also convert it back to a normal pointer if you like. This is possible
    because the reference counter is stored in the object itself and not in SmartPtr (in
    contrast to SharedPtr).

    @b Important: If you want to delete an object, you must not use @c delete @c object but
    rather @c object->destroy(). This function will check if there are smart pointers
    pointing to the object. If yes, the object will be kept alive until all smart pointes
    are destroyed. If no, the object is deleted instantly.

    If all smart pointers that point to an object are destroyed, but you never called
    @c object->destroy() before, the object will not be deleted! All a SmartPtr will do
    is to really just keep an object alive, but it will not delete it automatically
    unless you tried to destroy it before.

    Example:
    @code
    class MyClass                                           // class declaration
    {
        public:
            void setObject(OtherClass* object)              // passes a normal pointer which will be stored in a SmartPtr
                { this->object_ = object; }

            OtherClass* getObject() const                   // converts the SmartPtr to a normal pointer and returns it
                { return this->object_; }

        private:
            SmartPtr<OtherClass> object_;                   // a pointer to an instance of OtherClass is stored in a SmartPtr
    };
    @endcode
    In this example we assume that OtherClass is a child of Destroyable. We don't care
    about the inheritance of MyClass though.

    Now we create an instance of MyClass and assign a pointer to an instance of OtherClass:
    @code
    MyClass* myclass = new MyClass();                       // create an instance of MyClass
    OtherClass* object = new OtherClass();                  // create an instance of OtherClass
    myclass->setObject(object);                             // the object is now stored in a SmartPtr inside myclass

    object->destroy();                                      // we try to destroy object, but there's still a SmartPtr pointing at it.

    # object still exists at this point (because a SmartPtr points at it)

    delete myclass;                                         // now we delete myclass, which also destroys the SmartPtr

    # object doesn't exist anymore (because the SmartPtr is now destroyed)
    @endcode

    Now we look at the same example, but we first delete myclass, then destroy object:
    @code
    MyClass* myclass = new MyClass();                       // create an instance of MyClass
    OtherClass* object = new OtherClass();                  // create an instance of OtherClass
    myclass->setObject(object);                             // the object is now stored in a SmartPtr inside myclass

    delete myclass;                                         // we delete myclass, which also destroys the SmartPtr

    # object still exists at this point (because destroy() was not called yet)

    object->destroy();                                      // now we try to destroy object, which works instantly

    # object doesn't exist anymore (because we just destroyed it)
    @endcode

    Note that in any case @c object->destroy() has to be called to delete the object.
    However if a SmartPtr points at it, the destruction is delayed until all SmartPtr
    are destroyed.
*/

#ifndef _SmartPtr_H__
#define _SmartPtr_H__

#include "core/CorePrereqs.h"

#include <cassert>

#include "core/object/Destroyable.h"
#include "WeakPtr.h"

namespace orxonox
{
    /**
        @brief A smart pointer which wraps a pointer to an object and keeps this object alive as long as the smart pointer exists.

        @see See @ref SmartPtrExample "this description" for more information and an example.
    */
    template <class T>
    class SmartPtr
    {
        public:
            /// Constructor: Initializes the smart pointer with a null pointer.
            inline SmartPtr() : pointer_(0), base_(0)
            {
            }

            /// Constructor: Initializes the smart pointer with a pointer to an object. @param pointer The pointer @param bAddRef If true, the reference counter is increased. Don't set this to false unless you know exactly what you're doing! (for example to avoid circular references if the @c this pointer of the possessing object is stored)
            inline SmartPtr(T* pointer, bool bAddRef = true) : pointer_(pointer), base_(pointer)
            {
                if (this->base_ && bAddRef)
                    this->base_->incrementReferenceCount();
            }

            /// Copy-constructor
            inline SmartPtr(const SmartPtr& other) : pointer_(other.pointer_), base_(other.base_)
            {
                if (this->base_)
                    this->base_->incrementReferenceCount();
            }

            /// Copy-constructor for smart pointers to objects of another class.
            template <class O>
            inline SmartPtr(const SmartPtr<O>& other) : pointer_(other.get()), base_(other.base_)
            {
                if (this->base_)
                    this->base_->incrementReferenceCount();
            }

            /// Constructor: Initializes the smart pointer with the pointer that is stored in a WeakPtr.
            template <class O>
            inline SmartPtr(const WeakPtr<O>& other) : pointer_(other.get()), base_(other.getBase())
            {
                if (this->base_)
                    this->base_->incrementReferenceCount();
            }

            /// Destructor: Decrements the reference counter.
            inline ~SmartPtr()
            {
                if (this->base_)
                    this->base_->decrementReferenceCount();
            }

            /// Assigns a new pointer.
            inline SmartPtr& operator=(T* pointer)
            {
                SmartPtr(pointer).swap(*this);
                return *this;
            }

            /// Assigns the wrapped pointer of another SmartPtr.
            inline SmartPtr& operator=(const SmartPtr& other)
            {
                SmartPtr(other).swap(*this);
                return *this;
            }

            /// Assigns the wrapped pointer of a SmartPtr of another class
            template <class O>
            inline SmartPtr& operator=(const SmartPtr<O>& other)
            {
                SmartPtr(other).swap(*this);
                return *this;
            }

            /// Assigns the wrapped pointer of a WeakPtr.
            template <class O>
            inline SmartPtr& operator=(const WeakPtr<O>& other)
            {
                SmartPtr(other).swap(*this);
                return *this;
            }

            /// Returns the wrapped pointer as @c T*
            inline T* get() const
            {
                return this->pointer_;
            }

            /// Returns the wrapped pointer as @c Destroyable*
            inline Destroyable* getBase() const
            {
                return this->base_;
            }

            /// Implicitly converts the SmartPtr to a pointer of type @c T*
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

            /// Swaps the contents of two smart pointers.
            inline void swap(SmartPtr& other)
            {
                {
                    T* temp = this->pointer_;
                    this->pointer_ = other.pointer_;
                    other.pointer_ = temp;
                }
                {
                    Destroyable* temp = this->base_;
                    this->base_ = other.base_;
                    other.base_ = temp;
                }
            }

            /// Resets the smart pointer (equivalent to assigning a NULL pointer).
            inline void reset()
            {
                SmartPtr().swap(*this);
            }

        private:
            T* pointer_;            ///< The wrapped pointer to an object of type @a T
            Destroyable* base_;    ///< The wrapped pointer, casted up to Destroyable (this is needed because with just a T* pointer, SmartPtr couln't be used with forward declarations)
    };

    /// Swaps the contents of two smart pointers.
    template <class T>
    void swap(SmartPtr<T>& a, SmartPtr<T>& b)
    {
        a.swap(b);
    }

    /// Uses a static_cast to cast a pointer of type U* to a pointer of type T* and returns it in a new SmartPtr<T>.
    template <class T, class U>
    SmartPtr<T> static_pointer_cast(const SmartPtr<U>& p)
    {
        return static_cast<T*>(p.get());
    }

    /// Uses a const_cast to cast a pointer of type U* to a pointer of type T* and returns it in a new SmartPtr<T>.
    template <class T, class U>
    SmartPtr<T> const_pointer_cast(const SmartPtr<U>& p)
    {
        return const_cast<T*>(p.get());
    }

    /// Uses a dynamic_cast to cast a pointer of type U* to a pointer of type T* and returns it in a new SmartPtr<T>.
    template <class T, class U>
    SmartPtr<T> dynamic_pointer_cast(const SmartPtr<U>& p)
    {
        return orxonox_cast<T*>(p.get());
    }
}

#endif /* _SmartPtr_H__ */
