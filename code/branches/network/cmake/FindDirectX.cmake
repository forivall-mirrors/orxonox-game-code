IF(WIN32)
  # - Test for DirectX
  # Once loaded this will define
  #   DirectX_FOUND        - system has DirectX
  #   DirectX_INCLUDE_DIR  - include directory for DirectX
  #   DirectX_LIB_DIR      - lib directory for DirectX

  set(DirectX_FOUND "NO")

  FIND_PATH(DirectX_INCLUDE_DIR "dinput.h"
    C:/DXSDK/Include
  )

  FIND_PATH(DirectX_LIB_DIR "dinput8.lib"
    C:/DXSDK/Lib
    C:/DXSDK/Lib/x86
  )

  if(DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)
    set(DirectX_FOUND "YES")
  endif(DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)

  if (DirectX_FOUND)
    if (NOT DirectX_FIND_QUIETLY)
      message(STATUS "Found DirectX")
    endif (NOT DirectX_FIND_QUIETLY)
  else (DirectX_FOUND)
    if (DirectX_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find DirectX")
    else (DirectX_FOUNC)
      message(STATUS "Could not find DirectX")
    endif (DirectX_FIND_REQUIRED)
  endif (DirectX_FOUND)
ENDIF(WIN32)
