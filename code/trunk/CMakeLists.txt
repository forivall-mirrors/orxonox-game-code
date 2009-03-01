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
 
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# Keep devs from using the root directory as binary directory (messes up the source tree)
IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "Please do not use the root directory as CMake output directory!
  mkdir build; cd build; cmake ..
  And you will have to clean the source directory by deleting CMakeCache.txt and the folder CMakeFiles")
ENDIF()


PROJECT(Orxonox C CXX)

SET(ORXONOX_VERSION_MAJOR 0)
SET(ORXONOX_VERSION_MINOR 0)
SET(ORXONOX_VERSION_PATCH 2)
SET(ORXONOX_VERSION 0.0.2)
SET(ORXONOX_VERSION_NAME "Bellatrix")


# This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

IF(UNIX AND NOT APPLE)
  INCLUDE(FlagUtilities)
  SET_CACHE(CMAKE_INSTALL_PREFIX PATH "" /usr) # default: /usr/local
ENDIF()

# Library finding
INCLUDE(LibraryConfig)

# General build and compiler options and configurations
INCLUDE(BuildConfig)

# Configure media directory location and installation
ADD_SUBDIRECTORY(media)

# Remaining tardis hack
ADD_SUBDIRECTORY(config)

# Create the actual project
ADD_SUBDIRECTORY(src)

# Configure the binary output directory. Do this after src!
ADD_SUBDIRECTORY(bin)

# Last but not least: Try to make doxygen target
ADD_SUBDIRECTORY(doc)
