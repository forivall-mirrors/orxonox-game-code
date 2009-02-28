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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */

#include "SynchronisableVariable.h"
#include <cstring>
#include "util/Math.h"


namespace orxonox{

uint8_t SynchronisableVariableBase::state_ = 0;



// =================== Template specialisation stuff =============

// =========== bool

template <> uint32_t SynchronisableVariable<const bool>::returnSize()
{
  return sizeof(uint8_t);
}

template <> void SynchronisableVariable<const bool>::setAndIncrease( uint8_t*& mem )
{
  *(uint8_t*)(&this->variable_) = *static_cast<uint8_t*>(mem);
  mem += SynchronisableVariable<const bool>::returnSize();
}

template <> void SynchronisableVariable<const bool>::getAndIncrease( uint8_t*& mem )
{
  *static_cast<uint8_t*>(mem) = *(uint8_t*)(&this->variable_);
  mem += SynchronisableVariable<const bool>::returnSize();
}

template <> bool SynchronisableVariable<const bool>::checkEquality( uint8_t* mem )
{
  return *static_cast<uint8_t*>(mem) == *(uint8_t*)(&this->variable_);
}

// =========== char

template <> uint32_t SynchronisableVariable<const char>::returnSize()
{
  return sizeof(uint8_t);
}

template <> void SynchronisableVariable<const char>::setAndIncrease( uint8_t*& mem )
{
  *(uint8_t*)(&this->variable_) = *static_cast<uint8_t*>(mem);
  mem += SynchronisableVariable<const char>::returnSize();
}

template <> void SynchronisableVariable<const char>::getAndIncrease( uint8_t*& mem )
{
  *static_cast<uint8_t*>(mem) = *(uint8_t*)(&this->variable_);
  mem += SynchronisableVariable<const char>::returnSize();
}

template <> bool SynchronisableVariable<const char>::checkEquality( uint8_t* mem )
{
  return *static_cast<uint8_t*>(mem) == *(uint8_t*)(&this->variable_);
}

// =========== unsigned char

template <> uint32_t SynchronisableVariable<const unsigned char>::returnSize()
{
  return sizeof(uint8_t);
}

template <> void SynchronisableVariable<const unsigned char>::setAndIncrease( uint8_t*& mem )
{
  *(uint8_t*)(&this->variable_) = *static_cast<uint8_t*>(mem);
  mem += SynchronisableVariable<const unsigned char>::returnSize();
}

template <> void SynchronisableVariable<const unsigned char>::getAndIncrease( uint8_t*& mem )
{
  *static_cast<uint8_t*>(mem) = *(uint8_t*)(&this->variable_);
  mem += SynchronisableVariable<const unsigned char>::returnSize();
}

template <> bool SynchronisableVariable<const unsigned char>::checkEquality( uint8_t* mem )
{
  return *static_cast<uint8_t*>(mem) == *(uint8_t*)(&this->variable_);
}

// =========== short

template <> uint32_t SynchronisableVariable<const short>::returnSize()
{
  return sizeof(int16_t);
}

template <> void SynchronisableVariable<const short>::setAndIncrease( uint8_t*& mem )
{
  *(short*)(&this->variable_) = *(int16_t*)(mem);
  mem += SynchronisableVariable<const short>::returnSize();
}

template <> void SynchronisableVariable<const short>::getAndIncrease( uint8_t*& mem )
{
  *(int16_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const short>::returnSize();
}

template <> bool SynchronisableVariable<const short>::checkEquality( uint8_t* mem )
{
  return *(int16_t*)(mem) == static_cast<int16_t>(this->variable_);
}

// =========== unsigned short

template <> uint32_t SynchronisableVariable<const unsigned short>::returnSize()
{
  return sizeof(uint16_t);
}

template <> void SynchronisableVariable<const unsigned short>::setAndIncrease( uint8_t*& mem )
{
  *(unsigned short*)(&this->variable_) = *(uint16_t*)(mem);
  mem += SynchronisableVariable<const unsigned short>::returnSize();
}

template <> void SynchronisableVariable<const unsigned short>::getAndIncrease( uint8_t*& mem )
{
  *(uint16_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const unsigned short>::returnSize();
}

template <> bool SynchronisableVariable<const unsigned short>::checkEquality( uint8_t* mem )
{
  return *(uint16_t*)(mem) == this->variable_;
}

// =========== int

template <> uint32_t SynchronisableVariable<const int>::returnSize()
{
  return sizeof(int32_t);
}

template <> void SynchronisableVariable<const int>::setAndIncrease( uint8_t*& mem )
{
  *(int *)(&this->variable_) = *(int32_t*)(mem);
  mem += SynchronisableVariable<const int>::returnSize();
}

template <> void SynchronisableVariable<const int>::getAndIncrease( uint8_t*& mem )
{
  *(int32_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const int>::returnSize();
}

template <> bool SynchronisableVariable<const int>::checkEquality( uint8_t* mem )
{
  return *(int32_t*)(mem) == this->variable_;
}

// =========== unsigned int

template <> uint32_t SynchronisableVariable<const unsigned int>::returnSize()
{
  return sizeof(uint32_t);
}
    
template <> void SynchronisableVariable<const unsigned int>::setAndIncrease( uint8_t*& mem )
{
  *(unsigned int*)(&this->variable_) = *(uint32_t*)(mem);
  mem += SynchronisableVariable<const unsigned int>::returnSize();
}

template <> void SynchronisableVariable<const unsigned int>::getAndIncrease( uint8_t*& mem )
{
  *(uint32_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const unsigned int>::returnSize();
}

template <> bool SynchronisableVariable<const unsigned int>::checkEquality( uint8_t* mem )
{
  return *(uint32_t*)(mem) == this->variable_;
}

// =========== long

template <> uint32_t SynchronisableVariable<const long>::returnSize()
{
  return sizeof(int32_t);
}

template <> void SynchronisableVariable<const long>::setAndIncrease( uint8_t*& mem )
{
  *(long*)(&this->variable_) = *(int32_t*)(mem);
  mem += SynchronisableVariable<const long>::returnSize();
}

template <> void SynchronisableVariable<const long>::getAndIncrease( uint8_t*& mem )
{
  *(int32_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const long>::returnSize();
}

template <> bool SynchronisableVariable<const long>::checkEquality( uint8_t* mem )
{
  return *(int32_t*)(mem) == this->variable_;
}

// =========== unsigned long

template <> uint32_t SynchronisableVariable<const unsigned long>::returnSize()
{
  return sizeof(uint32_t);
}

template <> void SynchronisableVariable<const unsigned long>::setAndIncrease( uint8_t*& mem )
{
  *(unsigned long*)(&this->variable_) = *(uint32_t*)(mem);
  mem += SynchronisableVariable<const unsigned long>::returnSize();
}

template <> void SynchronisableVariable<const unsigned long>::getAndIncrease( uint8_t*& mem )
{
  *(uint32_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const unsigned long>::returnSize();
}

template <> bool SynchronisableVariable<const unsigned long>::checkEquality( uint8_t* mem )
{
  return *(uint32_t*)(mem) == this->variable_;
}

// =========== long long

template <> uint32_t SynchronisableVariable<const long long>::returnSize()
{
  return sizeof(int64_t);
}

template <> void SynchronisableVariable<const long long>::setAndIncrease( uint8_t*& mem )
{
  *(long long*)(&this->variable_) = *(int64_t*)(mem);
  mem += SynchronisableVariable<const long long>::returnSize();
}

template <> void SynchronisableVariable<const long long>::getAndIncrease( uint8_t*& mem )
{
  *(int64_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const long long>::returnSize();
}

template <> bool SynchronisableVariable<const long long>::checkEquality( uint8_t* mem )
{
  return *(int64_t*)(mem) == this->variable_;
}

// =========== unsigned long long

template <> uint32_t SynchronisableVariable<const unsigned long long>::returnSize()
{
  return sizeof(uint64_t);
}

template <> void SynchronisableVariable<const unsigned long long>::setAndIncrease( uint8_t*& mem )
{
  *(unsigned long long*)(&this->variable_) = *(uint64_t*)(mem);
  mem += SynchronisableVariable<const unsigned long long>::returnSize();
}

template <> void SynchronisableVariable<const unsigned long long>::getAndIncrease( uint8_t*& mem )
{
  *(uint64_t*)(mem) = this->variable_;
  mem += SynchronisableVariable<const unsigned long long>::returnSize();
}

template <> bool SynchronisableVariable<const unsigned long long>::checkEquality( uint8_t* mem )
{
  return *(uint64_t*)(mem) == this->variable_;
}

// =========== float

template <> uint32_t SynchronisableVariable<const float>::returnSize()
{
  return sizeof(uint32_t);
}

template <> void SynchronisableVariable<const float>::setAndIncrease( uint8_t*& mem )
{
  *(uint32_t*)(&this->variable_) = *(uint32_t*)(mem);
  mem += SynchronisableVariable<const float>::returnSize();
}

template <> void SynchronisableVariable<const float>::getAndIncrease( uint8_t*& mem )
{
  *(uint32_t*)(mem) = *(uint32_t*)(&this->variable_);
  mem += SynchronisableVariable<const float>::returnSize();
}

template <> bool SynchronisableVariable<const float>::checkEquality( uint8_t* mem )
{
  return *(uint32_t*)(mem) == *(uint32_t*)(&this->variable_);
}

// =========== double

template <> uint32_t SynchronisableVariable<const double>::returnSize()
{
  return sizeof(uint64_t);
}

template <> void SynchronisableVariable<const double>::setAndIncrease( uint8_t*& mem )
{
  *(uint64_t*)(&this->variable_) = *(uint64_t*)(mem);
  mem += SynchronisableVariable<const double>::returnSize();
}

template <> void SynchronisableVariable<const double>::getAndIncrease( uint8_t*& mem )
{
  *(uint64_t*)(mem) = *(uint64_t*)(&this->variable_);
  mem += SynchronisableVariable<const double>::returnSize();
}

template <> bool SynchronisableVariable<const double>::checkEquality( uint8_t* mem )
{
  return *(uint64_t*)(mem) == *(uint64_t*)(&this->variable_);
}

// =========== long double

template <> uint32_t SynchronisableVariable<const long double>::returnSize()
{
  return sizeof(uint64_t);
}

template <> void SynchronisableVariable<const long double>::setAndIncrease( uint8_t*& mem )
{
  double temp;
  memcpy(&temp, mem, sizeof(uint64_t));
  *(long double*)(&this->variable_) = static_cast<const long double>(temp);
  mem += SynchronisableVariable<const long double>::returnSize();
}

template <> void SynchronisableVariable<const long double>::getAndIncrease( uint8_t*& mem )
{
  double temp = static_cast<double>(this->variable_);
  memcpy(mem, &temp, sizeof(uint64_t));
  mem += SynchronisableVariable<const long double>::returnSize();
}

template <> bool SynchronisableVariable<const long double>::checkEquality( uint8_t* mem )
{
  double temp = static_cast<double>(this->variable_);
  return memcmp(&temp, mem, sizeof(uint64_t))==0;
}

// =========== string

template <> uint32_t SynchronisableVariable<const std::string>::returnSize()
{
  return variable_.length()+1;
}

template <> void SynchronisableVariable<const std::string>::getAndIncrease( uint8_t*& mem )
{
  memcpy(mem, this->variable_.c_str(), this->variable_.length()+1);
  mem += this->variable_.length()+1;
}

template <> void SynchronisableVariable<const std::string>::setAndIncrease( uint8_t*& mem )
{
  *(std::string*)(&this->variable_) = std::string((const char *)mem);
  mem += this->variable_.length()+1;
}

template <> bool SynchronisableVariable<const std::string>::checkEquality( uint8_t* mem )
{
  return std::string((const char*)mem)==this->variable_;
}

// =========== Degree

template <> uint32_t SynchronisableVariable<const Degree>::returnSize()
{
  return sizeof(Ogre::Real);
}

template <> void SynchronisableVariable<const Degree>::getAndIncrease( uint8_t*& mem )
{
  Ogre::Real r = this->variable_.valueDegrees();
  memcpy(mem, &r, returnSize());
  mem += returnSize();
}

template <> void SynchronisableVariable<const Degree>::setAndIncrease( uint8_t*& mem )
{
  Ogre::Real* r = (Ogre::Real*)mem;
  (Degree&)this->variable_ = *r;
  mem += returnSize();
}

template <> bool SynchronisableVariable<const Degree>::checkEquality( uint8_t* mem )
{
  Ogre::Real* r = (Ogre::Real*)mem;
  return this->variable_==Degree(*r);
}

}
