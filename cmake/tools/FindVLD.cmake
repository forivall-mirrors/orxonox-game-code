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
 #    Finds the Visual Leak Detector if installed
 #

INCLUDE(FindPackageHandleStandardArgs)

FIND_LIBRARY(VLD_LIBRARY
  NAMES vld
  PATH_SUFFIXES "Visual Leak Detector/lib/Win32" "../../lib/Win32"
)

# Handle the REQUIRED argument and set POCO_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VLD DEFAULT_MSG
  VLD_LIBRARY
)

MARK_AS_ADVANCED(
  VLD_LIBRARY
)
