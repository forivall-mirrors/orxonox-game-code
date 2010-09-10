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
    @defgroup SmallObjectAllocator SmallObjectAllocator
    @ingroup Util
*/

/**
    @file
    @ingroup SmallObjectAllocator
    @brief Declaration of SmallObjectAllocator

    @anchor SmallObjectAllocatorExample

    The default implementations of new and delete are designed to work with objects of
    arbitrary size. They are thus not optimal for small objects.
    @ref orxonox::SmallObjectAllocator "SmallObjectAllocator" allocates a large memory
    block and divides it into small chunks. These chunks are returned by the function
    @ref orxonox::SmallObjectAllocator::alloc() "alloc()" and can be used to create a
    new object using the placement new operator. Instead of delete, the function
    @ref orxonox::SmallObjectAllocator::free() "free()" is used to give the memory
    back to SmallObjectAllocator.

    Example:
    @code
    SmallObjectAllocator allocator(sizeof(MySmallObject));  // Create an allocator. The size of the memory chunks must equal the size of the desired class

    void* chunk = allocator.alloc();                        // Allocate a memory chunk
    MySmallObject* object = new (chunk) MySmallObject();    // Call the placement new operator

    object->someFunction();                                 // Do something with the object

    object->~MySmallObject();                               // Call the destructor
    allocator.free(object);                                 // Free the allocated memory
    @endcode

    @b Important: You have to call the destructor of the object manually, because this
    is not automatically done by the allocator nor free().

    @note The destructor can be ignored if it is empty or not implemented. This saves
    another amount of time.

    @remarks For a distributed usage of SmallObjectAllocator it may be a good idea to
    create a static function that returns an instance to it. The allocator then works
    like a singleton and can be accesses from everywhere.
*/

#ifndef _SmallObjectAllocator_H__
#define _SmallObjectAllocator_H__

#include "UtilPrereqs.h"
#include <vector>

namespace orxonox
{
    /**
        @brief This class is used to allocate and free small objects (usually not polymorphic).

        SmallObjectAllocator provides a fast alternative to new and delete for small objects.

        @see See @ref SmallObjectAllocatorExample "this description" for more information and an example.
    */
    class _UtilExport SmallObjectAllocator
    {
        /// The memory chunk is at the same time an element of a single linked list.
        struct Chunk
        {
            Chunk* next_;   ///< A pointer to the next chunk in the list
        };

        public:
            SmallObjectAllocator(size_t objectSize, size_t numObjects = 64);
            ~SmallObjectAllocator();

            void* alloc();
            void free(void* chunk);

        private:
            static void setNext(void* chunk, void* next);
            static void* getNext(void* chunk);

            void* first_;                   ///< A pointer to the first free memory chunk
            size_t chunkSize_;              ///< The size of each chunk (and usually also the size of the created objects)
            size_t numChunksPerBlock_;      ///< The number of chunks per memory block

            std::vector<char*> blocks_;     ///< A list of all allocated memory blocks (used to destroy them again)
    };
}

#endif /* _SmallObjectAllocator_H__ */
