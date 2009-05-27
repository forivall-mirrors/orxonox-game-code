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

SET_SOURCE_FILES(UTIL_FILES
  CRC32.h
  Clipboard.h
  Convert.h
  Debug.h
  Exception.h
  ExprParser.h
  Math.h
  MathConvert.h
  MultiType.h
  MultiTypeValue.h
  OrxEnum.h
  OutputBuffer.h
  OutputHandler.h
  SignalHandler.h
  Sleep.h
  String.h
  SubString.h
  UtilPrereqs.h
  mbool.h
  
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
  String.cc
  SubString.cc
)
#GET_ALL_HEADER_FILES(UTIL_HDR_FILES)
#SET(UTIL_FILES ${UTIL_SRC_FILES} ${UTIL_HDR_FILES})
GENERATE_SOURCE_GROUPS(${UTIL_FILES})
# Also add OrxonoxConfig to have it least somewhere in the IDE
LIST(APPEND UTIL_FILES
  ${CMAKE_BINARY_DIR}/src/OrxonoxConfig.h
  ${CMAKE_SOURCE_DIR}/src/OrxonoxConfig.h.in
  ${CMAKE_BINARY_DIR}/src/SpecialConfig.h
  ${CMAKE_SOURCE_DIR}/src/SpecialConfig.h.in
)
SOURCE_GROUP("" FILES
  ${CMAKE_BINARY_DIR}/src/OrxonoxConfig.h
  ${CMAKE_SOURCE_DIR}/src/OrxonoxConfig.h.in
  ${CMAKE_BINARY_DIR}/src/SpecialConfig.h
  ${CMAKE_SOURCE_DIR}/src/SpecialConfig.h.in
)

IF(GCC_NO_SYSTEM_HEADER_SUPPORT)
  # Get around displaying a few hundred lines of warning code
  SET_SOURCE_FILES_PROPERTIES(MultiType.cc PROPERTIES COMPILE_FLAGS "-w")
ENDIF()

ADD_LIBRARY(util SHARED ${UTIL_FILES})
SET_TARGET_PROPERTIES(util PROPERTIES DEFINE_SYMBOL "UTIL_SHARED_BUILD")
TARGET_LINK_LIBRARIES(util ${OGRE_LIBRARY})

ORXONOX_INSTALL(util)
