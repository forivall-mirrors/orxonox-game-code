# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARY, the library to link against to use OGRE.
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright � 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture of the ETH Zurich
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli
# Plugin checking by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(OGRE_INCLUDE_DIR Ogre.h
    PATHS $ENV{OGRE_HOME} /usr/local /usr
    PATH_SUFFIXES include include/OGRE
)
FIND_LIBRARY(OGRE_LIBRARY_OPTIMIZED
    NAMES OgreMain
    PATHS $ENV{OGRE_HOME} /usr/local /usr
    PATH_SUFFIXES lib bin/Release bin/release Release release
)
FIND_LIBRARY(OGRE_LIBRARY_DEBUG
    NAMES OgreMain${LIBRARY_DEBUG_POSTFIX}
    PATHS $ENV{OGRE_HOME} /usr/local /usr
    PATH_SUFFIXES lib bin/Debug bin/debug Debug debug
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGRE DEFAULT_MSG
    OGRE_LIBRARY_OPTIMIZED
    OGRE_INCLUDE_DIR
)

# Set optimized and debug libraries
HandleLibraryTypes(OGRE)

MARK_AS_ADVANCED(
    OGRE_LIBRARY
    OGRE_LIBRARY_OPTIMIZED
    OGRE_LIBRARY_DEBUG
    OGRE_INCLUDE_DIR
)
