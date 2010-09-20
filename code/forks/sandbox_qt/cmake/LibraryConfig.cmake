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
 #    Configures the external libraries. Whenever possible, the find scripts
 #    from the CMake module path are used, but that required some adjustments
 #    for certain libraries (Boost, OpenAL, TCL)
 #

INCLUDE(CompareVersionStrings)
INCLUDE(FindPackageHandleStandardArgs)

# Prevent CMake from finding libraries in the installation folder on Windows.
# There might already be an installation from another compiler
IF(WIN32)
  LIST(REMOVE_ITEM CMAKE_SYSTEM_PREFIX_PATH  "${CMAKE_INSTALL_PREFIX}")
  LIST(REMOVE_ITEM CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_INSTALL_PREFIX}/bin")
ENDIF()

############## Platform Scripts #################

# On Windows using a package causes way less problems
SET(_option_msg "Set this to true to use precompiled dependecy archives")
OPTION(DEPENDENCY_PACKAGE_ENABLE "${_option_msg}" FALSE)

# Scripts for specific library and CMake config
INCLUDE(LibraryConfigTardis)

IF(DEPENDENCY_PACKAGE_ENABLE)
  GET_FILENAME_COMPONENT(_dep_dir_1 ${CMAKE_SOURCE_DIR}/../dependencies ABSOLUTE)
  GET_FILENAME_COMPONENT(_dep_dir_2 ${CMAKE_SOURCE_DIR}/../lib_dist ABSOLUTE)
  IF(MINGW)
    SET(_compiler_prefix mingw)
  ELSEIF(MSVC80)
    SET(_compiler_prefix msvc8)
  ELSEIF(MSVC90)
    SET(_compiler_prefix msvc9)
  ENDIF()
  FIND_PATH(DEPENDENCY_PACKAGE_DIR
    NAMES version.txt
    PATHS
      ${CMAKE_SOURCE_DIR}/dependencies
      ${_dep_dir_1}
      ${_dep_dir_2}
    PATH_SUFFIXES ${_compiler_prefix} ${_compiler_prefix}/dependencies
    NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
  )
  IF(NOT DEPENDENCY_PACKAGE_DIR)
    MESSAGE(STATUS "Warning: Could not find dependency directory."
                   "Disable LIBRARY_USE_PACKAGE if you have none intalled.")
  ELSE()
    INCLUDE(PackageConfigMinGW)
    INCLUDE(PackageConfigMSVC)
    INCLUDE(PackageConfig) # For both msvc and mingw
  ENDIF()
ENDIF(DEPENDENCY_PACKAGE_ENABLE)

# User script
SET(LIBRARY_CONFIG_USER_SCRIPT "" CACHE FILEPATH
    "Specify a CMake script if you wish to write your own library path config.
     See LibraryConfigTardis.cmake for an example.")
IF(LIBRARY_CONFIG_USER_SCRIPT)
  IF(EXISTS ${CMAKE_MODULE_PATH}/${LIBRARY_CONFIG_USER_SCRIPT})
    INCLUDE(${CMAKE_MODULE_PATH}/${LIBRARY_CONFIG_USER_SCRIPT})
  ENDIF()
ENDIF(LIBRARY_CONFIG_USER_SCRIPT)


############### Library finding #################
# Performs the search and sets the variables    #

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)


####### Static/Dynamic linking options ##########

# On Windows dynamically linked libraries need some special treatment
# You may want to edit these settings if you provide your own libraries
# Note: Default option in the libraries vary, but our default option is dynamic
IF(WIN32)
    #OPTION(LINK_ZLIB_DYNAMIC  "Link ZLib dynamically on Windows" TRUE)

  IF(DEPENDENCY_PACKAGE_ENABLE)
    #MARK_AS_ADVANCED(
      #LINK_ZLIB_DYNAMIC
    #)
  ENDIF()
ENDIF(WIN32)
