INCLUDE_DIRECTORIES(.)

SET( LUA_SRC_FILES
  lapi.c
  lauxlib.c
  lbaselib.c
  lcode.c
  ldblib.c
  ldebug.c
  ldo.c
  ldump.c
  lfunc.c
  lgc.c
  liolib.c
  llex.c
  lmathlib.c
  lmem.c
  loadlib.c
  lobject.c
  lopcodes.c
  lparser.c
  lstate.c
  lstring.c
  lstrlib.c
  ltable.c
  ltablib.c
  ltests.c
  ltm.c
  lundump.c
  lvm.c
  lzio.c
)

ADD_LIBRARY( lua_orxonox SHARED ${LUA_SRC_FILES} )
