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
 #    Configures some basics and controls the configuration scripts
 #

# Defined LINUX
IF(UNIX AND NOT APPLE)
  SET(LINUX TRUE)
ENDIF()

IF(WIN32)
  IF(MSVC10)
    CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3 FATAL_ERROR)
  ELSE()
    CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
  ENDIF()
ELSE()
  CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
ENDIF()

# Keep devs from using the root directory as binary directory (messes up the source tree)
IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "Please do not use the root directory as CMake output directory!
  mkdir build; cd build; cmake ..
  And you will have to clean the source directory by deleting CMakeCache.txt and the folder CMakeFiles")
ENDIF()

PROJECT(Orxonox C CXX)

################ General Config #################

# Version info
SET(ORXONOX_VERSION_MAJOR 0)
SET(ORXONOX_VERSION_MINOR 0)
SET(ORXONOX_VERSION_PATCH 5)
SET(ORXONOX_VERSION
  ${ORXONOX_VERSION_MAJOR}.${ORXONOX_VERSION_MINOR}.${ORXONOX_VERSION_PATCH}
)
SET(ORXONOX_VERSION_NAME "Arcturus")

# Standard path suffixes
SET(DEFAULT_RUNTIME_PATH bin)
SET(DEFAULT_LIBRARY_PATH lib)
SET(DEFAULT_ARCHIVE_PATH lib/static)
SET(DEFAULT_MODULE_PATH  lib/modules)
SET(DEFAULT_DOC_PATH     doc)
SET(DEFAULT_DATA_PATH    data)
SET(DEFAULT_CONFIG_PATH  config)
SET(DEFAULT_LOG_PATH     log)
SET(DEFAULT_BUNDLE_PATH  bundle)

# Set output directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DEFAULT_RUNTIME_PATH})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DEFAULT_LIBRARY_PATH})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DEFAULT_ARCHIVE_PATH})
SET(CMAKE_MODULE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/${DEFAULT_MODULE_PATH})
SET(CMAKE_DOC_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/${DEFAULT_DOC_PATH})
# Data directories are only inputs, no delclaration here
SET(CMAKE_CONFIG_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/${DEFAULT_CONFIG_PATH})
SET(CMAKE_LOG_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/${DEFAULT_LOG_PATH})

# Set the extension of the dynamic modules
SET(ORXONOX_MODULE_EXTENSION ".module")

# Sets where to find the external libraries like OgreMain.dll at runtime
# On Unix you should not have to change this at all.
# This only applies to development runs in the build tree
SET(RUNTIME_LIBRARY_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Take care of some CMake 2.6.0 leftovers
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

# This sets where to look for CMake modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_SOURCE_DIR}/cmake/tools)

# Flag variables for extra configuration "RelForDevs" should be hidden
MARK_AS_ADVANCED(
  CMAKE_CXX_FLAGS_RELFORDEVS
  CMAKE_C_FLAGS_RELFORDEVS
  CMAKE_EXE_LINKER_FLAGS_RELFORDEVS
  CMAKE_MODULE_LINKER_FLAGS_RELFORDEVS
  CMAKE_SHARED_LINKER_FLAGS_RELFORDEVS
)

IF(NOT CMAKE_CONFIGURATION_TYPES)
  # Set Debug build to default when not having multi-config generator like MSVC or XCODE
  IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Build types are: Debug, RelForDevs, RelWithDebInfo, Release, MinSizeRel" FORCE)
  ENDIF()
  MARK_AS_ADVANCED(CLEAR CMAKE_BUILD_TYPE)

  MESSAGE(STATUS "*** Build type is ${CMAKE_BUILD_TYPE} ***")
ELSE()
  # Make sure no build type is ever set
  IF(CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE CACHE STRING FORCE)
  ENDIF()
  MARK_AS_ADVANCED(FORCE CMAKE_BUILD_TYPE)
  # Add our extra configuration "RelForDevs"
  SET(CMAKE_CONFIGURATION_TYPES "Debug;RelForDevs;RelWithDebInfo;Release;MinSizeRel"
      CACHE STRING "Semicolon separated list of supported configuration types." FORCE)
ENDIF()

# Debug builds can not be installed
INSTALL(SCRIPT cmake/InstallCheck.cmake)

IF(APPLE)
  # Set 10.5 as the base SDK by default
  SET(XCODE_ATTRIBUTE_SDKROOT macosx10.5)

  # 10.6 sets x86_64 as the default architecture.
  # Because Carbon isn't supported on 64-bit and we still need it, force the architectures to ppc and i386
  IF(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
    SET(CMAKE_OSX_ARCHITECTURES "i386")
  ENDIF()
  IF(CMAKE_OSX_ARCHITECTURES MATCHES "ppc64")
    SET(CMAKE_OSX_ARCHITECTURES "ppc")
  ENDIF()
  IF(NOT CMAKE_OSX_ARCHITECTURES)
    SET(CMAKE_OSX_ARCHITECTURES "i386")
  ENDIF()
ENDIF()

########### Subfolders and Subscripts ###########

# General build and compiler options and configurations
INCLUDE(CompilerConfig)

# Library finding
INCLUDE(LibraryConfig)

# Configure installation paths and options
INCLUDE(InstallConfig)

# Configure data directory location and installation
ADD_SUBDIRECTORY(data)

# Create the actual project
ADD_SUBDIRECTORY(src)

# Configure the binary output directory. Do this after src!
ADD_SUBDIRECTORY(bin)

# System specific files (mostly for installation)
ADD_SUBDIRECTORY(contrib)

# Last but not least: Try to make a doc target with Doxygen
ADD_SUBDIRECTORY(doc)

########### CPack Packaging ###########

# Currently only testing on Apple
#IF(APPLE)
#  INCLUDE(BundleConfig)
#ENDIF(APPLE)
