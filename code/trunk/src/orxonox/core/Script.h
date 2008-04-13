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
      inline static Script* getInstance() { if (!Script::singletonRef) Script::singletonRef = new Script(); return Script::singletonRef; } // tolua_export
      inline ~Script() { Script::singletonRef = NULL; };

    void loadFile(std::string filename, bool luaTags);
    //void init(lua_State *state_);
    //void xmlToLua();
    void run();
    void luaPrint(std::string str); // tolua_export

#if LUA_VERSION_NUM != 501
    inline static const char * lua_Chunkreader(lua_State *L, void *data, size_t *size) { return NULL;};
#endif

    inline lua_State* getLuaState() { return luaState_; };
    inline std::string getLuaOutput() { return output_; };
    //inline std::string* getFileString() { return &fileString_; };

    unsigned int getNextQuote(const std::string& text, unsigned int start);
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
