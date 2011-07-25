IF(WIN32)
  # - Test for DirectX
  # Once loaded this will define
  #   DirectX_FOUND        - system has DirectX
  #   DirectX_INCLUDE_DIR  - include directory for DirectX
  #   DirectX_LIB_DIR      - lib directory for DirectX
  #
  # Several changes and additions by Fabian 'x3n' Landau
  #                 > www.orxonox.net <

  IF (DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)
    SET(DirectX_FIND_QUIETLY TRUE)
  ENDIF (DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)

  set(DirectX_FOUND "NO")

  FIND_PATH(DirectX_INCLUDE_DIR "dinput.h"
    C:/DXSDK/Include
  )

  FIND_PATH(DirectX_LIB_DIR "dinput8.lib"
    C:/DXSDK/Lib
    C:/DXSDK/Lib/x86
  )

  IF (DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)
    SET (DirectX_FOUND "YES")
  ENDIF (DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)

  IF (DirectX_FOUND)
    IF (NOT DirectX_FIND_QUIETLY)
      MESSAGE(STATUS "DirectX was found.")
      IF (VERBOSE_FIND)
        MESSAGE (STATUS "  include path: ${DirectX_INCLUDE_DIR}")
        MESSAGE (STATUS "  library path: ${DirectX_LIB_DIR}")
        MESSAGE (STATUS "  libraries:    dinput8.lib")
      ENDIF (VERBOSE_FIND)
    ENDIF (NOT DirectX_FIND_QUIETLY)
  ELSE (DirectX_FOUND)
    IF (NOT DirectX_INCLUDE_DIR)
      MESSAGE(SEND_ERROR "DirectX include path was not found.")
    ENDIF (NOT DirectX_INCLUDE_DIR)
    IF (NOT DirectX_LIB_DIR)
      MESSAGE(SEND_ERROR "DirectX library was not found.")
    ENDIF (NOT DirectX_LIB_DIR)
  ENDIF (DirectX_FOUND)
ENDIF(WIN32)
