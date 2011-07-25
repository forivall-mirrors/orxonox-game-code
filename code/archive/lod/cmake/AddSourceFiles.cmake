#    AddSourceFiles.cmake - CMake Module to include source files in subdirectories.
#    Author: Reto '1337' Grieder (2008)
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

MACRO(ADD_SOURCE_DIRECTORY _target_list _directory)

  # Subfolder puts source files into CMake Cache variable _CACHED_SOURCE_FILES
  ADD_SUBDIRECTORY(${_directory})

  # Put the directory name in front of each source file from the subfolder
  # and add it to the source list in the current directory
  FOREACH(_source_file ${_CACHED_SOURCE_FILES})
    LIST(APPEND ${_target_list} "${_directory}/${_source_file}")
  ENDFOREACH(_source_file)

ENDMACRO(ADD_SOURCE_DIRECTORY)


MACRO(ADD_SOURCE_FILES _source_list)
  
  # Put the source file into a variable that still exists in this_folder/../
  # Use FORCE to always overwrite the cache variable
  SET(_CACHED_SOURCE_FILES ${${_source_list}} CACHE STRING "" FORCE)
  MARK_AS_ADVANCED(_CACHED_SOURCE_FILES FORCE)

ENDMACRO(ADD_SOURCE_FILES)
