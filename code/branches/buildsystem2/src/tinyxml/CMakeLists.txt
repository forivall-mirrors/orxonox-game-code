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

SET(TINYXML++_FILES
  ticpp.h
  ticpprc.h
  tinystr.h
  tinyxml.h

  ticpp.cpp
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
)
GENERATE_SOURCE_GROUPS(${TINYXML++_FILES})

IF(MSVC)
  ADD_LIBRARY(tinyxml++_orxonox STATIC ${TINYXML++_FILES})
ELSE()
  ADD_LIBRARY(tinyxml++_orxonox SHARED ${TINYXML++_FILES})
ENDIF()

ORXONOX_INSTALL(tinyxml++_orxonox)
