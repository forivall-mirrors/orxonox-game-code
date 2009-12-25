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

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src/external
  ${CMAKE_SOURCE_DIR}/src/libraries
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET_SOURCE_FILES(ORXONOX_SRC_FILES
  Level.cc
  LevelManager.cc
  Main.cc
  MoodManager.cc
  PawnManager.cc
  PlayerManager.cc
  Radar.cc
COMPILATION_BEGIN SceneCompilation.cc
  CameraManager.cc
  Scene.cc
COMPILATION_END
)

ADD_SUBDIRECTORY(collisionshapes)
ADD_SUBDIRECTORY(controllers)
ADD_SUBDIRECTORY(gamestates)
ADD_SUBDIRECTORY(gametypes)
ADD_SUBDIRECTORY(graphics)
ADD_SUBDIRECTORY(infos)
ADD_SUBDIRECTORY(interfaces)
ADD_SUBDIRECTORY(items)
ADD_SUBDIRECTORY(overlays)
ADD_SUBDIRECTORY(pickup)
ADD_SUBDIRECTORY(sound)
ADD_SUBDIRECTORY(weaponsystem)
ADD_SUBDIRECTORY(worldentities)

ORXONOX_ADD_LIBRARY(orxonox
  FIND_HEADER_FILES
  TOLUA_FILES
    LevelManager.h
    MoodManager.h
    controllers/HumanController.h
    pickup/BaseItem.h
    pickup/PickupInventory.h
    sound/SoundManager.h
  DEFINE_SYMBOL
    "ORXONOX_SHARED_BUILD"
  PCH_FILE
    OrxonoxPrecompiledHeaders.h
  LINK_LIBRARIES
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY} # Filesystem dependency
    ${Boost_THREAD_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY} # Thread dependency
    ${OGRE_LIBRARY}
    ${OPENAL_LIBRARY}
    ${ALUT_LIBRARY}
    ${VORBISFILE_LIBRARY}
    ${VORBIS_LIBRARY}
    ${OGG_LIBRARY}
    tinyxml++_orxonox
    tolua++_orxonox
    bullet_orxonox
    util
    core
    network
    tools
  SOURCE_FILES ${ORXONOX_SRC_FILES}
)
