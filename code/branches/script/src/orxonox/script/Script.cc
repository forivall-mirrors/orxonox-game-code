/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "Script.h"
extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}


namespace orxonox
{
  Script::Script()
  {
    state_ = lua_open();
#if Lua_VERSION == 51
    luaL_openlibs(state_);
#else
    luaopen_base(state_);
    luaopen_table(state_);
    luaopen_io(state_);
    luaopen_string(state_);
    luaopen_math(state_);
    luaopen_debug(state_);
#endif
  }

  Script::~Script()
  {

  }

}
