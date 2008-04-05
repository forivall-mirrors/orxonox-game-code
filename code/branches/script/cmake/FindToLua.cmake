# Find Lua includes and library
#
# This module defines
#  ToLua_INCLUDE_DIR
#  ToLua_LIBRARIES, the libraries to link against to use Lua.
#  ToLua_LIB_DIR, the location of the libraries
#  ToLua_FOUND, If false, do not try to use Lua
#
# Copyright © 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF (ToLua_LIBRARIES)
    SET(ToLua_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (ToLua_LIBRARIES)

FIND_LIBRARY(ToLua_LIBRARIES tolua++5.1
	/usr/lib
	/usr/local/lib)

IF (ToLua_LIBRARIES)
    SET(ToLua_FOUND TRUE)
ENDIF (ToLua_LIBRARIES)

IF (ToLua_FOUND)
    MESSAGE(STATUS "Found ToLua: ${ToLua_LIBRARIES}")
ELSE (ToLua_FOUND)
    IF (ToLua_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find ToLua")
    ENDIF (ToLua_FIND_REQUIRED)
ENDIF (ToLua_FOUND)
