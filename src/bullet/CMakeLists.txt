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

SET_SOURCE_FILES(BULLET_FILES
  btBulletCollisionCommon.h
  btBulletDynamicsCommon.h
  Bullet-C-Api.h
)
ADD_SUBDIRECTORY(BulletCollision)
ADD_SUBDIRECTORY(BulletDynamics)
ADD_SUBDIRECTORY(LinearMath)

GENERATE_SOURCE_GROUPS(${BULLET_FILES})

# No warnings needed from third party libraries
REMOVE_COMPILER_FLAGS("-W3 -W4" MSVC)
ADD_COMPILER_FLAGS("-w")

IF(MSVC)
  ADD_LIBRARY(bullet_orxonox STATIC ${BULLET_FILES})
ELSE(MSVC)
  ADD_LIBRARY(bullet_orxonox SHARED ${BULLET_FILES})
  ORXONOX_INSTALL(bullet_orxonox)
ENDIF(MSVC)

SET_TARGET_PROPERTIES(bullet_orxonox PROPERTIES VERSION 2.73)
