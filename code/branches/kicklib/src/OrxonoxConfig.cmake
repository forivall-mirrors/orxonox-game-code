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

# Use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# Global switch to disable Precompiled Header Files
# Note: PCH temporarily disabled on Mac because of severe problems
IF(PCH_COMPILER_SUPPORT AND NOT APPLE)
  OPTION(PCH_ENABLE "Global PCH switch" TRUE)
ENDIF()

# Global switch to disable multiple file compilations
OPTION(DISABLE_COMPILATIONS "Global multi-file compilation switch" FALSE)

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
SET(ORXONOX_PLATFORM_APPLE ${APPLE})
SET(ORXONOX_PLATFORM_UNIX ${UNIX})
IF(UNIX AND NOT APPLE)
  SET(ORXONOX_PLATFORM_LINUX TRUE)
ENDIF()

# Check __forceinline
IF(MSVC)
  INCLUDE(CheckCXXSourceCompiles)
  SET(_source "int main() { return 0; } __forceinline void test() { return; }")
  CHECK_CXX_SOURCE_COMPILES("${_source}" HAVE_FORCEINLINE)
ENDIF(MSVC)

# Part of a woraround for OS X warnings. See OrxonoxConfig.h.in
IF(HAVE_STDINT_H)
  SET(HAVE_STDINT_H 1)
ELSE()
  SET(HAVE_STDINT_H 0)
ENDIF()

# Check iso646.h include (literal operators)
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(iso646.h HAVE_ISO646_H)

IF(MSVC)
  # Check whether we can use Visual Leak Detector
  FIND_FILE(VLD_DLL vld_x86.dll)
  IF(VLD_DLL)
    SET(HAVE_VLD TRUE)
    OPTION(VISUAL_LEAK_DETECTOR_ENABLE "Memory leak detector" off)
    # Make sure the value is "on" or "off" for vld.ini
    IF(VISUAL_LEAK_DETECTOR_ENABLE)
      SET(VISUAL_LEAK_DETECTOR_ENABLE on)
    ELSE()
      SET(VISUAL_LEAK_DETECTOR_ENABLE off)
    ENDIF()
  ENDIF()
  MARK_AS_ADVANCED(VLD_DLL)
ENDIF()

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
