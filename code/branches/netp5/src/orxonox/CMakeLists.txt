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
  CameraManager.cc
  GraphicsManager.cc
  LevelManager.cc
  Main.cc
  PawnManager.cc
  PlayerManager.cc
)
ADD_SUBDIRECTORY(gamestates)
ADD_SUBDIRECTORY(gui)
ADD_SUBDIRECTORY(interfaces)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(overlays)
ADD_SUBDIRECTORY(sound)
ADD_SUBDIRECTORY(tools)

# Translate argument
IF(ORXONOX_USE_WINMAIN)
  SET(ORXONOX_WIN32 WIN32)
ENDIF()

ORXONOX_ADD_EXECUTABLE(orxonox
  FIND_HEADER_FILES
  TOLUA_FILES
    gui/GUIManager.h
    objects/pickup/BaseItem.h
    objects/pickup/PickupInventory.h
    objects/quest/QuestDescription.h
    objects/quest/QuestManager.h
  PCH_FILE
    OrxonoxPrecompiledHeaders.h
  PCH_NO_DEFAULT
  # When defined as WIN32 this removes the console window on Windows
  ${ORXONOX_WIN32}
  LINK_LIBRARIES
    ${OGRE_LIBRARY}
    ${CEGUI_LIBRARY}
    ${LUA_LIBRARIES}
    ${CEGUILUA_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${OPENAL_LIBRARY}
    ${ALUT_LIBRARY}
    ${VORBISFILE_LIBRARY}
    ${VORBIS_LIBRARY}
    ${OGG_LIBRARY}
    ogreceguirenderer_orxonox
    tinyxml++_orxonox
    tolua++_orxonox
    bullet_orxonox
    util
    core
    network
  SOURCE_FILES ${ORXONOX_SRC_FILES}
)

GET_TARGET_PROPERTY(_exec_loc orxonox LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE INTERNAL "")


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
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox.vcproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/orxonox.vcproj.user")
ENDIF(MSVC)
