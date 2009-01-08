IF(WIN32)
  # Find script for DirectX on Windows
  # Once loaded this will define
  #   DirectX_FOUND        - system has DirectX
  #   DirectX_INCLUDE_DIR  - include directory for DirectX
  #   DirectX_LIBRARY      - library for DirectX
  #
  # Several changes and additions by Fabian 'x3n' Landau
  #                 > www.orxonox.net <

  FIND_PATH(DirectX_INCLUDE_DIR dinput.h
    PATHS
    $ENV{DXSDK_DIR}
    PATH_SUFFIXES include
  )

  FIND_LIBRARY(DirectX_LIBRARY_1 dinput8
    PATHS
    $ENV{DXSDK_DIR}
    PATH_SUFFIXES lib Lib lib/x86 Lib/x86
  )
  FIND_LIBRARY(DirectX_LIBRARY_2 dxguid
    PATHS
    $ENV{DXSDK_DIR}
    PATH_SUFFIXES lib Lib lib/x86 Lib/x86
  )

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(DirectX DEFAULT_MSG DirectX_INCLUDE_DIR DirectX_LIBRARY_1 DirectX_LIBRARY_2)

  IF(DIRECTX_FOUND)
    SET(DirectX_LIBRARIES ${DirectX_LIBRARY_1} ${DirectX_LIBRARY_2} CACHE STRING "DirectX library files" FORCE)
  ENDIF(DIRECTX_FOUND)
ENDIF(WIN32)

MARK_AS_ADVANCED(
  DirectX_INCLUDE_DIR
  DirectX_LIBRARY_1
  DirectX_LIBRARY_2
  DirectX_LIBRARIES
)
