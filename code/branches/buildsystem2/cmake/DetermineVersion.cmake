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

MACRO(DetermineVersion _name _file _version_var)
  FILE(READ ${_file} _file_content)
  IF(_file_content)
    STRING(REGEX MATCH "${_name}_VERSION_MAJOR +([0-9]+)" _dummyvar ${_file_content})
    SET(_version "${CMAKE_MATCH_1}")
    STRING(REGEX MATCH "${_name}_VERSION_MINOR +([0-9]+)" _dummyvar ${_file_content})
    SET(_version "${_version}.${CMAKE_MATCH_1}")
    STRING(REGEX MATCH "${_name}_VERSION_PATCH +([0-9]+)" _dummyvar ${_file_content})
    SET(_version "${_version}.${CMAKE_MATCH_1}")
    SET(${_version_var} "${_version}")
  ENDIF(_file_content)
ENDMACRO(DetermineVersion)
