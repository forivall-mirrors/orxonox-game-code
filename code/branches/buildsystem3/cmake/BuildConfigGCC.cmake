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

# Determine compiler version
EXEC_PROGRAM(
  ${CMAKE_CXX_COMPILER}
  ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
  OUTPUT_VARIABLE GCC_VERSION
)

# GCC may not support #pragma GCC system_header correctly when using
# templates. According to Bugzilla, it was fixed March 07 but tests
# have confirmed that GCC 4.0.0 does not pose a problem for our cases.
INCLUDE(CompareVersionStrings)
COMPARE_VERSION_STRINGS("${GCC_VERSION}" "4.0.0" _compare_result)
IF(_compare_result LESS 0)
  SET(GCC_NO_SYSTEM_HEADER_SUPPORT TRUE)
ENDIF()

# Also include environment flags. Could cause conflicts though
SET_COMPILER_FLAGS("$ENV{CXXFLAGS}" CXX CACHE)
SET_COMPILER_FLAGS("$ENV{CFLAGS}"   C   CACHE)

# These flags get added to the flags above
SET_COMPILER_FLAGS("    -g -ggdb -D_DEBUG" Debug          CACHE)
SET_COMPILER_FLAGS("             -DNDEBUG" ReleaseAll     CACHE)
ADD_COMPILER_FLAGS("-O3"                   Release        CACHE)
ADD_COMPILER_FLAGS("-O2 -g -ggdb"          RelWithDebInfo CACHE)
ADD_COMPILER_FLAGS("-Os"                   MinSizeRel     CACHE)

# CMake doesn't seem to set the PIC flags right on certain 64 bit systems
IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  ADD_COMPILER_FLAGS("-fPIC" CACHE)
ENDIF()

# For GCC older than version 4, do not display sign compare warings
# because of boost::filesystem (which creates about a hundred per include)
ADD_COMPILER_FLAGS("-Wno-sign-compare" GCC_NO_SYSTEM_HEADER_SUPPORT CACHE)

# Increase warning level if requested
IF(EXTRA_COMPILER_WARNINGS)
  REMOVE_COMPILER_FLAGS("-Wall" CACHE)
  ADD_COMPILER_FLAGS("-Wextra --Wno-unused-parameter" CACHE)
ELSE()
  REMOVE_COMPILER_FLAGS("-Wextra --Wno-unused-parameter" CACHE)
  ADD_COMPILER_FLAGS("-Wall" CACHE)
ENDIF()

# General linker flags
SET_LINKER_FLAGS("--no-undefined" CACHE)
