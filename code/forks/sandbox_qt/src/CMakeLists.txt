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

####### Library Behaviour (dependencies) ########

# If no defines are specified, these libs get linked statically
#ADD_COMPILER_FLAGS("-DZLIB_DLL"           WIN32 LINK_ZLIB_DYNAMIC)
# If no defines are specified, these libs get linked dynamically
#ADD_COMPILER_FLAGS("-DSTATIC_BUILD"       WIN32 NOT LINK_TCL_DYNAMIC)

######### Library Behaviour (external) ##########

# Default linking for externals is SHARED
SET(ORXONOX_EXTERNAL_LINK_MODE SHARED CACHE STRING "Link mode for external libraries that we build ourselves. Note: on Windows, some libraries cannot be linked shared.")

IF(ORXONOX_EXTERNAL_LINK_MODE STREQUAL "SHARED")
  SET(_external_shared_link TRUE)
ELSE()
  SET(_external_shared_link FALSE)
ENDIF()
# If no defines are specified, these libs get linked dynamically
#ADD_COMPILER_FLAGS("-DTOLUA_STATIC_BUILD"          WIN32 NOT _external_shared_link)

############## Include Directories ##############

# Set the search paths for include files
INCLUDE_DIRECTORIES(
  # External
  ${VLD_INCLUDE_DIR}

  # All includes in "externals" should be prefixed with the path
  # relative to "external" to avoid conflicts
  ${CMAKE_CURRENT_SOURCE_DIR}/external
  # Include directories needed even if only included by Orxonox
  #${CMAKE_CURRENT_SOURCE_DIR}/external/bullet

  # OrxonoxConfig.h
  ${CMAKE_CURRENT_BINARY_DIR}
)

###################### Qt4 ######################

INCLUDE(${QT_USE_FILE})

################ Sub Directories ################

ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(libraries)
ADD_SUBDIRECTORY(orxonox)

################## Executable ###################

SET_SOURCE_FILES(ORXONOX_MAIN_SRC_FILES Orxonox.cc)

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
    ${QT_QTMAIN_LIBRARY}
    orxonox
  SOURCE_FILES
    ${ORXONOX_MAIN_SRC_FILES}
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

#################### Doxygen ####################

# Prepare include paths for Doxygen. This is necessary to display
# the correct path to use when including a file, e.g.
# core/XMLPort.h instead of src/core/XMLPort.h

GET_DIRECTORY_PROPERTY(_temp INCLUDE_DIRECTORIES)
# Replace ';' by spaces
STRING(REPLACE ";" " " _temp "${_temp}")
SET(DOXYGEN_INCLUDE_DIRECTORIES "${_temp}" PARENT_SCOPE)
