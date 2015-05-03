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

/**
    @defgroup SharedPtr SharedPtr<T>
    @ingroup Util
*/

/**
    @file
    @ingroup SharedPtr
    @brief Definition of the SharedPtr template that is used to manage pointers.

    @anchor SharedPtrExample

    The orxonox::SharedPtr template can be used to manage a pointer to an object
    that was created with new. The SharedPtr acts like the pointer itself, but it
    keeps track of the number of references to it. If all references are removed,
    SharedPtr deletes the managed object automatically.

    Example:

    Classic implementation using new and delete:
    @code
    void someFunction()
    {
        MyClass* object = new MyClass();            // Create a new instance of MyClass

        object->myFunction();                       // Calls MyClass::myFunction()

        delete object;                              // Delete the object at the end of the scope
    }
    @endcode

    The same function using SharedPtr:
    @code
    void someFunction()
    {
        SharedPtr<MyClass> object = new MyClass();  // Create a new instance of MyClass and store its pointer in a SharedPtr

        object->myFunction();                       // Calls MyClass::myFunction()

    }                                               // At the end of the scope, the SharedPtr is destroyed. Because no other SharedPtrs
                                                    // point at the object, the object itself is also destroyed automatically
    @endcode

    This is especially handy if you do not know what will happen with an object that was
    created with new, for example if you pass it to another object. If multiple instances
    share a pointer to the same object, none of these instances can delete the object
    without interfering with the other instances. But if none of the instances destroy the
    object, it will never be destroyed and results in a memory leak. With a SharedPtr
    however you don't have to think about destroying the object, because the SharedPtr
    itself keeps track of the references.

    Example:

    Classic implementation using new and delete:
    @code
    class OtherClass                                    // Declaration of some class
    {
        public:
            OtherClass(MyClass* object)                 // Constructor
            {
                this->object_ = object;                 // Assigns the pointer to the member variable object_
            }

            ~OtherClass()                               // Destructor
            {
                ???                                     // What to do with object_?
            }

        private:
            MyClass* object_;                           // A pointer to the object
    };

    void someFunction()
    {
        MyClass* object = new MyClass();                // Create a new instance of MyClass

        OtherClass* other1 = new OtherClass(object);    // Create an instance of OtherClass and pass the object pointer
        OtherClass* other2 = new OtherClass(object);    // "
        OtherClass* other3 = new OtherClass(object);    // "

        ???                                             // What happens with object now?
    }
    @endcode

    If you use SharedPtr<MyClass> instead of a classic MyClass* pointer, the instance of
    MyClass would be automatically destroyed if all instances of OtherClass are destroyed.
    You don't need any code in the destructor and you can completely forget about the
    object, because its managed by the SharedPtr.

    The same code using SharedPtr:
    @code
    class OtherClass                                        // Declaration of some class
    {
        public:
            OtherClass(const SharedPtr<MyClass>& object)    // Constructor
            {
                this->object_ = object;                     // Assigns the pointer to the member variable object_
            }

        private:
            SharedPtr<MyClass> object_;                     // A SharedPtr to the object
    };

    void someFunction()
    {
        SharedPtr<MyClass> object = new MyClass();          // Create a new instance of MyClass

        OtherClass* other1 = new OtherClass(object);        // Create an instance of OtherClass and pass the object pointer
        OtherClass* other2 = new OtherClass(object);        // "
        OtherClass* other3 = new OtherClass(object);        // "

    }                                                       // The SharedPtr "object" is destroyed at the end of the scope,
                                                            // but the three instances of OtherClass keep the object alive
                                                            // until they are all destroyed.
    @endcode
*/

#ifndef _SharedPtr_H__
#define _SharedPtr_H__

#include "UtilPrereqs.h"

#include <algorithm>
#include <cassert>

#include "SmallObjectAllocator.h"

namespace orxonox
{
    namespace detail
    {
        /// BaseClass of SharedCounterImpl, has a counter that is initialized with 1
        class SharedCounter
        {
            public:
                SharedCounter() : count_(1) {}
                virtual ~SharedCounter() {}
                virtual void destroy() = 0;

                int count_;
        };

        /// Child class of SharedCounter, keeps a pointer to an object of type T that can be destroyed with destroy()
        template <class T>
        class SharedCounterImpl : public SharedCounter
        {
            public:
                SharedCounterImpl(T* pointer) : pointer_(pointer) {}

                void destroy()
                {
                    delete this->pointer_;
                }

            private:
                T* pointer_;
        };

        _UtilExport SmallObjectAllocator& createSharedCounterPool();

        ORX_FORCEINLINE SmallObjectAllocator& getSharedCounterPool()
        {
            static SmallObjectAllocator& instance = createSharedCounterPool();
            return instance;
        }
    }

    /**
        @brief The SharedPtr template is a utility to manage pointers to an object.
        @param T The type of the managed object

        SharedPtr acts like a real pointer, except that it keeps track of the number of
        references to the object. If the the number of references drops to zero, the
        object is destroyed automatically.

        @see See @ref SharedPtrExample "this description" for some examples and more information.

        @note The number of references is stored in a separate object that is shared
        among all instances of SharedPtr that point to the same pointer. This object is
        also responsible for destroying the pointer if the reference counter becomes zero.
    */
    template <class T>
    class SharedPtr
    {
        template <class O>
        friend class SharedPtr;

