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
 #

# Required macros and functions
INCLUDE(FlagUtilities)
INCLUDE(TargetUtilities)

# Configure the two headers and set some options
INCLUDE(OrxonoxConfig.cmake)

################ Library Defines ################

# Disable Boost auto linking completely
ADD_COMPILER_FLAGS("-DBOOST_ALL_NO_LIB")

# If no defines are specified, these libs get linked statically
ADD_COMPILER_FLAGS("-DBOOST_ALL_DYN_LINK" WIN32 LINK_BOOST_DYNAMIC)
ADD_COMPILER_FLAGS("-DENET_DLL"           WIN32 LINK_ENET_DYNAMIC)
ADD_COMPILER_FLAGS("-DLUA_BUILD_AS_DLL"   WIN32 LINK_LUA_DYNAMIC)
ADD_COMPILER_FLAGS("-DOIS_DYNAMIC_LIB")
ADD_COMPILER_FLAGS("-DZLIB_DLL"           WIN32 LINK_ZLIB_DYNAMIC)
# If no defines are specified, these libs get linked dynamically
ADD_COMPILER_FLAGS("-DCEGUI_STATIC"       WIN32 NOT LINK_CEGUI_DYNAMIC)
ADD_COMPILER_FLAGS("-DOGRE_STATIC_LIB"    WIN32 NOT LINK_OGRE_DYNAMIC)
ADD_COMPILER_FLAGS("-DSTATIC_BUILD"       WIN32 NOT LINK_TCL_DYNAMIC)

# Use TinyXML++
ADD_COMPILER_FLAGS("-DTIXML_USE_TICPP")

############## Include Directories ##############

# Set the search paths for include files
INCLUDE_DIRECTORIES(
  # External
  ${OGRE_INCLUDE_DIR}
  ${CEGUI_INCLUDE_DIR}
  ${ENET_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIRS}
  ${ALUT_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR}
  ${OGG_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
  ${TCL_INCLUDE_PATH}
  ${DIRECTX_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
  ${VLD_INCLUDE_DIR}

  # All includes in "externals" should be prefixed with the path
  # relative to "external" to avoid conflicts
  ${CMAKE_CURRENT_SOURCE_DIR}/external
  # Include directories needed even if only included by Orxonox
  ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet
  ${CMAKE_CURRENT_SOURCE_DIR}/external/ois

  # OrxonoxConfig.h
  ${CMAKE_CURRENT_BINARY_DIR}
)

IF(CEGUILUA_USE_INTERNAL_LIBRARY)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/external/ceguilua/ceguilua-${CEGUI_VERSION})
ENDIF()

################### Tolua Bind ##################

# Create directory because the tolua application doesn't work otherwise
IF(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/toluabind/${CMAKE_CFG_INTDIR})
  FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/toluabind/${CMAKE_CFG_INTDIR})
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/toluabind/${CMAKE_CFG_INTDIR})

################ Sub Directories ################

ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(libraries)
ADD_SUBDIRECTORY(orxonox)
ADD_SUBDIRECTORY(modules)

################ Executable ################

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries
  ${CMAKE_CURRENT_SOURCE_DIR}/orxonox
)

# Translate argument
IF(ORXONOX_USE_WINMAIN)
  SET(ORXONOX_WIN32 WIN32)
ENDIF()

ORXONOX_ADD_EXECUTABLE(orxonox-main
  # When defined as WIN32 this removes the console window on Windows
  ${ORXONOX_WIN32}
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES
    Orxonox.cc
  OUTPUT_NAME orxonox
)

# Get name to configure the run scripts
GET_TARGET_PROPERTY(_exec_loc orxonox-main LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE INTERNAL "")


# When using Visual Studio we want to use the output directory as working
# directory and we also want to specify where the external dlls
# (lua, ogre, etc.) are. The problem hereby is that these information cannot
# be specified in CMake because they are not stored in the actual project file.
# This workaround will create a configured *.vcproj.user file that holds the
# right values. When starting the solution for the first time,
# these get written to the *vcproj.yourPCname.yourname.user
IF(MSVC)
  IF(CMAKE_CL_64)
    SET(MSVC_PLATFORM "x64")
  ELSE()
    SET(MSVC_PLATFORM "Win32")
  ENDIF()
  STRING(REGEX REPLACE "^Visual Studio ([0-9][0-9]?) .*$" "\\1"
         VISUAL_STUDIO_VERSION_SIMPLE "${CMAKE_GENERATOR}")
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox-main.vcproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/orxonox-main.vcproj.user")
ENDIF(MSVC)
