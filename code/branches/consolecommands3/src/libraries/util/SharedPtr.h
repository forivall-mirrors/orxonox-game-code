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

#ifndef _SharedPtr_H__
#define _SharedPtr_H__

#include "UtilPrereqs.h"
#include <algorithm>
#include <cassert>

namespace orxonox
{
    class SharedPtrDestroyer
    {
        public:
            virtual void destroy() = 0;
    };

    template <class T>
    class SharedPtrDestroyerImpl : public SharedPtrDestroyer
    {
        public:
            SharedPtrDestroyerImpl(T* pointer) : pointer_(pointer) {}

            void destroy()
            {
                delete this->pointer_;
            }

        private:
            T* pointer_;
    };

    template <class T>
    class SharedPtr
    {
        template <class O>
        friend class SharedPtr;

        public:
            inline SharedPtr() : pointer_(0), counter_(0), destroyer_(0)
            {
            }

            inline SharedPtr(T* pointer) : pointer_(pointer), counter_(0), destroyer_(0)
            {
                if (this->pointer_)
                {
                    this->counter_ = new int(1);
                    this->destroyer_ = new SharedPtrDestroyerImpl<T>(this->pointer_);
                }
            }

            inline SharedPtr(const SharedPtr& other) : pointer_(other.pointer_), counter_(other.counter_), destroyer_(other.destroyer_)
            {
                if (this->pointer_)
                    ++(*this->counter_);
            }

            template <class O>
            inline SharedPtr(const SharedPtr<O>& other) : pointer_(other.pointer_), counter_(other.counter_), destroyer_(other.destroyer_)
            {
                if (this->pointer_)
                    ++(*this->counter_);
            }

            inline ~SharedPtr()
            {
                if (this->pointer_)
                {
                    --(*this->counter_);

                    if (*this->counter_ == 0)
                    {
                        this->destroyer_->destroy();
                        delete this->destroyer_;
                        delete this->counter_;
                    }
                }
            }

            inline const SharedPtr& operator=(const SharedPtr& other)
            {
                SharedPtr(other).swap(*this);
                return *this;
            }

            template <class O>
            inline const SharedPtr& operator=(const SharedPtr<O>& other)
            {
                SharedPtr(other).swap(*this);
                return *this;
            }

            template <class O>
            inline SharedPtr<O> cast() const
            {
                O* temp = static_cast<O*>(this->pointer_); // temp value for prettier compiler error in case of an invalid static_cast
                return SharedPtr<O>(temp, this->counter_, this->destroyer_);
            }

            inline T* operator->() const
            {
                assert(this->pointer_ != 0);
                return this->pointer_;
            }

            inline T& operator*() const
            {
                assert(this->pointer_ != 0);
                return *this->pointer_;
            }

            inline T* get() const
            {
                return this->pointer_;
            }

            inline operator bool() const
            {
                return (this->pointer_ != 0);
            }

            inline void swap(SharedPtr& other)
            {
                std::swap(this->pointer_, other.pointer_);
                std::swap(this->counter_, other.counter_);
                std::swap(this->destroyer_, other.destroyer_);
            }

        private:
            inline SharedPtr(T* pointer, int* counter, SharedPtrDestroyer* destroyer) : pointer_(pointer), counter_(counter), destroyer_(destroyer)
            {
                if (this->pointer_)
                    ++(*this->counter_);
            }

            T* pointer_;
            int* counter_;
            SharedPtrDestroyer* destroyer_;
    };

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
