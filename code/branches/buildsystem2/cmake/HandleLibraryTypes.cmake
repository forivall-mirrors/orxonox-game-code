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

MACRO(HandleLibraryTypes _name)
  # Additional libraries can be added as additional arguments
  IF(${_name}_FOUND)
    IF(${_name}_LIBRARY_DEBUG)
      SET(${_name}_LIBRARY
          OPTIMIZED ${${_name}_LIBRARY_OPTIMIZED} ${ARGN}
          DEBUG     ${${_name}_LIBRARY_DEBUG}     ${ARGN}
          CACHE STRING "${_name} library file(s)" FORCE
      )
    ELSE(${_name}_LIBRARY_DEBUG)
      SET(${_name}_LIBRARY
          ${${_name}_LIBRARY_OPTIMIZED} ${ARGN}
          CACHE STRING "${_name} library file(s)" FORCE
      )
    ENDIF(${_name}_LIBRARY_DEBUG)
  ENDIF(${_name}_FOUND)
ENDMACRO(HandleLibraryTypes)
