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
    @file
    @brief Implementation of SmallObjectAllocator
*/

#include "SmallObjectAllocator.h"

namespace orxonox
{
    /**
        @brief Constructor: initializes the allocator and its values.
        @param objectSize The size in bytes (returned by sizeof()) of the allocated objects
        @param numObjects The number of objects that are allocated in one block of memory
    */
    SmallObjectAllocator::SmallObjectAllocator(size_t objectSize, size_t numObjects)
    {
        this->chunkSize_ = std::max(objectSize, sizeof(Chunk)); // the chunk's size will be the maximum of the object's size and the size of a Chunk object itself
        this->numChunksPerBlock_ = numObjects;
        this->first_ = 0;
    }

    /**
        @brief Destructor: deletes the allocated memory blocks.
    */
    SmallObjectAllocator::~SmallObjectAllocator()
    {
        for (std::vector<char*>::iterator it = this->blocks_.begin(); it != this->blocks_.end(); ++it)
            delete[] *it;
    }

    /**
        @brief Helper function, used to set the next_ pointer of a Chunk.
    */
    /* static */ void SmallObjectAllocator::setNext(void* chunk, void* next)
    {
        static_cast<Chunk*>(chunk)->next_ = static_cast<Chunk*>(next);
    }

    /**
        @brief Helper function, returns the next_ pointer of a Chunk
    */
    /* static */ void* SmallObjectAllocator::getNext(void* chunk)
    {
        return static_cast<Chunk*>(chunk)->next_;
    }

    /**
        @brief Returns the first free memory chunk or allocates a new block of memory.
    */
    void* SmallObjectAllocator::alloc()
    {
        // get the first free chunk
        void* chunk = this->first_;

        // check if the chunk exists
        if (chunk)
        {
            // yes it does - the first_ pointer now points to the second element in the list
            this->first_ = getNext(chunk);
        }
        else
        {
            // no it doesnt - allocate a new block of memory
            char* block = new char[this->chunkSize_ * this->numChunksPerBlock_];
            this->blocks_.push_back(block);

            // iterate through the chunks in the new memory block and link them together to a single linked list
            for (size_t i = 1; i < this->numChunksPerBlock_ - 1; ++i)
                setNext(block + i * this->chunkSize_, block + (i + 1) * this->chunkSize_);

            // the next_ pointer of the last chunk must point to NULL
            setNext(block + (this->numChunksPerBlock_ - 1) * this->chunkSize_, 0);

            // The second chunk in the block is assigned to the first_ pointer
            this->first_ = block + this->chunkSize_;

            // The first chunk in the block is returned
            chunk = block;
        }

        // return the pointer to the chunk
        return chunk;
    }

    /**
        @brief Puts the memory chunk back on the list of free memory.
    */
    void SmallObjectAllocator::free(void* chunk)
    {
        // The first_ pointer points to the freed chunk, its next_ pointer points to the rest of the list
        setNext(chunk, this->first_);
        this->first_ = chunk;
    }
}
