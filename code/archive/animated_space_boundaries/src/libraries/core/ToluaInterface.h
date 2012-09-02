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
@ingroup Lua
@brief
    This is required because tolua would parse this macro in LuaState.h and fail
*/

#ifndef _ToluaInterface_H__
#define _ToluaInterface_H__

#include "CorePrereqs.h"
#include <boost/preprocessor/cat.hpp>

// Macro for declaring a tolua interface of a library/module
#define DeclareToluaInterface(libraryName) \
    static bool BOOST_PP_CAT(bDummy##libraryName, __UNIQUE_NUMBER__) = orxonox::LuaState::addToluaInterface(&tolua_##libraryName##_open, #libraryName); \
    static Loki::ScopeGuardImpl1<bool (*)(const std::string&), std::string> BOOST_PP_CAT(dummy##libraryName, __UNIQUE_NUMBER__)(&orxonox::LuaState::removeToluaInterface, #libraryName)

#endif /* _ToluaInterface_H__ */
