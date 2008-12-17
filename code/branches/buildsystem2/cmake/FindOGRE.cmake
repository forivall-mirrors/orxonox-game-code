# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARY, the library to link against to use OGRE.
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright © 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture of the ETH Zurich
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli
#                 > www.orxonox.net <

FIND_PATH(OGRE_INCLUDE_DIR Ogre.h
    PATHS
    $ENV{OGREDIR}
    /usr/local
    /usr
    PATH_SUFFIXES include include/OGRE
)

IF(WIN32)
    FIND_LIBRARY(OGRE_LIBRARY_
        NAMES OgreMain
        PATHS
        $ENV{OGREDIR}
        PATH_SUFFIXES lib
    )
    FIND_LIBRARY(OGRE_LIBRARY_d
        NAMES OgreMain_d
        PATHS
        $ENV{OGREDIR}
        PATH_SUFFIXES lib
    )
    IF(OGRE_LIBRARY_)
        IF(OGRE_LIBRARY_d)
            SET(OGRE_LIBRARY optimized ${OGRE_LIBRARY_} debug ${OGRE_LIBRARY_d})
        ELSE(OGRE_LIBRARY_d)
            SET(OGRE_LIBRARY ${OGRE_LIBRARY_})
        ENDIF(OGRE_LIBRARY_d)
    ENDIF(OGRE_LIBRARY_)
ELSE(WIN32)
    FIND_LIBRARY(OGRE_LIBRARY
        NAMES OgreMain
        PATHS
        $ENV{OGREDIR}
        /usr/local
        /usr
        PATH_SUFFIXES lib
    )
ENDIF(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGRE DEFAULT_MSG
    OGRE_LIBRARY
    OGRE_INCLUDE_DIR
)

MARK_AS_ADVANCED(
    OGRE_LIBRARY
    OGRE_LIBRARY_
    OGRE_LIBRARY_d
    OGRE_INCLUDE_DIR
)
