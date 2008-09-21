INCLUDE_DIRECTORIES(.)

SET( CEGUILUA_SRC_FILES
  CEGUILua.cpp
  CEGUILuaFunctor.cpp
  lua_CEGUI.cpp
  required.cpp
)

ADD_LIBRARY( ceguilua_orxonox SHARED ${LUA_SRC_FILES} )
