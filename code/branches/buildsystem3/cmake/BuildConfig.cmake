 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #
 #  Author:
 #    Reto Grieder
 #  Description:
 #    Configures the compilers and sets build options.
 #    This also includes handling the OGRE plugins and the media directory.
 #

################# Misc Options ##################

# Set binary output directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Take care of some CMake 2.6.0 leftovers
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

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

  MESSAGE(STATUS "*** Build type is ${CMAKE_BUILD_TYPE} ***")
ELSE()
  IF(CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE CACHE STRING FORCE)
  ENDIF()
  MARK_AS_ADVANCED(CMAKE_BUILD_TYPE)
ENDIF()

OPTION(EXTRA_COMPILER_WARNINGS "Enable some extra warnings (heavily pollutes the output)")


################# OGRE Plugins ##################

# More plugins: Plugin_BSPSceneManager, Plugin_OctreeSceneManager
SET(OGRE_PLUGINS_INT Plugin_ParticleFX)
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


################ Compiler Config ################

INCLUDE(FlagUtilities)

# Configure the compiler specific build options
IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
  INCLUDE(BuildConfigGCC)
ELSEIF(MSVC)
  INCLUDE(BuildConfigMSVC)
ELSE()
  MESSAGE(STATUS "Warning: Your compiler is not officially supported.")
ENDIF()

SET(USER_SCRIPT_BUILD_CONFIG "" CACHE FILEPATH
    "Specify a CMake script if you wish to write your own build config.
     See BuildConfigGCC.cmake or BuildConfigMSVC.cmake for examples.")
IF(USER_SCRIPT_BUILD_CONFIG)
  IF(EXISTS ${CMAKE_MODULE_PATH}/${USER_SCRIPT_BUILD_CONFIG}.cmake)
    INCLUDE(${USER_SCRIPT_BUILD_CONFIG})
  ELSEIF(EXISTS ${USER_SCRIPT_BUILD_CONFIG})
    INCLUDE(${USER_SCRIPT_BUILD_CONFIG})
  ELSEIF(EXISTS ${CMAKE_MODULE_PATH}/${USER_SCRIPT_BUILD_CONFIG})
    INCLUDE(${CMAKE_MODULE_PATH}/${USER_SCRIPT_BUILD_CONFIG})
  ENDIF()
ENDIF(USER_SCRIPT_BUILD_CONFIG)


################# Test options ##################

OPTION(TESTING_ENABLE "Enable build tests.")
IF(TESTING_ENABLE)
  ENABLE_TESTING()
ENDIF(TESTING_ENABLE)

OPTION(NETWORK_TESTING_ENABLED "Build network testing tools: i.e. chatclient chatserver and alike.")
OPTION(NETWORKTRAFFIC_TESTING_ENABLED "Build dummyserver4 and dummyclient4.")


############# Installation Settings #############

IF(UNIX)
  SET(ORXONOX_RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin/orxonox)
  SET(ORXONOX_LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib/orxonox)
  SET(ORXONOX_ARCHIVE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib/orxonox/static)
  SET(ORXONOX_MEDIA_INSTALL_PATH   ${CMAKE_INSTALL_PREFIX}/share/orxonox)
  SET(ORXONOX_DOC_INSTALL_PATH     ${CMAKE_INSTALL_PREFIX}/share/doc/orxonox)
  SET(ORXONOX_LOG_INSTALL_PATH     ~/.orxonox/log)
  SET(ORXONOX_CONFIG_INSTALL_PATH  ~/.orxonox/config)
ELSEIF(WIN32)
  SET(ORXONOX_RUNTIME_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
  SET(ORXONOX_LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
  SET(ORXONOX_ARCHIVE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib/static)
  SET(ORXONOX_MEDIA_INSTALL_PATH   ${CMAKE_INSTALL_PREFIX}/media)
  SET(ORXONOX_DOC_INSTALL_PATH     ${CMAKE_INSTALL_PREFIX}/doc)
  SET(ORXONOX_LOG_INSTALL_PATH     ${CMAKE_INSTALL_PREFIX}/log)
  SET(ORXONOX_CONFIG_INSTALL_PATH  ${CMAKE_INSTALL_PREFIX}/config)
ENDIF()


################## Unix rpath ###################

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH ${ORXONOX_LIBRARY_INSTALL_PATH})

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


######## Static/Dynamic linking defines #########

# Disable Boost auto linking completely
ADD_COMPILER_FLAGS("-DBOOST_ALL_NO_LIB")

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
