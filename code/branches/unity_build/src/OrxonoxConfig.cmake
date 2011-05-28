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
 #    Configures the header files OrxonoxConfig.h and SpecialConfig.h
 #    and sets some other options. All the build related options should be
 #    found and set here if possible.
 #

#################### Options ####################

# Global switch to disable Precompiled Header Files
# Note: PCH temporarily disabled on Mac because of severe problems
IF(PCH_COMPILER_SUPPORT AND NOT APPLE)
  OPTION(PCH_ENABLE "Global PCH switch" TRUE)
ENDIF()

# Global option to steer building muliple files as a single one
# off/false: Turn off completely
# partial:   Only combine files explicitly specified with BUILD_UNIT
# full##:    Use ## source files per orxonox library and use manual build units
#            for external dependencies. Example: full8 will in general use 8
#            source files per library, but more specifically tries to occupy
#            8 CPU threads.
#            This is configured manually in BuildUnitsConfig.cmake
SET(ENABLE_BUILD_UNITS "partial" CACHE STRING "Enables building multiple source files as one.")
IF(ENABLE_BUILD_UNITS)
  IF(NOT "${ENABLE_BUILD_UNITS}" STREQUAL "partial")
    STRING(REGEX REPLACE "^full([1-9][0-9]?)$" "\\1" _nr_of_units "${ENABLE_BUILD_UNITS}")
    IF("${_nr_of_units}" STREQUAL "${ENABLE_BUILD_UNITS}") # Regex match failed
      MESSAGE(FATAL_ERROR "Unrecognised option for ENABLE_BUILD_UNITS: ${ENABLE_BUILD_UNITS}")
    ELSE()
      SET(NR_OF_BUILD_UNITS ${_nr_of_units})
    ENDIF()
  ENDIF()
  IF(CMAKE_COMPILER_IS_GNU)
    INCLUDE(BuildUnitsConfigGCC.cmake)
  ELSEIF(MSVC)
    INCLUDE(BuildUnitsConfigMSVC.cmake)
  ENDIF()
ENDIF()

# Use WinMain() or main()?
IF(WIN32)
  OPTION(ORXONOX_USE_WINMAIN "Use WinMain (doesn't show console) or main" FALSE)
ENDIF()

################ Platform Config ################

# Check endianness
INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(ORXONOX_BIG_ENDIAN)
IF(NOT ORXONOX_BIG_ENDIAN)
  SET(ORXONOX_LITTLE_ENDIAN TRUE)
ENDIF()

# Platforms
SET(ORXONOX_PLATFORM_WINDOWS ${WIN32})
SET(ORXONOX_PLATFORM_APPLE   ${APPLE})
SET(ORXONOX_PLATFORM_UNIX    ${UNIX})
SET(ORXONOX_PLATFORM_LINUX   ${LINUX})

# Check __forceinline
IF(MSVC)
  INCLUDE(CheckCXXSourceCompiles)
  SET(_source "int main() { return 0; } __forceinline void test() { return; }")
  CHECK_CXX_SOURCE_COMPILES("${_source}" HAVE_FORCEINLINE)
ENDIF(MSVC)

# Check some non standard system includes
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(iso646.h HAVE_ISO646_H)
CHECK_INCLUDE_FILE_CXX(stdint.h HAVE_STDINT_H)

# Part of a woraround for OS X warnings. See OrxonoxConfig.h.in
SET(ORX_HAVE_STDINT_H ${HAVE_STDINT_H})

# XCode and Visual Studio support multiple configurations. In order to tell
# about the active one we have to define the macro for each configuration
ADD_COMPILER_FLAGS("-DCMAKE_Debug_BUILD"          Debug)
ADD_COMPILER_FLAGS("-DCMAKE_RelForDevs_BUILD"     RelForDevs)
ADD_COMPILER_FLAGS("-DCMAKE_Release_BUILD"        Release)
ADD_COMPILER_FLAGS("-DCMAKE_RelWithDebInfo_BUILD" RelWithDebInfo)
ADD_COMPILER_FLAGS("-DCMAKE_MinSizeRel_BUILD"     MinSizeRel)

############## Configured Headers ###############

SET(GENERATED_FILE_COMMENT
   "DO NOT EDIT THIS FILE! <br>
    It has been automatically generated by CMake from OrxonoxConfig.h.in")
# Copy and configure OrxonoxConfig which gets included in every file
CONFIGURE_FILE(OrxonoxConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/OrxonoxConfig.h)
# This file only gets included by very few classes to avoid a large recompilation
CONFIGURE_FILE(SpecialConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/SpecialConfig.h)

SET(ORXONOX_CONFIG_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/OrxonoxConfig.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/SpecialConfig.h.in
)
SET(ORXONOX_CONFIG_FILES_GENERATED
  ${CMAKE_CURRENT_BINARY_DIR}/OrxonoxConfig.h
  ${CMAKE_CURRENT_BINARY_DIR}/SpecialConfig.h
)

# Make special target including the configured header files for Visual Studio
IF(MSVC)
  ADD_CUSTOM_TARGET(config
    SOURCES
      ${ORXONOX_CONFIG_FILES}
      ${ORXONOX_CONFIG_FILES_GENERATED}
  )
ENDIF()
