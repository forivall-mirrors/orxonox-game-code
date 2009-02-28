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

#ifndef _LuaBind_H__
#define _LuaBind_H__

#include "CorePrereqs.h"

extern "C" {
#include <lua.h>
}

#include <cassert>
#include <list>
#include <string>

// tolua_begin
namespace orxonox
{
  class _CoreExport LuaBind
  {

// tolua_end
    struct LoadS {
      const char *s;
      size_t size;
    };

    public:
      LuaBind();
      inline ~LuaBind() { assert(singletonRef_s); LuaBind::singletonRef_s = NULL; };

      inline static LuaBind& getInstance() { assert(singletonRef_s); return *LuaBind::singletonRef_s; } // tolua_export

    void loadFile(std::string filename, bool luaTags);
    void loadString(std::string code);
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

    std::string replaceLuaTags(const std::string& text); // tolua_export

    inline void setIncludePath(const std::string& includepath)
        { this->includePath_ = includepath; }

    private:
      static LuaBind* singletonRef_s;

      std::string luaSource_;
      std::string output_;
      lua_State* luaState_;
      bool isRunning_;
      std::string includePath_;

  }; // tolua_export
} // tolua_export
#endif /* _LuaBind_H__ */
