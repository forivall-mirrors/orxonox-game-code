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

namespace orxonox
{

  class Script
  {
    Script();
    ~Script();

    inline lua_State* getLuaState() { return state_; };

    private:
      lua_State* state_;

  };
}
#endif /* _Script_H__ */
