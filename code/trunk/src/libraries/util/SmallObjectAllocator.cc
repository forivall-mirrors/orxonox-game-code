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

#include "SmallObjectAllocator.h"

namespace orxonox
{
    SmallObjectAllocator::SmallObjectAllocator(size_t objectSize, size_t numObjects)
    {
        this->objectSize_ = std::max(objectSize, sizeof(Chunk));
        this->numObjects_ = numObjects;
        this->first_ = 0;
    }

    SmallObjectAllocator::~SmallObjectAllocator()
    {
        for (std::vector<char*>::iterator it = this->blocks_.begin(); it != this->blocks_.end(); ++it)
            delete[] *it;
    }

    /* static */ void SmallObjectAllocator::setNext(void* chunk, void* next)
    {
        static_cast<Chunk*>(chunk)->next_ = static_cast<Chunk*>(next);
    }

    /* static */ void* SmallObjectAllocator::getNext(void* chunk)
    {
        return static_cast<Chunk*>(chunk)->next_;
    }

    void* SmallObjectAllocator::alloc()
    {
        void* chunk = this->first_;

        if (chunk)
        {
            this->first_ = getNext(chunk);
        }
        else
        {
            char* block = new char[this->objectSize_ * this->numObjects_];
            this->blocks_.push_back(block);

            for (size_t i = 1; i < this->numObjects_ - 1; ++i)
                setNext(block + i * this->objectSize_, block + (i + 1) * this->objectSize_);

            setNext(block + (this->numObjects_ - 1) * this->objectSize_, 0);

            this->first_ = block + this->objectSize_;

            chunk = block;
        }

        return chunk;
    }

    void SmallObjectAllocator::free(void* chunk)
    {
        setNext(chunk, this->first_);
        this->first_ = chunk;
    }
}
