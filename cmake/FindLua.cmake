# Find Lua includes and library
#
# This module defines
#  Lua_INCLUDE_DIR
#  Lua_LIBRARIES, the libraries to link against to use Lua.
#  Lua_LIB_DIR, the location of the libraries
#  Lua_FOUND, If false, do not try to use Lua
#
# Copyright © 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF (Lua_LIBRARIES AND Lua_INCLUDE_DIR)
    SET(Lua_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (Lua_LIBRARIES AND Lua_INCLUDE_DIR)

FIND_PATH(Lua_INCLUDE_DIR lua.h
	/usr/include/lua5.1
	/usr/local/include/lua5.1
	../libs/lua-5.1.3/src)

FIND_LIBRARY(Lua_LIBRARIES lua5.1
	/usr/lib
	/usr/local/lib
	../libs/lua-5.1.3/lib)

IF (NOT Lua_INCLUDE_DIR)
FIND_PATH(Lua_INCLUDE_DIR lua.h
	/usr/include/lua50
	/usr/local/include/lua50
	/usr/pack/lua-5.0.3-sd/include)
ENDIF (NOT Lua_INCLUDE_DIR)

IF (NOT Lua_LIBRARIES)
FIND_LIBRARY(Lua_LIBRARIES lua50
	/usr/lib
	/usr/local/lib)

FIND_LIBRARY(Lua_LIBRARY lualib50
	/usr/lib
	/usr/local/lib)

SET(Lua_LIBRARIES ${Lua_LIBRARIES} ${Lua_LIBRARY})

ENDIF (NOT Lua_LIBRARIES)

IF (NOT Lua_LIBRARIES)
FIND_LIBRARY(Lua_LIBRARIES lua
	/usr/pack/lua-5.0.3-sd/i686-debian-linux3.1/lib)

FIND_LIBRARY(Lua_LIBRARY lualib
	/usr/pack/lua-5.0.3-sd/i686-debian-linux3.1/lib)

SET(Lua_LIBRARIES ${Lua_LIBRARIES} ${Lua_LIBRARY})

ENDIF (NOT Lua_LIBRARIES)

IF (Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FOUND TRUE)
ENDIF (Lua_INCLUDE_DIR AND Lua_LIBRARIES)

IF (Lua_FOUND)
    MESSAGE(STATUS "Found Lua: ${Lua_LIBRARIES}")
ELSE (Lua_FOUND)
    IF (Lua_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find Lua")
    ENDIF (Lua_FIND_REQUIRED)
ENDIF (Lua_FOUND)
