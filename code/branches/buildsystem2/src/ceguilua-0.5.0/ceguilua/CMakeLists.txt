SET( CEGUILUA_SRC_FILES
  CEGUILua.cpp
  CEGUILuaFunctor.cpp
  lua_CEGUI.cpp
  required.cpp
)

ADD_LIBRARY( ceguilua_orxonox SHARED ${CEGUILUA_SRC_FILES} )

TARGET_LINK_LIBRARIES(ceguilua_orxonox
  tolualib_orxonox
  ${LUA_LIBRARIES}
  ${CEGUI_LIBRARY}
)
