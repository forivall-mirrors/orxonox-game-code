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

# Set Debug build to default when not having multi-config generator like msvc
IF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

# When searching for debug libraries, this is appended to the libarary name
SET(LIBRARY_DEBUG_POSTFIX "_d")
# Sets where to find the binary directory of external libraries
SET(ORXONOX_LIBRARY_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# Working directory for the tolua parser. Adjust for windows because lua.dll has to be there!
SET(TOLUA_PARSER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

OPTION(EXTRA_WARNINGS "Enable some extra warnings (heavily pollutes the output)")
IF(EXTRA_WARNINGS)
  SET(ORXONOX_WARNING_FLAGS "-Wextra --Wno-unsued-parameter")
ELSE(EXTRA_WARNINGS)
  SET(ORXONOX_WARNING_FLAGS "-Wall")
ENDIF(EXTRA_WARNINGS)

SET(ORXONOX_MEDIA_DIRECTORY "${CMAKE_SOURCE_DIR}/../media")
# More plugins: Plugin_BSPSceneManager, Plugin_OctreeSceneManager
# Render systems may be optional, but at least one has to be found in FindOgre
SET(OGRE_PLUGINS RenderSystem_GL RenderSystem_Direct3D9 Plugin_ParticleFX)
IF(WIN32)
  # CG program manager is probably DirectX related (not available under unix)
  LIST(APPEND OGRE_PLUGINS Plugin_CgProgramManager)
ENDIF(WIN32)

# Check the plugins and determine the plugin folder
# You can give a hint by setting the environment variable ENV{OGRE_PLUGIN_DIR}
INCLUDE(CheckOGREPlugins)


############## Compiler Config ##################
INCLUDE(BuildConfigGCC)
INCLUDE(BuildConfigMSVC)
# User can create his own file if required
IF(EXISTS ${CMAKE_BINARY_DIR}/BuildConfigUser.cmake)
  INCLUDE(${CMAKE_BINARY_DIR}/BuildConfigUser)
ENDIF(EXISTS ${CMAKE_BINARY_DIR}/BuildConfigUser.cmake)


################ Test options ###################

OPTION(ENABLE_TESTS "Enable build tests.")
IF(ENABLE_TESTS)
  ENABLE_TESTING()
ENDIF(ENABLE_TESTS)

OPTION(NETWORK_TESTING_ENABLED "Build network testing tools: i.e. chatclient chatserver and alike.")
OPTION(NETWORKTRAFFIC_TESTING_ENABLED "Build dummyserver4 and dummyclient4.")


################### Macros ######################

# Also define macros to easily extend the compiler flags
# Additional argument is a condition
MACRO(ADD_CXX_FLAGS _flag _cond)
  IF(${_cond})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flag}")
  ENDIF(${_cond})
ENDMACRO(ADD_CXX_FLAGS _flag)
MACRO(ADD_C_FLAGS _flag)
  IF(${_cond})
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_flag}")
  ENDIF(${_cond})
ENDMACRO(ADD_C_FLAGS _flag)
