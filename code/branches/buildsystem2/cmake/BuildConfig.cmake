 #
 #   ORXONOX - the hottest 3D action shooter ever to exist
 #                    > www.orxonox.net <
 #
 #
 #   License notice:
 #
 #   This program is free software; you can redistribute it and/or
 #   modify it under the terms of the GNU General Public License
 #   as published by the Free Software Foundation; either version 2
 #   of the License, or (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program; if not, write to the Free Software
 #   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #   Author:
 #      Reto Grieder
 #   Co-authors:
 #      ...
 #

################ Misc Options ###################

# Set binary output directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Sets where to find the external libraries like OgreMain.dll at runtime
# On Unix you should not have to change this at all.
IF(NOT ORXONOX_RUNTIME_LIBRARY_DIRECTORY)
  SET(ORXONOX_RUNTIME_LIBRARY_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
ENDIF()

# Set Debug build to default when not having multi-config generator like msvc
IF(NOT CMAKE_CONFIGURATION_TYPES)
  IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Build types are: Debug, Release, MinSizeRel, RelWithDebInfo" FORCE)
  ENDIF()
  MARK_AS_ADVANCED(CLEAR CMAKE_BUILD_TYPE)
ELSE()
  IF(CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE CACHE STRING FORCE)
  ENDIF()
  MARK_AS_ADVANCED(CMAKE_BUILD_TYPE)
ENDIF()

OPTION(EXTRA_WARNINGS "Enable some extra warnings (heavily pollutes the output)")

# Specify media directory
GET_FILENAME_COMPONENT(_media_path "${CMAKE_SOURCE_DIR}/../media" ABSOLUTE)
SET(ORXONOX_MEDIA_DIRECTORY ${_media_path} CACHE PATH
    "Location of the media directory.")
IF(NOT EXISTS ${ORXONOX_MEDIA_DIRECTORY})
  MESSAGE(STATUS "Warning: The media directory does not exist ${ORXONOX_MEDIA_DIRECTORY}")
ENDIF()

# More plugins: Plugin_BSPSceneManager, Plugin_OctreeSceneManager
# Render systems may be optional, but at least one has to be found in FindOgre
SET(OGRE_PLUGINS_INT RenderSystem_GL RenderSystem_Direct3D9 Plugin_ParticleFX)
IF(WIN32)
  # CG program manager is probably DirectX related (not available under unix)
  LIST(APPEND OGRE_PLUGINS_INT Plugin_CgProgramManager)
ENDIF(WIN32)
SET(OGRE_PLUGINS ${OGRE_PLUGINS_INT} CACHE STRING
   "Specify which OGRE plugins to load. Existance check is performed.")

# Check the plugins and determine the plugin folder
# You can give a hint by setting the environment variable ENV{OGRE_PLUGIN_DIR}
INCLUDE(CheckOGREPlugins)
CHECK_OGRE_PLUGINS(${OGRE_PLUGINS})


############## Compiler Config ##################

INCLUDE(FlagUtilities)

# Configure the compiler specific build options
IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
  INCLUDE(BuildConfigGCC)
ELSEIF(MSVC)
  INCLUDE(BuildConfigMSVC)
ELSE()
  MESSAGE(STATUS "Warning: Your compiler is not officially supported.")
ENDIF()

SET(BUILD_CONFIG_USER_SCRIPT "" CACHE FILEPATH
    "Specify a CMake script if you wish to write your own build config.
     See BuildConfigGCC.cmake or BuildConfigMSVC.cmake for examples.")
IF(BUILD_CONFIG_USER_SCRIPT)
  IF(EXISTS ${CMAKE_MODULE_PATH}/${BUILD_CONFIG_USER_SCRIPT}.cmake)
    INCLUDE(${BUILD_CONFIG_USER_SCRIPT})
  ELSEIF(EXISTS ${BUILD_CONFIG_USER_SCRIPT})
    INCLUDE(${BUILD_CONFIG_USER_SCRIPT})
  ELSEIF(EXISTS ${CMAKE_MODULE_PATH}/${BUILD_CONFIG_USER_SCRIPT})
    INCLUDE(${CMAKE_MODULE_PATH}/${BUILD_CONFIG_USER_SCRIPT})
  ENDIF()
ENDIF(BUILD_CONFIG_USER_SCRIPT)


################ Test options ###################

OPTION(ENABLE_TESTS "Enable build tests.")
IF(ENABLE_TESTS)
  ENABLE_TESTING()
ENDIF(ENABLE_TESTS)

OPTION(NETWORK_TESTING_ENABLED "Build network testing tools: i.e. chatclient chatserver and alike.")
OPTION(NETWORKTRAFFIC_TESTING_ENABLED "Build dummyserver4 and dummyclient4.")


####### Static/Dynamic linking defines ##########

# If no defines are specified, these libs get linked statically
ADD_COMPILER_FLAGS("-DBOOST_ALL_DYN_LINK" WIN32 LINK_BOOST_DYNAMIC)
ADD_COMPILER_FLAGS("-DENET_DLL"           WIN32 LINK_ENET_DYNAMIC)
ADD_COMPILER_FLAGS("-DLUA_BUILD_AS_DLL"   WIN32 LINK_LUA_DYNAMIC)
ADD_COMPILER_FLAGS("-DZLIB_DLL"           WIN32 LINK_ZLIB_DYNAMIC)

# If no defines are specified, these libs get linked dynamically
# You can change that optionally in the Cache.
ADD_COMPILER_FLAGS("-DCEGUI_STATIC"       WIN32 NOT LINK_CEGUI_DYNAMIC)
ADD_COMPILER_FLAGS("-DOGRE_STATIC_LIB"    WIN32 NOT LINK_OGRE_DYNAMIC)
ADD_COMPILER_FLAGS("-DSTATIC_BUILD"       WIN32 NOT LINK_TCL_DYNAMIC)
