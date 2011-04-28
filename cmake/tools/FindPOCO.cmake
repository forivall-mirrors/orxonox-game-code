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
 #    Finds the POCO Foundation library.
 #

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(POCO_INCLUDE_DIR Poco/Poco.h
  PATHS $ENV{POCODIR}
  PATH_SUFFIXES include Foundation/include
)
FIND_LIBRARY(POCO_LIBRARY_OPTIMIZED
  NAMES PocoFoundation
  PATHS $ENV{POCODIR}
  PATH_SUFFIXES lib
)
FIND_LIBRARY(POCO_LIBRARY_DEBUG
  NAMES PocoFoundationd PocoFoundation_d PocoFoundationD PocoFoundation_D
  PATHS $ENV{POCODIR}
  PATH_SUFFIXES lib
)

# Handle the REQUIRED argument and set POCO_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(POCO DEFAULT_MSG
  POCO_LIBRARY_OPTIMIZED
  POCO_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(POCO)

MARK_AS_ADVANCED(
  POCO_INCLUDE_DIR
  POCO_LIBRARY_OPTIMIZED
  POCO_LIBRARY_DEBUG
)
