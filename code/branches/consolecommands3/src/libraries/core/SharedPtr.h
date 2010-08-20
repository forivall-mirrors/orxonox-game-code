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

#include "CorePrereqs.h"

namespace orxonox
{
    template <class T>
    class SharedPtr
    {
        public:
            inline SharedPtr() : pointer_(0)
            {
//                COUT(0) << "SharedPtr (1): " << this->pointer_ << std::endl;
            }

            inline SharedPtr(T* pointer) : pointer_(pointer)
            {
//                COUT(0) << "SharedPtr (2): " << this->pointer_ << std::endl;
                if (this->pointer_)
                    this->pointer_->incrementReferenceCount();
            }

            inline SharedPtr(const SharedPtr& other) : pointer_(other.pointer_)
            {
//                COUT(0) << "SharedPtr (3): " << this->pointer_ << std::endl;
                if (this->pointer_)
                    this->pointer_->incrementReferenceCount();
            }

            inline ~SharedPtr()
            {
//                COUT(0) << "~SharedPtr: " << this->pointer_ << std::endl;
                if (this->pointer_)
                    this->pointer_->decrementReferenceCount();
            }

            inline const SharedPtr& operator=(const SharedPtr& other)
            {
//                COUT(0) << "SharedPtr=" << std::endl;
                SharedPtr(other).swap(*this);
                return *this;
            }

            inline void cast(const SharedPtr& other)
            {
//                COUT(0) << "SharedPtr cast" << std::endl;
                SharedPtr(other).swap(*this);
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

            inline operator bool() const
            {
                return (this->pointer_ != 0);
            }

            inline void swap(SharedPtr& other)
            {
                T* temp = this->pointer_;
                this->pointer_ = other.pointer_;
                other.pointer_ = temp;
            }

        private:
            T* pointer_;
    };

    template <class T, class Parent>
    class SharedChildPtr : public SharedPtr<Parent>
    {
        public:
            inline SharedChildPtr() : SharedPtr<Parent>() {}
            inline SharedChildPtr(T* pointer) : SharedPtr<Parent>(pointer) {}
            inline SharedChildPtr(const SharedChildPtr& other) : SharedPtr<Parent>(other) {}
            inline const SharedChildPtr& operator=(const SharedChildPtr& other) { SharedPtr<Parent>::operator=(other); return *this; }
            inline T* operator->() const { return static_cast<T*>(SharedPtr<Parent>::operator->()); }
            inline T& operator*() const { return *static_cast<T*>(SharedPtr<Parent>::operator->()); }
    };
}

#endif /* _SharedPtr_H__ */
