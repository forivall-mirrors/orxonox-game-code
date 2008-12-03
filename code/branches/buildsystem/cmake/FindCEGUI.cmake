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

# TODO: Determine CEGUI version

FIND_PATH(CEGUI_INCLUDE_DIR CEGUI.h
    PATHS
    $ENV{CEGUIDIR}
    /usr/local
    /usr
    PATH_SUFFIXES include include/CEGUI
)

IF(WIN32)
    FIND_LIBRARY(CEGUI_LIBRARY_
        NAMES CEGUIBase
        PATHS
        $ENV{CEGUIDIR}
        PATH_SUFFIXES lib
    )
    FIND_LIBRARY(CEGUI_LIBRARY_d
        NAMES CEGUIBase_d
        PATHS
        $ENV{CEGUIDIR}
        PATH_SUFFIXES lib
    )
    IF(CEGUI_LIBRARY_)
        IF(CEGUI_LIBRARY_d)
            SET(CEGUI_LIBRARY optimized ${CEGUI_LIBRARY_} debug ${CEGUI_LIBRARY_d})
        ELSE(CEGUI_LIBRARY_d)
            SET(CEGUI_LIBRARY ${CEGUI_LIBRARY_})
        ENDIF(CEGUI_LIBRARY_d)
    ENDIF(CEGUI_LIBRARY_)
ELSE(WIN32)
    FIND_LIBRARY(CEGUI_LIBRARY
        NAMES CEGUIBase
        PATHS
        $ENV{CEGUIDIR}
        /usr/local
        /usr
        PATH_SUFFIXES lib
    )
ENDIF(WIN32)

#    IF (NOT CEGUI_SCRIPT_LIBDIR)
#        # Search Lua script module
#        SET(CEGUI_SCRIPT_LIBRARIES "CEGUILuaScriptModule")
#        FIND_LIBRARY(CEGUI_SCRIPT_LIBDIR NAMES ${CEGUI_SCRIPT_LIBRARIES} PATHS
#            ../libs/cegui-0.6.1/bin
#        )
#	IF (NOT CEGUI_SCRIPT_LIBDIR)
#            SET(CEGUI_SCRIPT_LIBRARIES "CEGUILua")
#            FIND_LIBRARY(CEGUI_SCRIPT_LIBDIR NAMES ${CEGUI_SCRIPT_LIBRARIES} PATHS
#                ../libs/cegui-0.6.1/bin
#            )
#            IF (NOT CEGUI_SCRIPT_LIBDIR)
#                SET(CEGUI_SCRIPT_LIBRARIES)
#            ENDIF (NOT CEGUI_SCRIPT_LIBDIR)
#        ENDIF (NOT CEGUI_SCRIPT_LIBDIR)
#    ENDIF (NOT CEGUI_SCRIPT_LIBDIR)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEGUI DEFAULT_MSG
    CEGUI_LIBRARY
    CEGUI_INCLUDE_DIR
    #CEGUI_SCRIPT_LIBRARIES
)

MARK_AS_ADVANCED(
    CEGUI_LIBRARY
    CEGUI_LIBRARY_
    CEGUI_LIBRARY_d
    CEGUI_INCLUDE_DIR
    #CEGUI_SCRIPT_LIBRARIES
)
