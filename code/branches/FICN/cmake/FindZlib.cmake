IF(WIN32)
  IF(EXISTS "../libs/ogre/Dependencies/lib/Release")
    SET (Zlib_LIBRARY_DIR "../libs/ogre/Dependencies/lib/Release")
  ENDIF(EXISTS "../libs/ogre/Dependencies/lib/Release")

  FIND_LIBRARY(Zlib_LIBRARY
    NAMES zlib
    PATHS ${Zlib_LIBRARY_DIR}
  )

  IF(Zlib_LIBRARY)
    MESSAGE(STATUS "Zlib was found.")
  ELSE(Zlib_LIBRARY)
    MESSAGE(FATAL_ERROR "Zlib was NOT found.")
  ENDIF(Zlib_LIBRARY)
ENDIF(WIN32)