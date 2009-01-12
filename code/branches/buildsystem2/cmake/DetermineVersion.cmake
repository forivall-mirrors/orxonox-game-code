#    DetermineVersion.cmake - CMake Module to get the version of a library from
#                             a header file.
#    Author: Reto '1337' Grieder (2009)
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

MACRO(DETERMINE_VERSION _name _file)
  FILE(READ ${_file} _file_content)
  IF(_file_content)
    SET(_parts ${ARGN})
    LIST(LENGTH _parts _parts_length)
    IF(NOT ${_parts_length} EQUAL 3)
      SET(_parts MAJOR MINOR PATCH)
    ENDIF(NOT ${_parts_length} EQUAL 3)

    FOREACH(_part ${_parts})
      STRING(REGEX MATCH "${_name}_VERSION_${_part} +([0-9]+)" _match ${_file_content})
      IF(_match)
        SET(${_name}_VERSION_${_part} ${CMAKE_MATCH_1} CACHE STRING "")
      ELSE(_match)
        SET(${_name}_VERSION_${_part} "0" CACHE STRING "")
      ENDIF(_match)
      MARK_AS_ADVANCED(${_name}_VERSION_${_part})
      IF("${_parts}" MATCHES "^${_part}") # First?
        SET(${_name}_VERSION "${${_name}_VERSION_${_part}}" CACHE STRING "" FORCE)
      ELSE("${_parts}" MATCHES "^${_part}")
        SET(${_name}_VERSION "${${_name}_VERSION}.${${_name}_VERSION_${_part}}" CACHE STRING "" FORCE)
      ENDIF("${_parts}" MATCHES "^${_part}")
    ENDFOREACH(_part)
    MARK_AS_ADVANCED(${_name}_VERSION)
  ENDIF(_file_content)
ENDMACRO(DETERMINE_VERSION)
