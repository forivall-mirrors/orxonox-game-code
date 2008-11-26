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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

/**
 @file
 @brief Representation of an interface to lua
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Script_H__
#define _Script_H__

#include "CorePrereqs.h"

extern "C" {
#include "lua.h"
}

#include <list>
#include <string>

// tolua_begin
namespace orxonox
{
  class _CoreExport Script
  {
// tolua_end
    struct LoadS {
      const char *s;
      size_t size;
    };

    public:
      inline static Script* getInstance() { if (!Script::singletonRef) Script::singletonRef = new Script(); return Script::singletonRef; } // tolua_export
      inline ~Script() { Script::singletonRef = NULL; };

    void loadFile(std::string filename, bool luaTags);
    //void init(lua_State *state_);
    //void xmlToLua();
    void run();
    void luaPrint(std::string str); // tolua_export

#if LUA_VERSION_NUM != 501
    static const char * lua_Chunkreader(lua_State *L, void *data, size_t *size);
#endif

    inline lua_State* getLuaState() { return luaState_; };
    inline std::string getLuaOutput() { return output_; };
    //inline std::string* getFileString() { return &fileString_; };
    inline void clearLuaOutput() { output_ = ""; }

    std::string replaceLuaTags(const std::string& text);

    private:
      Script();
      static Script* singletonRef;

      std::string luaSource_;
      std::string output_;
      lua_State* luaState_;

  }; // tolua_export
} // tolua_export
#endif /* _Script_H__ */
