# Find CEGUI includes and library
#
# This module defines
#  CEGUI_INCLUDE_DIR
#  CEGUI_LIBRARIES, the libraries to link against to use CEGUI.
#  CEGUI_LIB_DIR, the location of the libraries
#  CEGUI_FOUND, If false, do not try to use CEGUI
#
# Created by Matt Williams to find OGRE libraries
# Copyright © 2007, Matt Williams
#
# Modified by Nicolas Schlumberger to find CEGUI libraries
# and make it run on the Tardis-Infrastucture of the ETH Zurich
# Copyright 2007, Nicolas Schlumberger
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
#                 > www.orxonox.net <

IF (CEGUI_LIBRARIES AND CEGUI_INCLUDE_DIR AND CEGUI_LIB_DIR AND CEGUI_SCRIPT_LIBRARIES)
    SET(CEGUI_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (CEGUI_LIBRARIES AND CEGUI_INCLUDE_DIR AND CEGUI_LIB_DIR AND CEGUI_SCRIPT_LIBRARIES)

IF (WIN32) #Windows
    FIND_PATH(CEGUI_INCLUDE_DIR CEGUI.h
        ../libs/cegui-0.6.1/include
    )

    SET(CEGUI_LIBRARIES debug CEGUIBase_d optimized CEGUIBase)
    FIND_LIBRARY(CEGUI_LIBDIR NAMES ${CEGUI_LIBRARIES} PATHS
        ../libs/cegui-0.6.1/bin
    )

    # Strip the filename from the path
    IF (CEGUI_LIBDIR)
        GET_FILENAME_COMPONENT(CEGUI_LIBDIR ${CEGUI_LIBDIR} PATH)
        SET (CEGUI_LIB_DIR ${CEGUI_LIBDIR} CACHE FILEPATH "")
    ENDIF (CEGUI_LIBDIR)

    IF (NOT CEGUI_SCRIPT_LIBDIR)
        # Search Lua script module
        SET(CEGUI_SCRIPT_LIBRARIES "CEGUILuaScriptModule")
        FIND_LIBRARY(CEGUI_SCRIPT_LIBDIR NAMES ${CEGUI_SCRIPT_LIBRARIES} PATHS
            ../libs/cegui-0.6.1/bin
        )
        IF (NOT CEGUI_SCRIPT_LIBDIR)
            SET(CEGUI_SCRIPT_LIBRARIES "CEGUILua")
            FIND_LIBRARY(CEGUI_SCRIPT_LIBDIR NAMES ${CEGUI_SCRIPT_LIBRARIES} PATHS
                ../libs/cegui-0.6.1/bin
            )
            IF (NOT CEGUI_SCRIPT_LIBDIR)
                SET(CEGUI_SCRIPT_LIBRARIES)
            ENDIF (NOT CEGUI_SCRIPT_LIBDIR)
        ENDIF (NOT CEGUI_SCRIPT_LIBDIR)
    ENDIF (NOT CEGUI_SCRIPT_LIBDIR)

    # Strip the filename from the path
    IF (CEGUI_SCRIPT_LIBDIR)
        GET_FILENAME_COMPONENT(CEGUI_SCRIPT_LIBDIR ${CEGUI_SCRIPT_LIBDIR} PATH)
        SET (CEGUI_SCRIPT_LIB_DIR ${CEGUI_SCRIPT_LIBDIR} CACHE FILEPATH "")
    ENDIF (CEGUI_SCRIPT_LIBDIR)

ELSE (WIN32) #Unix
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(CEGUI CEGUI /usr/pack/cegui-0.5.0-sd/i686-debian-linux3.1/lib/pkgconfig/CEGUI.pc) # tardis specific hack
    SET(CEGUI_INCLUDE_DIR ${CEGUI_INCLUDE_DIRS})
    SET(CEGUI_LIB_DIR ${CEGUI_LIBDIR})
    SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES} CACHE STRING "")
    SET(CEGUI_SCRIPT_LIBRARIES "CEGUILuaScriptModule")
    SET(CEGUI_SCRIPT_LIB_DIR)
ENDIF (WIN32)

IF (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARIES AND CEGUI_LIB_DIR AND CEGUI_SCRIPT_LIBRARIES)
    SET(CEGUI_FOUND TRUE)

    #Do some preparation
    SEPARATE_ARGUMENTS(CEGUI_INCLUDE_DIR)
    SEPARATE_ARGUMENTS(CEGUI_LIBRARIES)

    SET(CEGUI_INCLUDE_DIR ${CEGUI_INCLUDE_DIR} CACHE PATH "")
    SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES} CACHE STRING "")
    SET(CEGUI_LIB_DIR ${CEGUI_LIB_DIR} CACHE PATH "")
    SET(CEGUI_SCRIPT_LIBRARIES ${CEGUI_SCRIPT_LIBRARIES} CACHE PATH "")
ENDIF (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARIES AND CEGUI_LIB_DIR AND CEGUI_SCRIPT_LIBRARIES)

IF (CEGUI_FOUND)
    # check CEGUIVersion.h to set the version macro
    FILE(READ "${CEGUI_INCLUDE_DIR}/CEGUIVersion.h" ceguiVersionFile)
    STRING(REGEX MATCH "\\#define CEGUI_VERSION_MINOR ([56])" "\\1" CEGUI_VERSION ${ceguiVersionFile})
    MESSAGE(STATUS "${CEGUI_VERSION}")
    IF (NOT CEGUI_FIND_QUIETLY)
        MESSAGE(STATUS "CEGUI was found.")
        IF (VERBOSE_FIND)
            MESSAGE (STATUS "  include path: ${CEGUI_INCLUDE_DIR}")
            MESSAGE (STATUS "  library path: ${CEGUI_LIB_DIR}")
            MESSAGE (STATUS "  libraries:    ${CEGUI_LIBRARIES}")
            MESSAGE (STATUS "           :    ${CEGUI_SCRIPT_LIBRARIES}")
        ENDIF (VERBOSE_FIND)
    ENDIF (NOT CEGUI_FIND_QUIETLY)
ELSE (CEGUI_FOUND)
    IF (NOT CEGUI_INCLUDE_DIR)
        MESSAGE(SEND_ERROR "CEGUI include path was not found.")
    ENDIF (NOT CEGUI_INCLUDE_DIR)
    IF (NOT CEGUI_LIB_DIR)
        MESSAGE(SEND_ERROR "CEGUI library was not found.")
    ENDIF (NOT CEGUI_LIB_DIR)
    IF (NOT CEGUI_LIBRARIES)
        MESSAGE(SEND_ERROR "CEGUI libraries not known.")
    ENDIF (NOT CEGUI_LIBRARIES)
    IF (NOT CEGUI_SCRIPT_LIBRARIES)
        MESSAGE(SEND_ERROR "CEGUI Lua script module was not found.")
    ENDIF (NOT CEGUI_SCRIPT_LIBRARIES)
ENDIF (CEGUI_FOUND)

