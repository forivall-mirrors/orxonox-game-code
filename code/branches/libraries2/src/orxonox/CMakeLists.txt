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

SET_SOURCE_FILES(ORXONOX_SRC_FILES
  Main.cc

  CameraManager.cc
  LevelManager.cc
  PawnManager.cc
  PlayerManager.cc
)
ADD_SUBDIRECTORY(interfaces)
ADD_SUBDIRECTORY(overlays)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(sound)

ORXONOX_ADD_LIBRARY(orxonox
  FIND_HEADER_FILES
  TOLUA_FILES
    LevelManager.h
    objects/pickup/BaseItem.h
    objects/pickup/PickupInventory.h
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


# When using Visual Studio we want to use the output directory as working
# directory and we also want to specify where the external dlls
# (lua, ogre, etc.) are. The problem hereby is that these information cannot
# be specified in CMake because they are not stored in the actual project file.
# This workaround will create a configured *.vcproj.user file that holds the
# right values. When starting the solution for the first time,
# these get written to the *vcproj.yourPCname.yourname.user
IF(MSVC)
  IF(CMAKE_CL_64)
    SET(MSVC_PLATFORM "x64")
  ELSE()
    SET(MSVC_PLATFORM "Win32")
  ENDIF()
  STRING(REGEX REPLACE "^Visual Studio ([0-9][0-9]?) .*$" "\\1"
         VISUAL_STUDIO_VERSION_SIMPLE "${CMAKE_GENERATOR}")
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox-main.vcproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/orxonox-main.vcproj.user")
ENDIF(MSVC)
