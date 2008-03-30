/**
 @file  script.h
 @brief Representation of a lua script
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Script_H__
#define _Script_H__

extern "C" {
#include "lua.h"
}

#include <list>
#include <string>

namespace orxonox
{

  class Script
  {
    Script();
    ~Script();

    void loadFile(std::string filename);
    void xmlToLua();

    inline lua_State* getLuaState() { return state_; };

    private:

      //std::list<std::string>& getLevelFileLines();

      std::list<std::string> fileLines_;
      lua_State* state_;

  };
}
#endif /* _Script_H__ */
