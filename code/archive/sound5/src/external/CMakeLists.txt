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

# Include CEGUILua if not requested otherwise
IF(CEGUILUA_USE_INTERNAL_LIBRARY)
  IF(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ceguilua/ceguilua-${CEGUI_VERSION})
    MESSAGE(FATAL_ERROR "CEGUILua version not found in src folder. Update list of supported versions in LibraryConfig.cmake!")
  ENDIF()

  ADD_SUBDIRECTORY(ceguilua)
  SET(CEGUILUA_LIBRARY ${CEGUILUA_LIBRARY} PARENT_SCOPE)
ENDIF()

ADD_SUBDIRECTORY(bullet)
ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(ogreceguirenderer)
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)
