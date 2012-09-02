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

#ifndef _TriBool_H__
#define _TriBool_H__

#include "UtilPrereqs.h"

// tolua_begin
namespace orxonox
{
    /** Sort of a boolean value that also has state \c Dontcare
    @remarks
        Even though \c False has the value 0, both \c True and \c Dontcare have
        a value other than 0. Keep that in mind when using TriBools in if statements.
    */
    namespace TriBool
    {
        enum Value
        {
            False    =  0,
            True     =  1,
            Dontcare =  2
        };
    }
}
// tolua_end

#endif /* _TriBool_H__ */
