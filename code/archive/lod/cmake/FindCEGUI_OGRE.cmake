# Find CEGUI_OGRE includes and library
#
# This module defines
#  CEGUI_OGRE_INCLUDE_DIR
#  CEGUI_OGRE_LIBRARIES, the libraries to link against to use CEGUI_OGRE.
#  CEGUI_OGRE_LIB_DIR, the location of the libraries
#  CEGUI_OGRE_FOUND, If false, do not try to use CEGUI_OGRE
#
# Created by Matt Williams to find OGRE libraries
# Copyright © 2007, Matt Williams
#
# Modified by Nicolas Schlumberger to find CEGUI-OGRE libraries
# and make it run on the Tardis-Infrastucture of the ETH Zurich
# Copyright 2007, Nicolas Schlumberger
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
#                 > www.orxonox.net <

IF (CEGUI_OGRE_LIBRARIES AND CEGUI_OGRE_INCLUDE_DIR AND CEGUI_OGRE_LIB_DIR)
    SET(CEGUI_OGRE_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (CEGUI_OGRE_LIBRARIES AND CEGUI_OGRE_INCLUDE_DIR AND CEGUI_OGRE_LIB_DIR)

IF (WIN32) #Windows
    MESSAGE(STATUS "Looking for CEGUI_OGRE")
    SET(CEGUI_OGRE_INCLUDE_DIR ../libs/ogre/Samples/Common/CEGUIRenderer/include)
    SET(CEGUI_OGRE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs/ogre/Samples/Common/bin/Release/lib)
    SET(CEGUI_OGRE_LIBRARIES debug OgreGUIRenderer_d optimized OgreGUIRenderer)
ELSE (WIN32) #Unix
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(CEGUI_OGRE CEGUI-OGRE /usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1/lib/pkgconfig/CEGUI-OGRE.pc) # tardis specific hack
    SET(CEGUI_OGRE_INCLUDE_DIR ${CEGUI_OGRE_INCLUDE_DIRS})
    SET(CEGUI_OGRE_LIB_DIR ${CEGUI_OGRE_LIBDIR})
    SET(CEGUI_OGRE_LIBRARIES ${CEGUI_OGRE_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(CEGUI_OGRE_INCLUDE_DIR)
SEPARATE_ARGUMENTS(CEGUI_OGRE_LIBRARIES)

SET(CEGUI_OGRE_INCLUDE_DIR ${CEGUI_OGRE_INCLUDE_DIR} CACHE PATH "")
SET(CEGUI_OGRE_LIBRARIES ${CEGUI_OGRE_LIBRARIES} CACHE STRING "")
SET(CEGUI_OGRE_LIB_DIR ${CEGUI_OGRE_LIB_DIR} CACHE PATH "")

IF (CEGUI_OGRE_INCLUDE_DIR AND CEGUI_OGRE_LIBRARIES AND CEGUI_OGRE_LIB_DIR)
    SET(CEGUI_OGRE_FOUND TRUE)
ENDIF (CEGUI_OGRE_INCLUDE_DIR AND CEGUI_OGRE_LIBRARIES AND CEGUI_OGRE_LIB_DIR)

IF (CEGUI_OGRE_FOUND)
    IF (NOT CEGUI_OGRE_FIND_QUIETLY)
        MESSAGE(STATUS "CEGUI_Ogre was found.")
        IF (VERBOSE_FIND)
            MESSAGE (STATUS "  include path: ${CEGUI_OGRE_INCLUDE_DIR}")
            MESSAGE (STATUS "  library path: ${CEGUI_OGRE_LIB_DIR}")
            MESSAGE (STATUS "  libraries:    ${CEGUI_OGRE_LIBRARIES}")
        ENDIF (VERBOSE_FIND)
    ENDIF (NOT CEGUI_OGRE_FIND_QUIETLY)
ELSE (CEGUI_OGRE_FOUND)
    IF (NOT CEGUI_OGRE_INCLUDE_DIR)
        MESSAGE(SEND_ERROR "CEGUI_Ogre include path was not found.")
    ENDIF (NOT CEGUI_OGRE_INCLUDE_DIR)
    IF (NOT CEGUI_OGRE_LIB_DIR)
        MESSAGE(SEND_ERROR "CEGUI_Ogre library was not found.")
    ENDIF (NOT CEGUI_OGRE_LIB_DIR)
    IF (NOT CEGUI_OGRE_LIBRARIES)
        MESSAGE(SEND_ERROR "CEGUI_Ogre libraries not known.")
    ENDIF (NOT CEGUI_OGRE_LIBRARIES)
ENDIF (CEGUI_OGRE_FOUND)
