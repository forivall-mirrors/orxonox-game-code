# Find CEGUI includes and library
#
# This module defines
#  CEGUI_INCLUDE_DIR
#  CEGUI_LIBRARY, the library to link against to use CEGUI.
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
# Lots of simplifications by Adrian Friedli
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(CEGUI_INCLUDE_DIR CEGUI.h
    PATHS
    $ENV{CEGUIDIR}
    /usr/local
    /usr
    PATH_SUFFIXES include include/CEGUI
)
FIND_LIBRARY(CEGUI_LIBRARY_OPTIMIZED
    NAMES CEGUIBase
    PATHS
    $ENV{CEGUIDIR}
    /usr/local
    /usr
    PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUI_LIBRARY_DEBUG
    NAMES CEGUIBase${LIBRARY_DEBUG_POSTFIX}
    PATHS
    $ENV{CEGUIDIR}
    /usr/local
    /usr
    PATH_SUFFIXES lib bin
)

# Look in CEGUIVersion.h for the version number
INCLUDE(DetermineVersion)
DETERMINE_VERSION(CEGUI ${CEGUI_INCLUDE_DIR}/CEGUIVersion.h)
# STRLESS can be dangerous since it only compares strings.
# Unfortunately VERSION_LESS is only provided since CMake v2.6.2
IF(${CEGUI_VERSION} STRLESS "0.5.0")
   MESSAGE(FATAL_ERROR "Minimum CEGUI version required is 0.5.0")
ENDIF(${CEGUI_VERSION} STRLESS "0.5.0")


#    IF (NOT CEGUI_SCRIPT_LIBDIR)
#        # Search Lua script module
#        SET(CEGUI_SCRIPT_LIBRARIES "CEGUILuaScriptModule")
#        FIND_LIBRARY(CEGUI_SCRIPT_LIBDIR NAMES ${CEGUI_SCRIPT_LIBRARIES} PATHS
#            ../libs/cegui-0.6.1/bin
#        )
#    IF (NOT CEGUI_SCRIPT_LIBDIR)
#            SET(CEGUI_SCRIPT_LIBRARIES "CEGUILua")
#            FIND_LIBRARY(CEGUI_SCRIPT_LIBDIR NAMES ${CEGUI_SCRIPT_LIBRARIES} PATHS
#                ../libs/cegui-0.6.1/bin
#            )
#            IF (NOT CEGUI_SCRIPT_LIBDIR)
#                SET(CEGUI_SCRIPT_LIBRARIES)
#            ENDIF (NOT CEGUI_SCRIPT_LIBDIR)
#        ENDIF (NOT CEGUI_SCRIPT_LIBDIR)
#    ENDIF (NOT CEGUI_SCRIPT_LIBDIR)


FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEGUI DEFAULT_MSG
    CEGUI_LIBRARY_OPTIMIZED
    CEGUI_INCLUDE_DIR
    #CEGUI_SCRIPT_LIBRARIES
)

# Set optimized and debug libraries
HANDLE_LIBRARY_TYPES(CEGUI)

MARK_AS_ADVANCED(
    CEGUI_LIBRARY
    CEGUI_LIBRARY_OPTIMIZED
    CEGUI_LIBRARY_DEBUG
    CEGUI_INCLUDE_DIR
    #CEGUI_SCRIPT_LIBRARIES
)
