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

IF (ALUT_LIBRARY AND ALUT_INCLUDE_DIR)
  SET (ALUT_FIND_QUIETLY TRUE)
ENDIF (ALUT_LIBRARY AND ALUT_INCLUDE_DIR)

FIND_PATH(ALUT_INCLUDE_DIR AL/alut.h
  PATHS
  $ENV{ALUTDIR}
  ~/Library/Frameworks/OpenAL.framework
  /Library/Frameworks/OpenAL.framework
  /System/Library/Frameworks/OpenAL.framework # Tiger
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
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
    SET (ALUT_LIBRARY "-framework OpenAL" CACHE STRING "OpenAL framework for OSX")
  ELSE("${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/Library/Frameworks"
      OR "${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/System/Library/Frameworks"
      )
    # String is not /Library/Frameworks, need to use -F
    SET(ALUT_LIBRARY "-F${ALUT_FRAMEWORK_PATH_TMP} -framework OpenAL" CACHE STRING "OpenAL framework for OSX")
  ENDIF("${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/Library/Frameworks"
    OR "${ALUT_FRAMEWORK_PATH_TMP}" STREQUAL "/System/Library/Frameworks"
    )
  # Clear the temp variable so nobody can see it
  SET(ALUT_FRAMEWORK_PATH_TMP "" CACHE INTERNAL "")

ELSE(${ALUT_INCLUDE_DIR} MATCHES ".framework")
  FIND_LIBRARY(ALUT_LIBRARY
    NAMES alut
    PATHS
    $ENV{ALUTDIR}
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    PATH_SUFFIXES lib libs
    )
ENDIF(${ALUT_INCLUDE_DIR} MATCHES ".framework")

SET (ALUT_FOUND "NO")
IF (ALUT_LIBRARY AND ALUT_INCLUDE_DIR)
  SET (ALUT_FOUND "YES")
  IF (NOT ALUT_FIND_QUIETLY)
    MESSAGE (STATUS "FreeAlut was found.")
    IF (VERBOSE_FIND)
      MESSAGE (STATUS "  include path: ${ALUT_INCLUDE_DIR}")
      MESSAGE (STATUS "  library path: ${ALUT_LIBRARY}")
      MESSAGE (STATUS "  libraries:    alut")
    ENDIF (VERBOSE_FIND)
  ENDIF (NOT ALUT_FIND_QUIETLY)
ELSE (ALUT_LIBRARY AND ALUT_INCLUDE_DIR)
  IF (NOT ALUT_INCLUDE_DIR)
    MESSAGE (SEND_ERROR "FreeAlut include path was not found.")
  ENDIF (NOT ALUT_INCLUDE_DIR)
  IF (NOT ALUT_LIBRARY)
    MESSAGE (SEND_ERROR "FreeAlut library was not found.")
  ENDIF (NOT ALUT_LIBRARY)
ENDIF (ALUT_LIBRARY AND ALUT_INCLUDE_DIR)
