INCLUDE(LibraryConfigMinGW)
INCLUDE(LibraryConfigMSVC)
INCLUDE(LibraryConfigTardis)

# Performs the search and sets the variables

# Expand the next statement if newer boost versions than 1.36.1 are released
SET(Boost_ADDITIONAL_VERSIONS 1.37 1.37.0 CACHE STRING "")
FIND_PACKAGE(Boost 1.34 REQUIRED thread filesystem)
# With MSVC, automatic linking is performed for boost. So wee need to tell
# the linker where to find them. Also note that when running FindBoost for the
# first time, it will set ${Boost_LIBRARIES} to "" but afterwards to the libs.
IF (MSVC)
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF (MSVC)
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(ENet REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(DirectX REQUIRED)

##### Lua #####
IF(CEGUILUA_USE_EXTERNAL_LIBRARY)
  COMPARE_VERSION_STRINGS(${CEGUI_VERSION} "0.6" _version_comparison)
  IF(version_comparison LESS 0)
    SET(LUA_VERSION_REQUEST 5.0)
  ELSE(version_comparison LESS 0)
    SET(LUA_VERSION_REQUEST 5.1)
  ENDIF(version_comparison LESS 0)
ELSE(CEGUILUA_USE_EXTERNAL_LIBRARY)
  SET(LUA_VERSION_REQUEST 5)
ENDIF(CEGUILUA_USE_EXTERNAL_LIBRARY)
FIND_PACKAGE(Lua ${LUA_VERSION_REQUEST} EXACT REQUIRED)

# QUIET: Don't require the whole tcl rat tail
FIND_PACKAGE(TCL QUIET)
IF(NOT TCL_FOUND)
  MESSAGE(FATAL_ERROR "Tcl was not found.")
ENDIF(NOT TCL_FOUND)

# Hide variables created by CMake FindXX scripts
MARK_AS_ADVANCED(
  LUA_LIBRARY_lua
  LUA_LIBRARY_lualib
  OPENAL_INCLUDE_DIR
  OPENAL_LIBRARY
)
