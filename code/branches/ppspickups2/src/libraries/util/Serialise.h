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
    @brief Functions to serialise most of the types/classed used in Orxonox
*/

#ifndef _Serialise_H__
#define _Serialise_H__

#include <cstring>
#include "util/Math.h"
#include "util/mbool.h"

namespace orxonox{

    /** @brief returns the size of the variable in a datastream */
    template <class T> inline uint32_t returnSize( const T& variable );
    /** @brief loads the value of a variable out of the bytestream and increases the mem pointer */
    template <class T> inline void loadAndIncrease( const T& variable, uint8_t*& mem );
    /** @brief saves the value of a variable into the bytestream and increases the mem pointer */
    template <class T> inline void saveAndIncrease( const T& variable, uint8_t*& mem );
    /** @brief checks whether the variable of type T is the same as in the bytestream */
    template <class T> inline bool checkEquality( const T& variable, uint8_t* mem );

// =================== Template specialisation stuff =============

// =========== bool

    template <> inline uint32_t returnSize( const bool& variable )
    {
        return sizeof(uint8_t);
    }

    template <> inline void loadAndIncrease( const bool& variable, uint8_t*& mem )
    {
        *(uint8_t*)( &variable ) = *static_cast<uint8_t*>(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const bool& variable, uint8_t*& mem )
    {
        *static_cast<uint8_t*>(mem) = *(uint8_t*)( &variable );
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const bool& variable, uint8_t* mem )
    {
        return *static_cast<uint8_t*>(mem) == *(uint8_t*)( &variable );
    }

// =========== char

    template <> inline uint32_t returnSize( const char& variable )
    {
        return sizeof(uint8_t);
    }

    template <> inline void loadAndIncrease( const char& variable, uint8_t*& mem )
    {
        *(uint8_t*)( &variable ) = *static_cast<uint8_t*>(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const char& variable, uint8_t*& mem )
    {
        *static_cast<uint8_t*>(mem) = *(uint8_t*)( &variable );
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const char& variable, uint8_t* mem )
    {
        return *static_cast<uint8_t*>(mem) == *(uint8_t*)( &variable );
    }

// =========== unsigned char

    template <> inline uint32_t returnSize( const unsigned char& variable )
    {
        return sizeof(uint8_t);
    }

    template <> inline void loadAndIncrease( const unsigned char& variable, uint8_t*& mem )
    {
        *(uint8_t*)( &variable ) = *static_cast<uint8_t*>(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const unsigned char& variable, uint8_t*& mem )
    {
        *static_cast<uint8_t*>(mem) = *(uint8_t*)( &variable );
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const unsigned char& variable, uint8_t* mem )
    {
        return *static_cast<uint8_t*>(mem) == *(uint8_t*)( &variable );
    }

// =========== short

    template <> inline uint32_t returnSize( const short& variable )
    {
        return sizeof(int16_t);
    }

    template <> inline void loadAndIncrease( const short& variable, uint8_t*& mem )
    {
        *(short*)( &variable ) = *(int16_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const short& variable, uint8_t*& mem )
    {
        *(int16_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const short& variable, uint8_t* mem )
    {
        return *(int16_t*)(mem) == static_cast<int16_t>(variable);
    }

// =========== unsigned short

    template <> inline uint32_t returnSize( const unsigned short& variable )
    {
        return sizeof(uint16_t);
    }

    template <> inline void loadAndIncrease( const unsigned short& variable, uint8_t*& mem )
    {
        *(unsigned short*)( &variable ) = *(uint16_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const unsigned short& variable, uint8_t*& mem )
    {
        *(uint16_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const unsigned short& variable, uint8_t* mem )
    {
        return *(uint16_t*)(mem) == variable;
    }

// =========== int

    template <> inline uint32_t returnSize( const int& variable )
    {
        return sizeof(int32_t);
    }

    template <> inline void loadAndIncrease( const int& variable, uint8_t*& mem )
    {
        *(int *)( &variable ) = *(int32_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const int& variable, uint8_t*& mem )
    {
        *(int32_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const int& variable, uint8_t* mem )
    {
        return *(int32_t*)(mem) == variable;
    }

// =========== unsigned int

    template <> inline uint32_t returnSize( const unsigned int& variable )
    {
        return sizeof(uint32_t);
    }

    template <> inline void loadAndIncrease( const unsigned int& variable, uint8_t*& mem )
    {
        *(unsigned int*)( &variable ) = *(uint32_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const unsigned int& variable, uint8_t*& mem )
    {
        *(uint32_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const unsigned int& variable, uint8_t* mem )
    {
        return *(uint32_t*)(mem) == variable;
    }

// =========== long

    template <> inline uint32_t returnSize( const long& variable )
    {
        return sizeof(int32_t);
    }

    template <> inline void loadAndIncrease( const long& variable, uint8_t*& mem )
    {
        *(long*)( &variable ) = *(int32_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const long& variable, uint8_t*& mem )
    {
        *(int32_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const long& variable, uint8_t* mem )
    {
        return *(int32_t*)(mem) == variable;
    }

// =========== unsigned long

    template <> inline uint32_t returnSize( const unsigned long& variable )
    {
        return sizeof(uint32_t);
    }

    template <> inline void loadAndIncrease( const unsigned long& variable, uint8_t*& mem )
    {
        *(unsigned long*)( &variable ) = *(uint32_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const unsigned long& variable, uint8_t*& mem )
    {
        *(uint32_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const unsigned long& variable, uint8_t* mem )
    {
        return *(uint32_t*)(mem) == variable;
    }

// =========== long long

    template <> inline uint32_t returnSize( const long long& variable )
    {
        return sizeof(int64_t);
    }

    template <> inline void loadAndIncrease( const long long& variable, uint8_t*& mem )
    {
        *(long long*)( &variable ) = *(int64_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const long long& variable, uint8_t*& mem )
    {
        *(int64_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const long long& variable, uint8_t* mem )
    {
        return *(int64_t*)(mem) == variable;
    }

// =========== unsigned long long

    template <> inline uint32_t returnSize( const unsigned long long& variable )
    {
        return sizeof(uint64_t);
    }

    template <> inline void loadAndIncrease( const unsigned long long& variable, uint8_t*& mem )
    {
        *(unsigned long long*)( &variable ) = *(uint64_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const unsigned long long& variable, uint8_t*& mem )
    {
        *(uint64_t*)(mem) = variable;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const unsigned long long& variable, uint8_t* mem )
    {
        return *(uint64_t*)(mem) == variable;
    }

// =========== float

    template <> inline uint32_t returnSize( const float& variable )
    {
        return sizeof(uint32_t);
    }

    template <> inline void loadAndIncrease( const float& variable, uint8_t*& mem )
    {
        *(uint32_t*)( &variable ) = *(uint32_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const float& variable, uint8_t*& mem )
    {
        *(uint32_t*)(mem) = *(uint32_t*)( &variable );
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const float& variable, uint8_t* mem )
    {
        return *(uint32_t*)(mem) == *(uint32_t*)( &variable );
    }

// =========== double

    template <> inline uint32_t returnSize( const double& variable )
    {
        return sizeof(uint64_t);
    }

    template <> inline void loadAndIncrease( const double& variable, uint8_t*& mem )
    {
        *(uint64_t*)( &variable ) = *(uint64_t*)(mem);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const double& variable, uint8_t*& mem )
    {
        *(uint64_t*)(mem) = *(uint64_t*)( &variable );
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const double& variable, uint8_t* mem )
    {
        return *(uint64_t*)(mem) == *(uint64_t*)( &variable );
    }

// =========== long double

    template <> inline uint32_t returnSize( const long double& variable )
    {
        return sizeof(uint64_t);
    }

    template <> inline void loadAndIncrease( const long double& variable, uint8_t*& mem )
    {
        double temp;
        memcpy(&temp, mem, sizeof(uint64_t));
        *(long double*)( &variable ) = static_cast<long double>(temp);
        mem += returnSize( variable );
    }

    template <> inline void saveAndIncrease( const long double& variable, uint8_t*& mem )
    {
        double temp = static_cast<double>(variable);
        memcpy(mem, &temp, sizeof(uint64_t));
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const long double& variable, uint8_t* mem )
    {
        double temp = static_cast<double>(variable);
        return memcmp(&temp, mem, sizeof(uint64_t))==0;
    }

// =========== string

    template <> inline uint32_t returnSize( const std::string& variable )
    {
        return variable.length()+1;
    }

    template <> inline void saveAndIncrease( const std::string& variable, uint8_t*& mem )
    {
        memcpy(mem, variable.c_str(), variable.length()+1);
        mem += variable.length()+1;
    }

    template <> inline void loadAndIncrease( const std::string& variable, uint8_t*& mem )
    {
        *(std::string*)( &variable ) = (const char *)mem;
        mem += variable.length()+1;
    }

    template <> inline bool checkEquality( const std::string& variable, uint8_t* mem )
    {
        //return std::string((const char*)mem)==variable;
        return (const char*)mem==variable;
    }

// =========== Degree

    template <> inline uint32_t returnSize( const Degree& variable )
    {
        return sizeof(Ogre::Real);
    }

    template <> inline void saveAndIncrease( const Degree& variable, uint8_t*& mem )
    {
        Ogre::Real r = variable.valueDegrees();
        memcpy(mem, &r, returnSize( variable ));
        mem += returnSize( variable );
    }

    template <> inline void loadAndIncrease( const Degree& variable, uint8_t*& mem )
    {
        Ogre::Real* r = (Ogre::Real*)mem;
        (Degree&)variable = *r;
        mem += returnSize( variable );
    }

     template <> inline bool checkEquality( const Degree& variable, uint8_t* mem )
    {
        Ogre::Real* r = (Ogre::Real*)mem;
        return variable==Degree(*r);
    }

// =========== Radian

    template <> inline uint32_t returnSize( const Radian& variable )
    {
        return sizeof(Ogre::Real);
    }

    template <> inline void saveAndIncrease( const Radian& variable, uint8_t*& mem )
    {
        Ogre::Real r = variable.valueRadians();
        memcpy(mem, &r, returnSize( variable ));
        mem += returnSize( variable );
    }

    template <> inline void loadAndIncrease( const Radian& variable, uint8_t*& mem )
    {
        Ogre::Real* r = (Ogre::Real*)mem;
        (Radian&)variable = *r;
        mem += returnSize( variable );
    }

    template <> inline bool checkEquality( const Radian& variable, uint8_t* mem )
    {
        Ogre::Real* r = (Ogre::Real*)mem;
        return variable==Degree(*r);
    }

    // =========== Vector2

    template <> inline uint32_t returnSize( const Vector2& variable )
    {
        return returnSize( variable.x )+returnSize( variable.y );
    }

    template <> inline void saveAndIncrease( const Vector2& variable, uint8_t*& mem )
    {
        saveAndIncrease( variable.x, mem );
        saveAndIncrease( variable.y, mem );
    }

    template <> inline void loadAndIncrease( const Vector2& variable, uint8_t*& mem )
    {
        loadAndIncrease( variable.x, mem );
        loadAndIncrease( variable.y, mem );
    }

    template <> inline bool checkEquality( const Vector2& variable, uint8_t* mem )
    {
        return checkEquality(variable.x, mem) && checkEquality(variable.y, mem+returnSize(variable.x));
    }

    // =========== Vector3

    template <> inline uint32_t returnSize( const Vector3& variable )
    {
        return returnSize( variable.x )+returnSize( variable.y )+returnSize( variable.z );
    }

    template <> inline void saveAndIncrease( const Vector3& variable, uint8_t*& mem )
    {
        saveAndIncrease( variable.x, mem );
        saveAndIncrease( variable.y, mem );
        saveAndIncrease( variable.z, mem );
    }

    template <> inline void loadAndIncrease( const Vector3& variable, uint8_t*& mem )
    {
        loadAndIncrease( variable.x, mem );
        loadAndIncrease( variable.y, mem );
        loadAndIncrease( variable.z, mem );
    }

    template <> inline bool checkEquality( const Vector3& variable, uint8_t* mem )
    {
        return checkEquality(variable.x, mem) && checkEquality(variable.y, mem+returnSize(variable.x)) &&
            checkEquality(variable.z, mem+returnSize(variable.x)+returnSize(variable.y));
    }

    // =========== Vector4

    template <> inline uint32_t returnSize( const Vector4& variable )
    {
        return returnSize( variable.w )+returnSize( variable.x )+returnSize( variable.y )+returnSize( variable.z );
    }

    template <> inline void saveAndIncrease( const Vector4& variable, uint8_t*& mem )
    {
        saveAndIncrease( variable.w, mem );
        saveAndIncrease( variable.x, mem );
        saveAndIncrease( variable.y, mem );
        saveAndIncrease( variable.z, mem );
    }

    template <> inline void loadAndIncrease( const Vector4& variable, uint8_t*& mem )
    {
        loadAndIncrease( variable.w, mem );
        loadAndIncrease( variable.x, mem );
        loadAndIncrease( variable.y, mem );
        loadAndIncrease( variable.z, mem );
    }

    template <> inline bool checkEquality( const Vector4& variable, uint8_t* mem )
    {
        return checkEquality(variable.w, mem) && checkEquality(variable.x, mem+returnSize(variable.w)) &&
            checkEquality(variable.y, mem+returnSize(variable.w)+returnSize(variable.x)) &&
            checkEquality(variable.z, mem+returnSize(variable.w)+returnSize(variable.x)+returnSize(variable.y));
    }

    // =========== Quaternion

    template <> inline uint32_t returnSize( const Quaternion& variable )
    {
        return returnSize( variable.w )+returnSize( variable.x )+returnSize( variable.y )+returnSize( variable.z );
    }

    template <> inline void saveAndIncrease( const Quaternion& variable, uint8_t*& mem )
    {
        saveAndIncrease( variable.w, mem );
        saveAndIncrease( variable.x, mem );
        saveAndIncrease( variable.y, mem );
        saveAndIncrease( variable.z, mem );
    }

    template <> inline void loadAndIncrease( const Quaternion& variable, uint8_t*& mem )
    {
        loadAndIncrease( variable.w, mem );
        loadAndIncrease( variable.x, mem );
        loadAndIncrease( variable.y, mem );
        loadAndIncrease( variable.z, mem );
    }

    template <> inline bool checkEquality( const Quaternion& variable, uint8_t* mem )
    {
        return checkEquality(variable.w, mem) && checkEquality(variable.x, mem+returnSize(variable.w)) &&
            checkEquality(variable.y, mem+returnSize(variable.w)+returnSize(variable.x)) &&
            checkEquality(variable.z, mem+returnSize(variable.w)+returnSize(variable.x)+returnSize(variable.y));
    }

    // =========== ColourValue

    template <> inline uint32_t returnSize( const ColourValue& variable )
    {
        return returnSize( variable.r )+returnSize( variable.g )+returnSize( variable.b )+returnSize( variable.a );
    }

    template <> inline void saveAndIncrease( const ColourValue& variable, uint8_t*& mem )
    {
        saveAndIncrease( variable.r, mem );
        saveAndIncrease( variable.g, mem );
        saveAndIncrease( variable.b, mem );
        saveAndIncrease( variable.a, mem );
    }

    template <> inline void loadAndIncrease( const ColourValue& variable, uint8_t*& mem )
    {
        loadAndIncrease( variable.r, mem );
        loadAndIncrease( variable.g, mem );
        loadAndIncrease( variable.b, mem );
        loadAndIncrease( variable.a, mem );
    }

    template <> inline bool checkEquality( const ColourValue& variable, uint8_t* mem )
    {
        return checkEquality(variable.r, mem) && checkEquality(variable.g, mem+returnSize(variable.r)) &&
            checkEquality(variable.b, mem+returnSize(variable.r)+returnSize(variable.g)) &&
            checkEquality(variable.a, mem+returnSize(variable.r)+returnSize(variable.g)+returnSize(variable.b));
    }

    // =========== mbool

    template <> inline uint32_t returnSize( const mbool& variable )
    {
        return returnSize( (unsigned char&)((mbool&)variable).getMemory() );
    }

    template <> inline void saveAndIncrease( const mbool& variable, uint8_t*& mem )
    {
        saveAndIncrease( (unsigned char&)((mbool&)variable).getMemory(), mem );
    }

    template <> inline void loadAndIncrease( const mbool& variable, uint8_t*& mem )
    {
        loadAndIncrease( (unsigned char&)((mbool&)variable).getMemory(), mem );
    }

    template <> inline bool checkEquality( const mbool& variable, uint8_t* mem )
    {
        return checkEquality( (unsigned char&)((mbool&)variable).getMemory(), mem );
    }
}


#endif
