PROJECT(Orxonox C CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

SET(ORXONOX_VERSION_MAJOR 0)
SET(ORXONOX_VERSION_MINOR 1)
SET(ORXONOX_VERSION_PATCH 0)
SET(ORXONOX_VERSION 0.1.0)

# Keep devs from using the root directory as binary directory (messes up the source tree)
IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "Do not use the root directory as CMake output directory! mkdir build; cd build; cmake ..")
ENDIF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})

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
