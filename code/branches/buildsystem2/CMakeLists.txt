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
 
PROJECT(Orxonox C CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

SET(ORXONOX_VERSION_MAJOR 0)
SET(ORXONOX_VERSION_MINOR 1)
SET(ORXONOX_VERSION_PATCH 0)
SET(ORXONOX_VERSION 0.1.0)

# Keep devs from using the root directory as binary directory (messes up the source tree)
IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "Do not use the root directory as CMake output directory! mkdir build; cd build; cmake ..")
ENDIF()

# This sets where to look for modules (e.g. "Find*.cmake" files)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Library Config
INCLUDE(LibraryConfig)

# Build Config
INCLUDE(BuildConfig)

# Creates the actual project
ADD_SUBDIRECTORY(src)

# Configure the binary output directory
ADD_SUBDIRECTORY(bin-config)