        public:
            /// Default constructor, the pointer is set to NULL.
            inline SharedPtr() : pointer_(0), counter_(0)
            {
            }

            /// Constructor, creates a SharedPtr that points to @a pointer, increments the counter.
            inline SharedPtr(T* pointer) : pointer_(pointer), counter_(0)
            {
                if (this->pointer_)
                {
                    void* chunk = detail::getSharedCounterPool().alloc();
                    this->counter_ = new (chunk) detail::SharedCounterImpl<T>(this->pointer_);
                }
            }

            /// Copy-constructor, this SharedPtr now points to the same object like the other SharedPtr, increments the counter.
            inline SharedPtr(const SharedPtr& other) : pointer_(other.pointer_), counter_(other.counter_)
            {
                if (this->pointer_)
                    ++this->counter_->count_;
            }

            /// Copy-constructor for SharedPtr with another template agument, increments the counter.
            template <class O>
            inline SharedPtr(const SharedPtr<O>& other) : pointer_(other.pointer_), counter_(other.counter_)
            {
                if (this->pointer_)
                    ++this->counter_->count_;
            }

            /// Destructor, decrements the counter and deletes the object if the counter becomes zero.
            inline ~SharedPtr()
            {
                if (this->pointer_)
                {
                    --this->counter_->count_;

                    if (this->counter_->count_ == 0)
                    {
                        this->counter_->destroy();
                        detail::getSharedCounterPool().free(this->counter_);
                    }
                }
            }

            /// Assigns a new object, decrements the counter of the old object, increments the counter of the new object.
            inline SharedPtr& operator=(const SharedPtr& other)
            {
                SharedPtr(other).swap(*this);
                return *this;
            }

            /// Assigns a new object with another template argument, decrements the counter of the old object, increments the counter of the new object.
            template <class O>
            inline SharedPtr& operator=(const SharedPtr<O>& other)
            {
                SharedPtr(other).swap(*this);
                return *this;
            }

            /// Casts the pointer to another type
            template <class O>
            inline SharedPtr<O> cast() const
            {
                O* temp = static_cast<O*>(this->pointer_); // temp value for prettier compiler error in case of an invalid static_cast
                return SharedPtr<O>(temp, this->counter_);
            }

            /// Overloaded -> operator, returns the pointer to the managed object.
            inline T* operator->() const
            {
                assert(this->pointer_ != 0);
                return this->pointer_;
            }

            /// Overloaded * operator, returns a reference ot the managed object.
            inline T& operator*() const
            {
                assert(this->pointer_ != 0);
                return *this->pointer_;
            }

            /// Returns the pointer to the managed object.
            inline T* get() const
            {
                return this->pointer_;
            }

            /// Returns true if the pointer is not NULL.
            inline operator bool() const
            {
                return (this->pointer_ != 0);
            }

            /// Swaps the pointer and the counter of two instances of SharedPtr with the same template argument.
            inline void swap(SharedPtr& other)
            {
                std::swap(this->pointer_, other.pointer_);
                std::swap(this->counter_, other.counter_);
            }

        private:
            /// Private constructor, used by the cast() function.
            inline SharedPtr(T* pointer, detail::SharedCounter* counter) : pointer_(pointer), counter_(counter)
            {
                if (this->pointer_)
                    ++this->counter_->count_;
            }

            T* pointer_;                        ///< A pointer to the managed object of type @a T
            detail::SharedCounter* counter_;    ///< A pointer to the shared reference counter
    };

    /**
        @brief A child class of SharedPtr, used to reflect the hierarchy of the underlying class @a T.
        @param T The type of the managed object
        @param Parent The type of the SharedPtr that manages the parent class of @a T

        This class is used to reflect the hierarchy of the underlying class @a T.
        For example the @c Functor classes: While a @c Functor* pointer would be managed by
        @c SharedPtr<Functor>, the child class @c FunctorStatic is managed by the class
        <tt>SharedChildPtr<FunctorStatic, SharedPtr<Functor> ></tt>.

        The second template argument @a Parent is used as the parent class of
        SharedChildPtr. This means that each instance of <tt>SharedChildPtr<T, Parent></tt>
        can be upcasted to @c Parent.

        So for example this works:
        @code
        SharedChildPtr<FunctorStatic, SharedPtr<Functor> > functorStatic = createFunctor(&MyClass::myStaticFunction);
        SharedPtr<Functor> functor = functorStatic;
        @endcode

        @note There are some typedefs and more to make the usage of SharedChildPtr easier
        for the classes Functor and Executor. See FunctorPtr.h and ExecutorPtr.h. The above
        example could thus be simplified the following way:
        @code
        FunctorStaticPtr functorStatic = createFunctor(&MyClass::myStaticFunction);
        FunctorPtr functor = functorStatic;
        @endcode

        @see See SharedPtr for more information about the base class SharedPtr.
        @see See @ref SharedPtrExample "this description" for some examples about how to use SharedPtr.
    */
    template <class T, class Parent>
    class SharedChildPtr : public Parent
    {
        public:
            inline SharedChildPtr() : Parent() {}
            inline SharedChildPtr(T* pointer) : Parent(pointer) {}
            inline SharedChildPtr(const SharedPtr<T>& other) : Parent(other) {}

            inline T* operator->() const { return static_cast<T*>(Parent::operator->()); }
            inline T& operator*() const { return *static_cast<T*>(Parent::operator->()); }
    };

}

#endif /* _SharedPtr_H__ */
