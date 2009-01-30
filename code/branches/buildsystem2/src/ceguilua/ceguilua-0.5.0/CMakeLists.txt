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

# Copy all files from the package folder to the binary directory

# Only do this once since GLOB_RECURSE is quite slow
# No, don't. When switch back from 0.6 to 0.5 and keeping the binary dir
# This will cause problems.
#IF(EXISTS ${CEGUILUA_BINARY_DIR}/exceptions.lua)
#  RETURN()
#ENDIF(EXISTS ${CEGUILUA_BINARY_DIR}/exceptions.lua)

FILE(GLOB_RECURSE _package_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/package "*.pkg")
FOREACH(_file ${_package_files} "exceptions.lua")
  CONFIGURE_FILE(package/${_file} ${CEGUILUA_BINARY_DIR}/${_file} COPYONLY)
ENDFOREACH(_file)
