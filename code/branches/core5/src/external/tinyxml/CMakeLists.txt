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

SET_SOURCE_FILES(TINYXML++_FILES
  ticpp.h
  ticpprc.h
  tinystr.h
  tinyxml.h

COMPILATION_BEGIN TicppCompilation.cpp
  ticpp.cpp
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
COMPILATION_END
)

ORXONOX_ADD_LIBRARY(tinyxml++_orxonox
  ORXONOX_EXTERNAL
  NO_DLL_INTERFACE
  VERSION
    2.5.3
  SOURCE_FILES
    ${TINYXML++_FILES}
)
