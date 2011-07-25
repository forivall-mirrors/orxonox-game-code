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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Functions to serialise pointers to objects that inherit from Synchronisable
*/

#ifndef _NetworkSerialise_H__
#define _NetworkSerialise_H__

#include "util/Serialise.h"
#include "util/TypeTraits.h"
#include "core/CorePrereqs.h"

namespace orxonox{
    
    // These functions implement loading / saving / etc. for pointer types
    
    /** @brief returns the size of the objectID needed to synchronise the pointer */
    template <class T> inline uint32_t returnSize( T*& variable )
    {
      return sizeof(uint32_t);
    }
    
    /** @brief reads the objectID of a pointer out of the bytestream and increases the mem pointer */
    template <class T> inline void loadAndIncrease( T*& variable, uint8_t*& mem )
    {
        *const_cast<typename Loki::TypeTraits<T*>::UnqualifiedType*>(&variable) = dynamic_cast<T*>(variable->getSynchronisable( *(uint32_t*)(mem) ));
        mem += returnSize( variable );
    }
    
    /** @brief saves the objectID of a pointer into the bytestream and increases the mem pointer */
    template <class T> inline void saveAndIncrease( T*& variable, uint8_t*& mem )
    {
        if ( variable )
            *(uint32_t*)(mem) = static_cast<uint32_t>(variable->getObjectID());
        else
            *(uint32_t*)(mem) = OBJECTID_UNKNOWN;
        mem += returnSize( variable );
    }
    
    /** @brief checks whether the objectID of the variable is the same as in the bytestream */
    template <class T> inline  bool checkEquality( T*& variable, uint8_t* mem )
    {
        if ( variable )
            return *(uint32_t*)(mem) == variable->getObjectID();
        else
            return *(uint32_t*)(mem) == OBJECTID_UNKNOWN;
    }
}


#endif
