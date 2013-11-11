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

SET_SOURCE_FILES(UTIL_SRC_FILES

BUILD_UNIT StableBuildUnit.cc
  Clipboard.cc
  Clock.cc
  Convert.cc
  CRC32.cc
  ExprParser.cc
  Scope.cc
  ScopedSingletonManager.cc
  SharedPtr.cc
  Sleep.cc
  SmallObjectAllocator.cc
  SubString.cc
END_BUILD_UNIT

  MultiType.cc
  Exception.cc
  Math.cc
  SignalHandler.cc
  StringUtils.cc
)

ADD_SUBDIRECTORY(output)

ORXONOX_ADD_LIBRARY(util
  FIND_HEADER_FILES
  LINK_LIBRARIES
    ${CEGUI_LIBRARY}
    ${OGRE_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
  SOURCE_FILES
    ${UTIL_SRC_FILES}
)

IF (DBGHELP_FOUND)
  TARGET_LINK_LIBRARIES(util ${DBGHELP_LIBRARY})
ENDIF()
