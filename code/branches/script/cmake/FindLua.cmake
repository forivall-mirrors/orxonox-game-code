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
	/usr/local/include/lua5.1)

FIND_PATH(Lua_INCLUDE_DIR lauxlib.h
	/usr/include/lua5.1
	/usr/local/include/lua5.1)

FIND_PATH(Lua_INCLUDE_DIR lualib.h
	/usr/include/lua5.1
	/usr/local/include/lua5.1)

FIND_LIBRARY(Lua_LIBRARIES lua5.1 PATH
	/usr/lib
	/usr/local/lib)


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
