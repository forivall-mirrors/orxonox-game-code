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

#ifndef _mbool_H__
#define _mbool_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    struct _UtilExport mbool
    {
        public:
            inline mbool(bool value = false)
                { this->value_.memory_ = 0; this->value_.bool_ = value; }
            inline mbool(const mbool& value)
                { this->value_.memory_ = value.value_.memory_; }

            inline const mbool& operator=(bool value)
                { if (value != this->value_.bool_) { ++this->value_.memory_; } return (*this); }
            inline const mbool& operator=(const mbool& value)
                { this->value_.memory_ = value.value_.memory_; return (*this); }

            inline mbool& operator++()
                { ++this->value_.memory_; return (*this); }
            inline mbool operator++(int i)
                { mbool temp = (*this); ++this->value_.memory_; return temp; }

            inline operator bool() const
                { return this->value_.bool_; }

            inline bool operator==(bool other) const
                { return this->value_.bool_ == other; }
            inline bool operator!=(bool other) const
                { return this->value_.bool_ != other; }

            inline bool operator==(const mbool& other) const
                { return this->value_.memory_ == other.value_.memory_; }
            inline bool operator!=(const mbool& other) const
                { return this->value_.memory_ != other.value_.memory_; }

            inline bool operator!() const
                { return (!this->value_.bool_); }

            inline unsigned char& getMemory(){ return value_.memory_; }

        private:
            union
            {
                bool bool_ : 1;
                unsigned char memory_;
            } value_;
    };
}

#endif /* _mbool_H__ */
