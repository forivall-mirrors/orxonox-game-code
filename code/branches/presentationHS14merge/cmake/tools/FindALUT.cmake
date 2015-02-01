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
 #    Kevin Young
 #  Description:
 #    Variables defined:
 #      ALUT_FOUND
 #      ALUT_INCLUDE_DIR
 #      ALUT_LIBRARY
 #

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(ALUT_INCLUDE_DIR alut.h
  PATHS $ENV{ALUTDIR}
  PATH_SUFFIXES include include/AL
)
FIND_LIBRARY(ALUT_LIBRARY_OPTIMIZED
  NAMES alut ALUT
  PATHS $ENV{ALUTDIR}
  PATH_SUFFIXES lib bin/Release bin/release Release release ALUT
)
FIND_LIBRARY(ALUT_LIBRARY_DEBUG
  NAMES alutd alut_d alutD alut_D ALUTd ALUT_d ALUTD ALUT_D
  PATHS $ENV{ALUTDIR}
  PATH_SUFFIXES lib bin/Debug bin/debug Debug debug ALUT
)

# Handle the REQUIRED argument and set ALUT_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALUT DEFAULT_MSG
  ALUT_LIBRARY_OPTIMIZED
  ALUT_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(ALUT)

MARK_AS_ADVANCED(
  ALUT_INCLUDE_DIR
  ALUT_LIBRARY_OPTIMIZED
  ALUT_LIBRARY_DEBUG
)
