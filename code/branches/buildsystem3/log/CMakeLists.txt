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
 
SET(ORXONOX_LOG_DEV_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

# Create some directories for the logs or CEGUI crashes when setting up the log
IF(CMAKE_CONFIGURATION_TYPES)
  FOREACH(_configuration ${CMAKE_CONFIGURATION_TYPES})
    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_configuration})
  ENDFOREACH(_configuration)
ENDIF()

# Also create the folder when installing
INSTALL(CODE "FILE(MAKE_DIRECTORY \"${ORXONOX_LOG_INSTALL_PATH}\")")
