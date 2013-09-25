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
 #    Reto Grieder, Adrian Friedli
 #  Description:
 #    Sets the right compiler and linker flags for GCC.
 #

INCLUDE(FlagUtilities)
INCLUDE(CompareVersionStrings)
INCLUDE(CheckCXXCompilerFlag)

# Shortcut for CMAKE_COMPILER_IS_GNUCXX and ..._GNUC
SET(CMAKE_COMPILER_IS_GNU TRUE)

# Determine compiler version
EXEC_PROGRAM(
  ${CMAKE_CXX_COMPILER}
  ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
  OUTPUT_VARIABLE GCC_VERSION
)

# GCC only supports PCH in versions 3.4 and above
INCLUDE(CompareVersionStrings)
COMPARE_VERSION_STRINGS("${GCC_VERSION}" "3.4.0" _compare_result)
IF(_compare_result GREATER -1)
  SET(PCH_COMPILER_SUPPORT TRUE)
ENDIF()

# __COUNTER__ macro was only added in GCC 4.3
# It might be required to make full build units work
COMPARE_VERSION_STRINGS("${GCC_VERSION}" "4.3.0" _compare_result)
IF(_compare_result GREATER -1)
  SET(HAVE_COUNTER_MACRO TRUE)
ENDIF()

# Also include environment flags. Could cause conflicts though
SET_COMPILER_FLAGS("$ENV{CXXFLAGS}" CXX CACHE)
SET_COMPILER_FLAGS("$ENV{CFLAGS}"   C   CACHE)

# These flags get added to the flags above
SET_COMPILER_FLAGS("    -g -ggdb -D_DEBUG" Debug          CACHE)
SET_COMPILER_FLAGS("             -DNDEBUG" ReleaseAll     CACHE)
ADD_COMPILER_FLAGS("-O2 -g -ggdb"          RelForDevs     CACHE)
ADD_COMPILER_FLAGS("-O3 -g -ggdb"          RelWithDebInfo CACHE)
ADD_COMPILER_FLAGS("-O3"                   Release        CACHE)
ADD_COMPILER_FLAGS("-Os"                   MinSizeRel     CACHE)

# CMake doesn't seem to set the PIC flags right on certain 64 bit systems
IF(NOT MINGW AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  ADD_COMPILER_FLAGS("-fPIC" CACHE)
ENDIF()

# Never omit frame pointers that could interfere with proper stack traces
ADD_COMPILER_FLAGS("-fno-omit-frame-pointer" CACHE)

# Enable non standard floating point optimisations
ADD_COMPILER_FLAGS("-ffast-math" CACHE)

# Use SSE if possible
# Commented because this might not work for cross compiling
#CHECK_CXX_COMPILER_FLAG(-msse _gcc_have_sse)
#IF(_gcc_have_sse)
#  ADD_COMPILER_FLAGS("-msse" CACHE)
#ENDIF()

IF(NOT MINGW)
  # Have GCC visibility?
  CHECK_CXX_COMPILER_FLAG("-fvisibility=hidden" _gcc_have_visibility)
  IF(_gcc_have_visibility)
    # Note: There is a possible bug with the flag in gcc < 4.2 and Debug versions
    COMPARE_VERSION_STRINGS("${GCC_VERSION}" "4.2.0" _compare_result)
    IF(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" OR _compare_result GREATER -1)
      ADD_COMPILER_FLAGS("-DORXONOX_GCC_VISIBILITY -fvisibility=default -fvisibility-inlines-hidden" CACHE)
    ENDIF()
  ENDIF(_gcc_have_visibility)
ENDIF()

# We have some unconformant code, disable an optimisation feature
ADD_COMPILER_FLAGS("-fno-strict-aliasing" CACHE)

# For newer GCC (4.3 and above), don't display hundreds of annoying deprecated
# messages. Other versions don't seem to show any such warnings at all.
ADD_COMPILER_FLAGS("-Wno-deprecated" CXX CACHE)

# Always show why a precompiled header file could not be used
ADD_COMPILER_FLAGS("-Winvalid-pch" CXX CACHE)

# Increase warning level if requested
IF(EXTRA_COMPILER_WARNINGS)
  ADD_COMPILER_FLAGS("-Wall -Wextra -Wno-unused-parameter" CACHE)
ELSE()
  REMOVE_COMPILER_FLAGS("-Wextra -Wno-unused-parameter" CACHE)
  ADD_COMPILER_FLAGS("-Wall" CACHE)
ENDIF()

# Linker flags
IF(LINUX)
  # Don't allow undefined symbols in a shared library
  SET_LINKER_FLAGS("-Wl,--no-undefined" CACHE)
ENDIF()

# Add compiler and linker flags for MinGW
IF (MINGW)
  ADD_COMPILER_FLAGS("-gstabs+" Debug RelForDevs RelWithDebInfo CACHE)

  ADD_LINKER_FLAGS("-enable-auto-import" CACHE)
ENDIF()
