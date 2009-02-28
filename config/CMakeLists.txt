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
 
IF(TARDIS)
  # OGRE can't find fonts to display config screen on Tardis,
  # so providing default config file here (bug).
  # Cause: Ogre 1.4.5 was compiled for an older debian version
  IF(NOT EXISTS ${CMAKE_CONFIG_OUTPUT_DIRECTORY}/ogre.cfg)
    CONFIGURE_FILE(ogre.cfg ${CMAKE_CONFIG_OUTPUT_DIRECTORY}/ogre.cfg COPYONLY)
  ENDIF()
ENDIF(TARDIS)
