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

#ifndef _WeakPtr_H__
#define _WeakPtr_H__

#include "CorePrereqs.h"

#include <cassert>
#include "OrxonoxClass.h"
#include "Functor.h"

namespace orxonox
{
    template <class T>
    class WeakPtr
    {
        friend class OrxonoxClass;

        public:
            inline WeakPtr() : pointer_(0), base_(0), callback_(0)
            {
            }

            inline WeakPtr(int) : pointer_(0), base_(0), callback_(0)
            {
            }

            inline WeakPtr(T* pointer) : pointer_(pointer), base_(pointer), callback_(0)
            {
                if (this->base_)
                    this->base_->registerWeakPtr(this);
            }

            inline WeakPtr(const WeakPtr& other) : pointer_(other.pointer_), base_(other.base_), callback_(0)
            {
                if (this->base_)
                    this->base_->registerWeakPtr(this);
            }

            template <class O>
            inline WeakPtr(const WeakPtr<O>& other) : pointer_(other.get()), base_(other.base_), callback_(0)
            {
                if (this->base_)
                    this->base_->registerWeakPtr(this);
            }

            inline ~WeakPtr()
            {
                if (this->base_)
                    this->base_->unregisterWeakPtr(this);
                if (this->callback_)
                    delete this->callback_;

            }

            inline const WeakPtr& operator=(int)
            {
                WeakPtr(0).swap(*this);
                return *this;
            }

            inline const WeakPtr& operator=(T* pointer)
            {
                WeakPtr(pointer).swap(*this);
                return *this;
            }

            inline const WeakPtr& operator=(const WeakPtr& other)
            {
                WeakPtr(other).swap(*this);
                return *this;
            }

            template <class O>
            inline const WeakPtr& operator=(const WeakPtr<O>& other)
            {
                WeakPtr(other).swap(*this);
                return *this;
            }

            inline T* get() const
            {
                return this->pointer_;
            }

            inline OrxonoxClass* getBase() const
            {
                return this->base_;
            }

            inline operator T*() const
            {
                return this->pointer_;
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

            inline bool operator!() const
            {
                return (this->pointer_ == 0);
            }

            inline void swap(WeakPtr& other)
            {
                if (this->base_)
                    this->base_->unregisterWeakPtr(this);
                if (other.base_)
                    other.base_->unregisterWeakPtr(&other);

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

                if (this->base_)
                    this->base_->registerWeakPtr(this);
                if (other.base_)
                    other.base_->registerWeakPtr(&other);
            }

            inline void reset()
            {
                WeakPtr().swap(*this);
            }

            inline void setCallback(Functor* callback)
            {
                this->callback_ = callback;
            }

            inline Functor* getFunctor() const
            {
                return this->callback_;
            }

        private:
            inline void objectDeleted()
            {
                this->base_ = 0;
                this->pointer_ = 0;
                if (this->callback_)
                    (*this->callback_)();
            }

            T* pointer_;
            OrxonoxClass* base_;
            Functor* callback_;
    };

    template <class T>
    void swap(WeakPtr<T>& a, WeakPtr<T>& b)
    {
        a.swap(b);
    }

    template <class T, class U>
    WeakPtr<T> static_pointer_cast(const WeakPtr<U>& p)
    {
        return static_cast<T*>(p.get());
    }

    template <class T, class U>
    WeakPtr<T> const_pointer_cast(const WeakPtr<U>& p)
    {
        return const_cast<T*>(p.get());
    }

    template <class T, class U>
    WeakPtr<T> dynamic_pointer_cast(const WeakPtr<U>& p)
    {
        return dynamic_cast<T*>(p.get());
    }
}

#endif /* _WeakPtr_H__ */
