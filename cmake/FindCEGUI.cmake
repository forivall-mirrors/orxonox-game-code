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

IF (CEGUI_LIBRARIES AND CEGUI_INCLUDE_DIR)
    SET(CEGUI_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (CEGUI_LIBRARIES AND CEGUI_INCLUDE_DIR)

IF (WIN32) #Windows
    MESSAGE(STATUS "Looking for CEGUI")
    SET(CEGUI_INCLUDE_DIR ../ogre/Dependencies/include/CEGUI)
    SET(CEGUI_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ogre/Samples/Common/bin/Release/lib)
    SET(CEGUI_LIBRARIES debug CEGUIBase_d optimized CEGUIBase)
ELSE (WIN32) #Unix
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(CEGUI CEGUI /usr/pack/cegui-0.5.0-sd/i686-debian-linux3.1/lib/pkgconfig/CEGUI.pc) # tardis specific hack
    SET(CEGUI_INCLUDE_DIR ${CEGUI_INCLUDE_DIRS})
    SET(CEGUI_LIB_DIR ${CEGUI_LIBDIR})
    SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(CEGUI_INCLUDE_DIR)
SEPARATE_ARGUMENTS(CEGUI_LIBRARIES)

SET(CEGUI_INCLUDE_DIR ${CEGUI_INCLUDE_DIR} CACHE PATH "")
SET(CEGUI_LIBRARIES ${CEGUI_LIBRARIES} CACHE STRING "")
SET(CEGUI_LIB_DIR ${CEGUI_LIB_DIR} CACHE PATH "")

IF (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARIES)
    SET(CEGUI_FOUND TRUE)
ENDIF (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARIES)

IF (CEGUI_FOUND)
    IF (NOT CEGUI_FIND_QUIETLY)
        MESSAGE(STATUS "  libraries : ${CEGUI_LIBRARIES} from ${CEGUI_LIB_DIR}")
        MESSAGE(STATUS "  includes  : ${CEGUI_INCLUDE_DIR}")
    ENDIF (NOT CEGUI_FIND_QUIETLY)
ELSE (CEGUI_FOUND)
    IF (CEGUI_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find CEGUI")
    ENDIF (CEGUI_FIND_REQUIRED)
ENDIF (CEGUI_FOUND)
