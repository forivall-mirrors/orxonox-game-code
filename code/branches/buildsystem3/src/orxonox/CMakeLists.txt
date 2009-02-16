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
  GraphicsEngine.cc
  LevelManager.cc
  Main.cc
  PawnManager.cc
  PlayerManager.cc
  Settings.cc
)
ADD_SUBDIRECTORY(gamestates)
ADD_SUBDIRECTORY(gui)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(overlays)
ADD_SUBDIRECTORY(tools)
GET_ALL_HEADER_FILES(ORXONOX_HDR_FILES)
SET(ORXONOX_FILES ${ORXONOX_SRC_FILES} ${ORXONOX_HDR_FILES})

GENERATE_SOURCE_GROUPS(${ORXONOX_FILES})
GENERATE_TOLUA_BINDINGS(Orxonox ORXONOX_FILES INPUTFILES gui/GUIManager.h)

# Not using precompiled header files: Avoid dependencies
INCLUDE_DIRECTORIES(pch/nopch)

IF(GCC_NO_SYSTEM_HEADER_SUPPORT)
  # Get around displaying a few hundred lines of warning code
  SET_SOURCE_FILES_PROPERTIES(gamestates/GSGraphics.cc PROPERTIES COMPILE_FLAGS "-w")
ENDIF()

ADD_EXECUTABLE(orxonox ${ORXONOX_FILES})
GET_TARGET_PROPERTY(_exec_loc orxonox LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE INTERNAL "")

IF(NETWORKTRAFFIC_TESTING_ENABLED)
  SET(ORXONOXS_SRC_FILES
    GraphicsEngine.cc
    objects/Camera.cc
    CameraManager.cc
  )
  ADD_LIBRARY(orxonoxs SHARED ${ORXONOXS_SRC_FILES})
ENDIF(NETWORKTRAFFIC_TESTING_ENABLED)

TARGET_LINK_LIBRARIES(orxonox
  ${OGRE_LIBRARY}
  ${CEGUI_LIBRARY}
  ${LUA_LIBRARIES}
  ${CEGUILUA_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ogreceguirenderer_orxonox
  tinyxml++_orxonox
  tolua++_orxonox
  bullet_orxonox
  util
  core
  network
  #audio
)

ORXONOX_INSTALL(orxonox)


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
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox.vcproj.user" "${CMAKE_CURRENT_BINARY_DIR}/orxonox.vcproj.user")
ENDIF(MSVC)
