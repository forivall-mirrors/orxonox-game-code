# General package configuration. Merely sets the include paths.
# Library files are treated separately.

# Check package version info
# MAJOR: Interface breaking change somewhere (library version changed, etc.)
# MINOR: Bug fix or small conformant changes
SET(DEPENDENCY_VERSION_REQUIRED 2)
IF(NOT EXISTS ${DEPENDENCY_PACKAGE_DIR}/version.txt)
  SET(DEPENDENCY_VERSION 1.0)
ELSE()
  # Get version from file
  FILE(READ ${DEPENDENCY_PACKAGE_DIR}/version.txt _file_content)
  SET(_match)
  STRING(REGEX MATCH "([0-9]+.[0-9]+)" _match ${_file_content})
  IF(_match)
    SET(DEPENDENCY_VERSION ${_match})
  ELSE()
    MESSAGE(FATAL_ERROR "The version.txt file in the dependency file has corrupt version information.")
  ENDIF()
ENDIF()

INCLUDE(CompareVersionStrings)
COMPARE_VERSION_STRINGS(${DEPENDENCY_VERSION} ${DEPENDENCY_VERSION_REQUIRED} _result)
IF(NOT _result EQUAL 0)
  MESSAGE(FATAL_ERROR "Your dependency package version is ${DEPENDENCY_VERSION}\n"
          "Required version: ${DEPENDENCY_VERSION_REQUIRED}\n"
	  "You can get a new version from www.orxonox.net")
ENDIF()

MESSAGE(STATUS "Using library package for the dependencies.")

# Include paths and other special treatments
SET(ENV{ALUTDIR}               ${DEP_INCLUDE_DIR}/freealut-1.1.0)
SET(ENV{BOOST_ROOT}            ${DEP_INCLUDE_DIR}/boost-1.37.0)
SET(ENV{CEGUIDIR}              ${DEP_INCLUDE_DIR}/cegui-0.6.2)
SET(ENV{DXSDK_DIR}             ${DEP_INCLUDE_DIR}/directx-2007.aug)
SET(ENV{ENETDIR}               ${DEP_INCLUDE_DIR}/enet-1.2)
SET(ENV{LUA_DIR}               ${DEP_INCLUDE_DIR}/lua-5.1.4)
SET(ENV{OGGDIR}                ${DEP_INCLUDE_DIR}/libogg-1.1.3)
SET(ENV{VORBISDIR}             ${DEP_INCLUDE_DIR}/libvorbis-1.2.0)
SET(ENV{OGRE_HOME}             ${DEP_INCLUDE_DIR}/ogre-1.4.9)
SET(ENV{OGRE_PLUGIN_DIR}       ${DEP_BINARY_DIR})
SET(ENV{OPENALDIR}             ${DEP_INCLUDE_DIR}/openal-1.1)
LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/tcl-8.5.2/include)
LIST(APPEND CMAKE_INCLUDE_PATH ${DEP_INCLUDE_DIR}/zlib-1.2.3/include)

### INSTALL ###
# On Windows, DLLs have to be in the executable folder, install them
IF(WIN32 AND DEP_BINARY_DIR)
  ## DEBUG
  # When installing a debug version, we really can't know which libraries
  # are used in released mode because there might be deps of deps.
  # --> Copy all of them, except the debug databases
  INSTALL(
    DIRECTORY ${DEP_BINARY_DIR}/
    DESTINATION bin
    CONFIGURATIONS Debug
    REGEX "^.*\\.pdb$" EXCLUDE
  )

  ## RELEASE
  # Try to filter out all the debug libraries. If the regex doesn't do the
  # job anymore, simply adjust it.
  INSTALL(
    DIRECTORY ${DEP_BINARY_DIR}/
    DESTINATION bin
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel
    REGEX "_[Dd]\\.[a-zA-Z0-9+-]+$|-mt-gd-|^.*\\.pdb$" EXCLUDE
  )
ENDIF()
