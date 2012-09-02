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

################ Sub Directories ################

ADD_SUBDIRECTORY(tolua)
ADD_SUBDIRECTORY(bullet)
ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(enet)
ADD_SUBDIRECTORY(loki)
IF(CEGUI_OGRE_RENDERER_BUILD_REQUIRED)
  ADD_SUBDIRECTORY(ogreceguirenderer)
ENDIF()
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)
