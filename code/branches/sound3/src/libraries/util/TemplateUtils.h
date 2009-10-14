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
    @brief Declaration of various helper templates.
*/

#ifndef _TemplateUtils_H__
#define _TemplateUtils_H__

#include "UtilPrereqs.h"

namespace orxonox
{
    ///////////////////////////////////////////////////
    // Static detection of implicit type conversions //
    ///////////////////////////////////////////////////

// disable warnings about possible loss of data
#ifdef ORXONOX_COMPILER_MSVC
#  pragma warning(push)
#  pragma warning(disable:4244)
#endif
// GCC generates warnings when implicitely casting from float to int for instance.
#ifdef ORXONOX_COMPILER_GCC
#  pragma GCC system_header
#endif

    /**
    @brief
        Detects whether you can implicitely cast FromType to ToType.

        Usage: ImplicitConversion<FromType, ToType>::exists
        This gives you a compile time constant boolean in the form of an enum value.
    @note
        The idea to use the sizeof() operator on return values to determine function existance
        is described in 'Modern C++ design' by Alexandrescu (2001).
    */
    template <class FromType, class ToType>
    class ImplicitConversion
    {
    private:
        ImplicitConversion(); ImplicitConversion(const ImplicitConversion&); ~ImplicitConversion();
        // Gets chosen only if there is an implicit conversion from FromType to ToType.
        static char test(ToType);
        // Accepts any argument. Why do we not use a template? The reason is that with templates,
        // the function above is only taken iff it is an exact type match. But since we want to
        // check for implicit conversion, we have to use the ellipsis.
        static long long test(...);
        static FromType object; // helper object to handle private c'tor and d'tor
    public:
        // test(object) only has 'long long' return type iff the compiler doesn't choose test(...)
        enum { exists = (sizeof(test(object)) == sizeof(char)) };
    };

#ifdef ORXONOX_COMPILER_MSVC
#  pragma warning(pop)
#endif

}

#endif /* _TemplateUtils_H__ */
