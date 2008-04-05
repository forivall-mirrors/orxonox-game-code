/**
 @file  script.h
 @brief Representation of an interface to lua
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Script_H__
#define _Script_H__

extern "C" {
#include "lua.h"
}

#include <list>
#include <string>

namespace orxonox // tolua_export
{ // tolua_export

  class Script // tolua_export
  { // tolua_export
    public:
    Script(); // tolua_export
    ~Script();

    static void loadFile(std::string filename, bool luaTags);
    static void init(lua_State *state_);
    //void xmlToLua();
    static void run();
    void luaPrint(std::string str); // tolua_export

    inline static lua_State* getLuaState() { return luaState_; };
    inline static std::string* getFileString() { return &fileString_; };

    static unsigned int getNextQuote(const std::string& text, unsigned int start);
    static std::string replaceLuaTags(const std::string& text);

    private:

      //std::list<std::string>& getLevelFileLines();

      static std::string fileString_;
      static lua_State* luaState_;

  }; // tolua_export
} // tolua_export
#endif /* _Script_H__ */
