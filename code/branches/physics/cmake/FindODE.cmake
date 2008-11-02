# Find ODE includes and library
#
# This module defines
#  ODE_INCLUDE_DIR
#  ODE_LIBRARIES, the libraries to link against to use ODE.
#  ODE_LIB_DIR, the location of the libraries
#  ODE_FOUND, If false, do not try to use ODE
#
# Copyright © 2007, Matt Williams (version for FindOGRE)
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture of the ETH Zurich
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Adaption from FindOGRE to FindODE by Reto '1337' Grieder
#                 > www.orxonox.net <

IF (ODE_LIBRARIES AND ODE_INCLUDE_DIR AND ODE_LIB_DIR)
    SET (ODE_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (ODE_LIBRARIES AND ODE_INCLUDE_DIR AND ODE_LIB_DIR)

IF (WIN32) #Windows
    FIND_PATH(ODE_INCLUDE_DIR ode/ode.h
        ../libs/ode-0.10.1/include
        ${DEPENDENCY_DIR}/ode-0.10.1/include
    )

    SET(ODE_LIBRARIES debug ode_singled optimized ode_single)
    FIND_LIBRARY(ODE_LIBDIR NAMES ${ODE_LIBRARIES} PATHS
        ../libs/ode-0.10.1/lib
        ${DEPENDENCY_DIR}/ode-0.10.1/lib
    )

    # Strip the filename from the path
    IF (ODE_LIBDIR)
        GET_FILENAME_COMPONENT(ODE_LIBDIR ${ODE_LIBDIR} PATH)
        SET (ODE_LIB_DIR ${ODE_LIBDIR} CACHE FILEPATH "")
    ENDIF (ODE_LIBDIR)
ELSE (WIN32) #Unix
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(ODE ODE /usr/pack/ode-0.9-sd/i686-debian-linux3.1/lib/pkgconfig/ode.pc) # tardis specific hack
    SET(ODE_INCLUDE_DIR ${ODE_INCLUDE_DIRS})
    SET(ODE_LIB_DIR ${ODE_LIBDIR})
    SET(ODE_LIBRARIES ${ODE_LIBRARIES})
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(ODE_INCLUDE_DIR)
SEPARATE_ARGUMENTS(ODE_LIBRARIES)

SET (ODE_INCLUDE_DIR ${ODE_INCLUDE_DIR} CACHE PATH "")
SET (ODE_LIBRARIES ${ODE_LIBRARIES} CACHE STRING "")
SET (ODE_LIB_DIR ${ODE_LIB_DIR} CACHE PATH "")

IF (ODE_INCLUDE_DIR AND ODE_LIBRARIES AND ODE_LIB_DIR)
    SET(ODE_FOUND TRUE)
ENDIF (ODE_INCLUDE_DIR AND ODE_LIBRARIES AND ODE_LIB_DIR)

IF (ODE_FOUND)
    IF (NOT ODE_FIND_QUIETLY)
        MESSAGE(STATUS "ODE was found.")
        IF (VERBOSE_FIND)
            MESSAGE (STATUS "  include path: ${ODE_INCLUDE_DIR}")
            MESSAGE (STATUS "  library path: ${ODE_LIB_DIR}")
            MESSAGE (STATUS "  libraries:    ${ODE_LIBRARIES}")
        ENDIF (VERBOSE_FIND)
    ENDIF (NOT ODE_FIND_QUIETLY)
ELSE (ODE_FOUND)
    IF (NOT ODE_INCLUDE_DIR)
        MESSAGE(SEND_ERROR "ODE include path was not found.")
    ENDIF (NOT ODE_INCLUDE_DIR)
    IF (NOT ODE_LIB_DIR)
        MESSAGE(SEND_ERROR "ODE library was not found.")
    ENDIF (NOT ODE_LIB_DIR)
    IF (NOT ODE_LIBRARIES)
        MESSAGE(SEND_ERROR "ODE libraries not known.")
    ENDIF (NOT ODE_LIBRARIES)
ENDIF (ODE_FOUND)
