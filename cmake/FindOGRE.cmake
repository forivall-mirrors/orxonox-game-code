# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARIES, the libraries to link against to use OGRE.
#  OGRE_LIB_DIR, the location of the libraries
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright © 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture of the ETH Zurich
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
#                 > www.orxonox.net <

IF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR AND OGRE_LIB_DIR)
    SET (OGRE_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR AND OGRE_LIB_DIR)

IF (WIN32) #Windows
    FIND_PATH(OGRE_INCLUDE_DIR Ogre.h
        ../libs/ogre/OgreMain/include
    )

    SET(OGRE_LIBRARIES debug OgreMain_d optimized OgreMain)
    FIND_LIBRARY(OGRE_LIBDIR NAMES ${OGRE_LIBRARIES} PATHS
        ../libs/ogre/Samples/Common/bin/Release
    )

    # Strip the filename from the path
    IF (OGRE_LIBDIR)
        GET_FILENAME_COMPONENT(OGRE_LIBDIR ${OGRE_LIBDIR} PATH)
        SET (OGRE_LIB_DIR ${OGRE_LIBDIR} CACHE FILEPATH "")
    ENDIF (OGRE_LIBDIR)
ELSE (WIN32) #Unix
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(OGRE OGRE /usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1/lib/pkgconfig/OGRE.pc) # tardis specific hack
    SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIRS})
    SET(OGRE_LIB_DIR ${OGRE_LIBDIR})
    SET(OGRE_LIBRARIES ${OGRE_LIBRARIES})
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(OGRE_INCLUDE_DIR)
SEPARATE_ARGUMENTS(OGRE_LIBRARIES)

SET (OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR} CACHE PATH "")
SET (OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
SET (OGRE_LIB_DIR ${OGRE_LIB_DIR} CACHE PATH "")

IF (OGRE_INCLUDE_DIR AND OGRE_LIBRARIES AND OGRE_LIB_DIR)
    SET(OGRE_FOUND TRUE)
ENDIF (OGRE_INCLUDE_DIR AND OGRE_LIBRARIES AND OGRE_LIB_DIR)

IF (OGRE_FOUND)
    IF (NOT OGRE_FIND_QUIETLY)
        MESSAGE(STATUS "Ogre was found.")
        IF (VERBOSE_FIND)
            MESSAGE (STATUS "  include path: ${OGRE_INCLUDE_DIR}")
            MESSAGE (STATUS "  library path: ${OGRE_LIB_DIR}")
            MESSAGE (STATUS "  libraries:    ${OGRE_LIBRARIES}")
        ENDIF (VERBOSE_FIND)
    ENDIF (NOT OGRE_FIND_QUIETLY)
ELSE (OGRE_FOUND)
    IF (NOT OGRE_INCLUDE_DIR)
        MESSAGE(SEND_ERROR "Ogre include path was not found.")
    ENDIF (NOT OGRE_INCLUDE_DIR)
    IF (NOT OGRE_LIB_DIR)
        MESSAGE(SEND_ERROR "Ogre library was not found.")
    ENDIF (NOT OGRE_LIB_DIR)
    IF (NOT OGRE_LIBRARIES)
        MESSAGE(SEND_ERROR "Ogre libraries not known.")
    ENDIF (NOT OGRE_LIBRARIES)
ENDIF (OGRE_FOUND)
