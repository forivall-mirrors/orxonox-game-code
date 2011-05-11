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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @ingroup Util
*/

#ifndef _OrxEnum_H__
#define _OrxEnum_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    /** Lightweight enumeration class that can be extended at run time.
    @details
        The class accepts type int and also defines operator int(). Therefore
        int and OrxEnum can be used interchangeably so you can extend the content
        of the enumeration at run time by adding ints.
    @par Declaring an OrxEnum
        Write a struct that inherits OrxEnum and use some macros:
        @code
        struct MyEnum : OrxEnum<MyEnum>
        {
            OrxEnumConstructors(MyEnum);

            static const int Value1 = -1;
            static const int Value2 = 0;
            static const int Value3 = Value2 + 10;
        };
        @endcode
    */
    template <class T>
    struct OrxEnum
    {
        public:
            OrxEnum() { }
            OrxEnum(int type)            { type_ = type; }
            OrxEnum(const T& instance)   { type_ = instance.type_; }

            operator int()               { return type_; }
            T& operator =(int type)      { type_ = type; return *this; }
            bool operator <(const T& right) const { return (type_ < right.type_); }
            bool operator >(const T& right) const { return (type_ > right.type_); }

        private:
            OrxEnum(const OrxEnum& instance);
            int type_;
    };
}

/// See orxonox::OrxEnum for more info
#define OrxEnumConstructors(enumName)                        \
enumName() { }                                               \
enumName(int type) : OrxEnum<enumName>(type)             { } \
enumName(const enumName& inst) : OrxEnum<enumName>(inst) { } \
void dummyFunction()

#endif /* _OrxEnum_H__ */
