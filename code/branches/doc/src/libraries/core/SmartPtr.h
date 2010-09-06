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
*/

#ifndef _SmartPtr_H__
#define _SmartPtr_H__

#include "CorePrereqs.h"

#include <cassert>

#include "Identifier.h"
#include "OrxonoxClass.h"
#include "WeakPtr.h"

namespace orxonox
{
    template <class T>
    class SmartPtr
    {
        public:
            inline SmartPtr() : pointer_(0), base_(0)
            {
            }

            inline SmartPtr(int) : pointer_(0), base_(0)
            {
            }

            inline SmartPtr(T* pointer, bool bAddRef = true) : pointer_(pointer), base_(pointer)
            {
                if (this->base_ && bAddRef)
                    this->base_->incrementReferenceCount();
            }

            inline SmartPtr(const SmartPtr& other) : pointer_(other.pointer_), base_(other.base_)
            {
                if (this->base_)
                    this->base_->incrementReferenceCount();
            }

            template <class O>
            inline SmartPtr(const SmartPtr<O>& other) : pointer_(other.get()), base_(other.base_)
            {
                if (this->base_)
                    this->base_->incrementReferenceCount();
            }

            template <class O>
            inline SmartPtr(const WeakPtr<O>& other) : pointer_(other.get()), base_(other.getBase())
            {
                if (this->base_)
                    this->base_->incrementReferenceCount();
            }

            inline ~SmartPtr()
            {
                if (this->base_)
                    this->base_->decrementReferenceCount();
            }

            inline SmartPtr& operator=(int)
            {
                SmartPtr(0).swap(*this);
                return *this;
            }

            inline SmartPtr& operator=(T* pointer)
            {
                SmartPtr(pointer).swap(*this);
                return *this;
            }

            inline SmartPtr& operator=(const SmartPtr& other)
            {
                SmartPtr(other).swap(*this);
                return *this;
            }

            template <class O>
            inline SmartPtr& operator=(const SmartPtr<O>& other)
            {
                SmartPtr(other).swap(*this);
                return *this;
            }

            template <class O>
            inline SmartPtr& operator=(const WeakPtr<O>& other)
            {
                SmartPtr(other).swap(*this);
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

            inline void swap(SmartPtr& other)
            {
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
            }

            inline void reset()
            {
                SmartPtr().swap(*this);
            }

        private:
            T* pointer_;
            OrxonoxClass* base_;
    };

    template <class T>
    void swap(SmartPtr<T>& a, SmartPtr<T>& b)
    {
        a.swap(b);
    }

    template <class T, class U>
    SmartPtr<T> static_pointer_cast(const SmartPtr<U>& p)
    {
        return static_cast<T*>(p.get());
    }

    template <class T, class U>
    SmartPtr<T> const_pointer_cast(const SmartPtr<U>& p)
    {
        return const_cast<T*>(p.get());
    }

    template <class T, class U>
    SmartPtr<T> dynamic_pointer_cast(const SmartPtr<U>& p)
    {
        return orxonox_cast<T*>(p.get());
    }
}

#endif /* _SmartPtr_H__ */
