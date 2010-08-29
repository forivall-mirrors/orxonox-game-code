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

#ifndef _SmallObjectAllocator_H__
#define _SmallObjectAllocator_H__

#include "UtilPrereqs.h"
#include <vector>

namespace orxonox
{
    class _UtilExport SmallObjectAllocator
    {
        struct Chunk
        {
            Chunk* next_;
        };

        public:
            SmallObjectAllocator(size_t objectSize, size_t numObjects = 64);
            ~SmallObjectAllocator();

            void* alloc();
            void free(void* chunk);

        private:
            static void setNext(void* chunk, void* next);
            static void* getNext(void* chunk);

            void* first_;
            size_t objectSize_;
            size_t numObjects_;

            std::vector<char*> blocks_;
    };
}

#endif /* _SmallObjectAllocator_H__ */
