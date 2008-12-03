IF(WIN32)
  # - Test for DirectX
  # Once loaded this will define
  #   DirectX_FOUND        - system has DirectX
  #   DirectX_INCLUDE_DIR  - include directory for DirectX
  #   DirectX_LIB_DIR      - lib directory for DirectX
  #
  # Several changes and additions by Fabian 'x3n' Landau
  #                 > www.orxonox.net <

  FIND_PATH(DirectX_INCLUDE_DIR dinput.h
    PATHS
    $ENV{DXSDK_DIR}
    PATH_SUFFIXES include
  )

  FIND_PATH(DirectX_LIB_DIR dinput8.lib
    $ENV{DXSDK_DIR}
    PATH_SUFFIXES lib lib/x86
  )

  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(DirectX DEFAULT_MSG DirectX_INCLUDE_DIR DirectX_LIB_DIR)

ENDIF(WIN32)

MARK_AS_ADVANCED(DirectX_INCLUDE_DIR DirectX_LIB_DIR)
