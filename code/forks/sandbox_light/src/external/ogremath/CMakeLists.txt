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

SET_SOURCE_FILES(OGREMATH_SRC_FILES
COMPILATION_BEGIN OgreCompilation.cc
  OgreColourValue.cpp
  OgreMath.cpp
  OgreMatrix3.cpp
  OgreMatrix4.cpp
  OgreQuaternion.cpp
  OgreVector2.cpp
  OgreVector3.cpp
  OgreVector4.cpp
COMPILATION_END
)

IF(WIN32)
  ADD_SOURCE_FILES(OGREMATH_SRC_FILES WIN32/OgreTimer.cpp)
ELSEIF(APPLE)
  ADD_SOURCE_FILES(OGREMATH_SRC_FILES OSX/OgreTimer.cpp)
ELSEIF(UNIX)
  ADD_SOURCE_FILES(OGREMATH_SRC_FILES GLX/OgreTimer.cpp)
ENDIF()

ORXONOX_ADD_LIBRARY(ogremath_orxonox
  ORXONOX_EXTERNAL
  FIND_HEADER_FILES
  DEFINE_SYMBOL
    "OGRE_NONCLIENT_BUILD"
  SOURCE_FILES
    ${OGREMATH_SRC_FILES}
) 
