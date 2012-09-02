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
    @ingroup Util
    @brief Declaration and implementation of the @ref orxonox::mbool class.
*/

#ifndef _mbool_H__
#define _mbool_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    /**
        @brief mbool is a small helper class that acts like a bool, but keeps track of the number of its state changes.

        The mbool class acts like a bool, but it has an internal counter that counts
        the number state changes (i.e. when the bool changes from true to false or
        back). This is used in the network if a boolean value is synchronized, because
        if a value changes quickly from false to true and back in the same tick, the
        clients will never be notified of this action. By using mbool however this
        behaviour is fixed, which is important for triggers and other objects.

        @note This is efficiently solved by using a union that combines a counter and a
        boolean bitfield of size 1. The boolean value corresponds always to the first
        bit of the counter - this means, if the counter is incremented, the boolean state
        changes. On the other hand, if you want to change the state, you can simply increase
        the counter.
    */
    struct mbool
    {
        public:
            /// Constructor: Creates the mbool and initializes the boolean value (default to false).
            inline mbool(bool value = false)
                { this->value_.memory_ = 0; this->value_.bool_ = value; }
            /// Copy-constructor, copies state and memory.
            inline mbool(const mbool& value)
                { this->value_.memory_ = value.value_.memory_; }
            /// Destructor does nothing but not defining it might create a symbol (class is header only)
            inline ~mbool()
                { }

            /// Assigns a boolean value (and increases the memory value if the value is different to the old value).
            inline mbool& operator=(bool value)
                { if (value != this->value_.bool_) { ++this->value_.memory_; } return (*this); }
            /// Assigns another mbool, copies state and memory.
            inline mbool& operator=(const mbool& value)
                { this->value_.memory_ = value.value_.memory_; return (*this); }

            /// Increases the memory which also inverts it's state (++mbool).
            inline mbool& operator++()
                { ++this->value_.memory_; return (*this); }
            /// Increases the memory which also inverts it's state (mbool++).
            inline mbool operator++(int)
                { mbool temp = (*this); ++this->value_.memory_; return temp; }

            /// Implicitly converts the mbool to a bool.
            inline operator bool() const
                { return this->value_.bool_; }

            /// Compares the mbool to a bool, returns true if the bool has the same value as the state of the mbool.
            inline bool operator==(bool other) const
                { return this->value_.bool_ == other; }
            /// Compares the mbool to a bool, returns true if the bool has a different value than the state of the mbool.
            inline bool operator!=(bool other) const
                { return this->value_.bool_ != other; }

            /// Compares two mbools, returns true if their memory matches.
            inline bool operator==(const mbool& other) const
                { return this->value_.memory_ == other.value_.memory_; }
            /// Compares two mbools, returns true if they have a different memory value.
            inline bool operator!=(const mbool& other) const
                { return this->value_.memory_ != other.value_.memory_; }

            /// Returns the inverted state of the bool (doesn't change the internal state).
            inline bool operator!() const
                { return (!this->value_.bool_); }

            /// Returns the memory value.
            inline unsigned char& getMemory(){ return value_.memory_; }

        private:
            union
            {
                bool bool_ : 1;         ///< The boolean state of the mbool, is located on the first bit of the memory variable
                unsigned char memory_;  ///< The memory of the mbool, counts the state-changes (and the first bit represents also the boolean value)
            } value_;                   ///< A union containing the state and the memory of the mbool
    };
}

#endif /* _mbool_H__ */
