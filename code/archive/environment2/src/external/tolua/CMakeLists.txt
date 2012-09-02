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

################## Tolua++ generator ##################

ORXONOX_ADD_EXECUTABLE(toluaapp_orxonox
  ORXONOX_EXTERNAL
  VERSION
    1.0.92
  NO_INSTALL
  LINK_LIBRARIES
    ${LUA5.1_LIBRARY}
  SOURCE_FILES
    tolua.c
)

# Set some variables to the cache in order to use them in the TOLUA macro
SET(TOLUA_PARSER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/all-5.1.lua" CACHE INTERNAL "")
SET(TOLUA_PARSER_DEPENDENCIES
  toluaapp_orxonox
  ${CMAKE_CURRENT_SOURCE_DIR}/all-5.1.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/compat-5.1.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/compat.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/basic.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/feature.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/verbatim.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/code.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/typedef.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/container.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/package.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/module.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/namespace.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/define.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/enumerate.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/declaration.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/variable.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/array.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/function.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/operator.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/class.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/clean.lua
  ${CMAKE_CURRENT_SOURCE_DIR}/lua/doit.lua

  CACHE INTERNAL ""
)
