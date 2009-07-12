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
  Clipboard.cc
  CRC32.cc
  Exception.cc
  ExprParser.cc
  Math.cc
  MultiType.cc
  OutputBuffer.cc
  OutputHandler.cc
  SignalHandler.cc
  Sleep.cc
  StringUtils.cc
  SubString.cc
)

IF(GCC_NO_SYSTEM_HEADER_SUPPORT)
  # Get around displaying a few hundred lines of warning code
  SET_SOURCE_FILES_PROPERTIES(MultiType.cc PROPERTIES COMPILE_FLAGS "-w")
ENDIF()

ORXONOX_ADD_LIBRARY(util
  FIND_HEADER_FILES
  DEFINE_SYMBOL
    "UTIL_SHARED_BUILD"
  LINK_LIBRARIES
    ${OGRE_LIBRARY}
  SOURCE_FILES
    ${UTIL_SRC_FILES}
)
