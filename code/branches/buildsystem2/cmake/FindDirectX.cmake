# Find script for DirectX on Windows
# Once loaded this will define
#   DirectX_FOUND        - system has DirectX
#   DirectX_INCLUDE_DIR  - include directory for DirectX
#   DirectX_LIBRARY      - library for DirectX
#
# Several changes and additions by Fabian 'x3n' Landau
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(DIRECTX_INCLUDE_DIR dinput.h
  PATHS
  $ENV{DXSDK_DIR}
  PATH_SUFFIXES include
)
FIND_LIBRARY(DIRECTX_LIBRARY_input dinput8
  PATHS
  $ENV{DXSDK_DIR}
  PATH_SUFFIXES lib Lib lib/x86 Lib/x86
)
FIND_LIBRARY(DIRECTX_LIBRARY_guid dxguid
  PATHS
  $ENV{DXSDK_DIR}
  PATH_SUFFIXES lib Lib lib/x86 Lib/x86
)

# Handle the REQUIRED argument and set DIRECTX_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DirectX DEFAULT_MSG
  DIRECTX_INCLUDE_DIR
  DIRECTX_LIBRARY_input
  DIRECTX_LIBRARY_guid
)

SET(DIRECTX_LIBRARIES ${DIRECTX_LIBRARY_input} ${DIRECTX_LIBRARY_guid})

MARK_AS_ADVANCED(
  DIRECTX_INCLUDE_DIR
  DIRECTX_LIBRARY_input
  DIRECTX_LIBRARY_guid
)
