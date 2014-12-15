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

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src/libraries
  ${CMAKE_SOURCE_DIR}/src/orxonox
  ${CMAKE_CURRENT_SOURCE_DIR}
)

################ Sub Directories ################

ADD_SUBDIRECTORY(designtools)
ADD_SUBDIRECTORY(gametypes)
ADD_SUBDIRECTORY(notifications)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(overlays)
ADD_SUBDIRECTORY(pickup)
ADD_SUBDIRECTORY(pong)
ADD_SUBDIRECTORY(portals)
ADD_SUBDIRECTORY(questsystem)
ADD_SUBDIRECTORY(tetris)
ADD_SUBDIRECTORY(weapons)
ADD_SUBDIRECTORY(docking)
ADD_SUBDIRECTORY(towerdefense)
ADD_SUBDIRECTORY(invader)
ADD_SUBDIRECTORY(mini4Dgame)
ADD_SUBDIRECTORY(dodgerace2)
