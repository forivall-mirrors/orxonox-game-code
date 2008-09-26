# - Find zlib
# Find the native ZLIB includes and library
#
#  ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#  ZLIB_LIBRARY     - List of libraries when using zlib.
#  ZLIB_FOUND       - True if zlib found.
#
#  Modified by Nicolas Schlumberger, (c) 2007
#  to make it work on the Tardis Infrastucture of the ETH Zurich
#
# Several changes and additions by Fabian 'x3n' Landau
#                 > www.orxonox.net <

IF (ZLIB_INCLUDE_DIR)
  SET(ZLIB_FIND_QUIETLY TRUE)
ENDIF (ZLIB_INCLUDE_DIR)

IF(UNIX)
  FIND_PATH(ZLIB_INCLUDE_DIR zlib.h
    /usr/pack/zziplib-0.13.49-sd/include/
    /usr/local/include
    /usr/include
  )

  SET(ZLIB_NAMES z zlib zdll)
  FIND_LIBRARY(ZLIB_LIBRARY
    NAMES ${ZLIB_NAMES}
    PATHS /usr/pack/zziplib-0.13.49-sd/i686-debian-linux3.1/lib/ /usr/lib /usr/local/lib
  )
ENDIF(UNIX)

IF(WIN32)
  IF(EXISTS "../libs/zlib")
    SET (ZLIB_LIBRARY_DIR "../libs/zlib")
  ENDIF(EXISTS "../libs/zlib")

  FIND_PATH(ZLIB_INCLUDE_DIR zlib.h
    ../libs/zlib/include
  )

  FIND_LIBRARY(ZLIB_LIBRARY
    NAMES zlib
    PATHS ${ZLIB_LIBRARY_DIR}
  )
ENDIF(WIN32)

SET(ZLIB_FOUND FALSE)
IF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
  SET(ZLIB_FOUND TRUE)
ENDIF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)

IF (ZLIB_FOUND)
  IF (NOT ZLIB_FIND_QUIETLY)
    MESSAGE(STATUS "ZLIB was found.")
    IF (VERBOSE_FIND)
      MESSAGE (STATUS "  include path: ${ZLIB_INCLUDE_DIR}")
      MESSAGE (STATUS "  library path: ${ZLIB_LIBRARY}")
      MESSAGE (STATUS "  libraries:    zlib")
    ENDIF (VERBOSE_FIND)
  ENDIF (NOT ZLIB_FIND_QUIETLY)
ELSE (ZLIB_FOUND)
  IF (NOT ZLIB_INCLUDE_DIR)
    MESSAGE(SEND_ERROR "ZLIB include path was not found.")
  ENDIF (NOT ZLIB_INCLUDE_DIR)
  IF (NOT ZLIB_LIBRARY)
    MESSAGE(SEND_ERROR "ZLIB was not found.")
    MESSAGE(STATUS "Looked for Z libraries named ${ZLIBS_NAMES}.")
  ENDIF (NOT ZLIB_LIBRARY)
ENDIF (ZLIB_FOUND)

MARK_AS_ADVANCED(
  ZLIB_LIBRARY
  ZLIB_INCLUDE_DIR
)
