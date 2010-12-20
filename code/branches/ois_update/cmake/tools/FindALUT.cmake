# Find ALUT includes and library
#
# This module defines
#  ALUT_INCLUDE_DIR
#  ALUT_LIBRARY, the library to link against to use ALUT.
#  ALUT_FOUND, If false, do not try to use ALUT
#
# Copyright © 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture
# of the ETH Zurich (removed later on)
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli
# Version checking by Reto Grieder
# Adaption of the OGRE find script to ALUT by Kevin Young
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleAdvancedArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(ALUT_INCLUDE_DIR alut.h
  PATHS $ENV{ALUTDIR}
  PATH_SUFFIXES include include/AL ALUT
)
FIND_LIBRARY(ALUT_LIBRARY_OPTIMIZED
  NAMES ALUT alut
  PATHS $ENV{ALUTDIR}
  PATH_SUFFIXES lib bin/Release bin/release Release release ALUT
)
FIND_LIBRARY(ALUT_LIBRARY_DEBUG
  NAMES ALUTD alutd alut_d alutD alut_D
  PATHS $ENV{ALUTDIR}
  PATH_SUFFIXES lib bin/Debug bin/debug Debug debug ALUT
)

# Handle the REQUIRED argument and set ALUT_FOUND
# Also check the version requirements
FIND_PACKAGE_HANDLE_ADVANCED_ARGS(ALUT DEFAULT_MSG
  ALUT_LIBRARY_OPTIMIZED
  ALUT_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(ALUT)

MARK_AS_ADVANCED(
  ALUT_INCLUDE_DIR
  ALUT_LIBRARY_OPTIMIZED
  ALUT_LIBRARY_DEBUG
)
