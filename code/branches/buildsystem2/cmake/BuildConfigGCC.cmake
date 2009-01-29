 #
 #   ORXONOX - the hottest 3D action shooter ever to exist
 #                    > www.orxonox.net <
 #
 #
 #   License notice:
 #
 #   This program is free software; you can redistribute it and/or
 #   modify it under the terms of the GNU General Public License
 #   as published by the Free Software Foundation; either version 2
 #   of the License, or (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program; if not, write to the Free Software
 #   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #   Author:
 #      Reto Grieder
 #   Co-authors:
 #      ...
 #

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
ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")

# Increase warning level if requested
IF(EXTRA_WARNINGS)
  REMOVE_COMPILER_FLAGS("-Wall" CACHE)
  ADD_COMPILER_FLAGS("-Wextra --Wno-unused-parameter" CACHE)
ELSE(EXTRA_WARNINGS)
  REMOVE_COMPILER_FLAGS("-Wextra --Wno-unused-parameter" CACHE)
  ADD_COMPILER_FLAGS("-Wall" CACHE)
ENDIF(EXTRA_WARNINGS) 

# General linker flags
SET_LINKER_FLAGS("--no-undefined" CACHE)
