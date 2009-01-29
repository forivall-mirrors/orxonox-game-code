# - Locate FreeAlut
# This module defines
#  ALUT_LIBRARY
#  ALUT_FOUND, if false, do not try to link to Alut
#  ALUT_INCLUDE_DIR, where to find the headers
#
# $ALUTDIR is an environment variable that would
# correspond to the ./configure --prefix=$ALUTDIR
# used in building Alut.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake module.
# On OSX, this will prefer the Framework version (if found) over others.
# People will have to manually change the cache values of
# ALUT_LIBRARY to override this selection.
# Tiger will include OpenAL as part of the System.
# But for now, we have to look around.
# Other (Unix) systems should be able to utilize the non-framework paths.
#
# Several changes and additions by Fabian 'x3n' Landau
# Some simplifications by Adrian Friedli
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

SET(ALUT_PATHS
  $ENV{ALUTDIR}
  /usr/local
  /usr
  /sw        # Fink
  /opt/local # DarwinPorts
  /opt/csw   # Blastwave
  /opt
)

FIND_PATH(ALUT_INCLUDE_DIR AL/alut.h
  PATHS
  ${ALUT_PATHS}
  ~/Library/Frameworks/OpenAL.framework
  /Library/Frameworks/OpenAL.framework
  /System/Library/Frameworks/OpenAL.framework # Tiger
  PATH_SUFFIXES include include/OpenAL include/AL Headers
)

# I'm not sure if I should do a special casing for Apple. It is
# unlikely that other Unix systems will find the framework path.
# But if they do ([Next|Open|GNU]Step?),
# do they want the -framework option also?
IF(${ALUT_INCLUDE_DIR} MATCHES ".framework")

  STRING(REGEX REPLACE "(.*)/.*\\.framework/.*" "\\1" ALUT_FRAMEWORK_PATH_TMP ${ALUT_INCLUDE_DIR})
  IF("${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/Library/Frameworks"
      OR "${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/System/Library/Frameworks"
      )
    # String is in default search path, don't need to use -F
    SET (ALUT_LIBRARY_OPTIMIZED "-framework OpenAL" CACHE STRING "OpenAL framework for OSX")
  ELSE("${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/Library/Frameworks"
      OR "${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/System/Library/Frameworks"
      )
    # String is not /Library/Frameworks, need to use -F
    SET(ALUT_LIBRARY_OPTIMIZED "-F${ALUT_FRAMEWORK_PATH_TMP} -framework OpenAL" CACHE STRING "OpenAL framework for OSX")
  ENDIF("${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/Library/Frameworks"
    OR "${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/System/Library/Frameworks"
    )
  # Clear the temp variable so nobody can see it
  SET(ALUT_FRAMEWORK_PATH_TMP "" CACHE INTERNAL "")

ELSE(${ALUT_INCLUDE_DIR} MATCHES ".framework")
  FIND_LIBRARY(ALUT_LIBRARY_OPTIMIZED
    NAMES alut
    PATHS ${ALUT_PATHS}
    PATH_SUFFIXES lib libs
  )
  FIND_LIBRARY(ALUT_LIBRARY_DEBUG
    NAMES alut${LIBRARY_DEBUG_POSTFIX}
    PATHS ${ALUT_PATHS}
    PATH_SUFFIXES lib libs
  )
ENDIF(${ALUT_INCLUDE_DIR} MATCHES ".framework")

# handle the QUIETLY and REQUIRED arguments and set ALUT_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALUT DEFAULT_MSG
    ALUT_LIBRARY_OPTIMIZED
    ALUT_INCLUDE_DIR
)

# Set optimized and debug libraries
HANDLE_LIBRARY_TYPES(ALUT)

MARK_AS_ADVANCED(
    ALUT_LIBRARY
    ALUT_LIBRARY_OPTIMIZED
    ALUT_LIBRARY_DEBUG
    ALUT_INCLUDE_DIR
)
