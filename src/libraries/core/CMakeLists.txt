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

SET_SOURCE_FILES(CORE_SRC_FILES

#BUILD_UNIT CoreStableBuildUnit.cc
  ClassTreeMask.cc
  DynLib.cc
  DynLibManager.cc
  Event.cc
  Game.cc
  GameMode.cc
  GameState.cc
  Language.cc
  Loader.cc
  LuaState.cc
  Namespace.cc
  NamespaceNode.cc
  Template.cc
  ViewportEventListener.cc
  WindowEventListener.cc
  XMLNameListener.cc
  XMLPort.cc
#END_BUILD_UNIT

  BaseObject.cc
  Core.cc

BUILD_UNIT OgreBuildUnit.cc
  GraphicsManager.cc
  GUIManager.cc
  Resource.cc
END_BUILD_UNIT

BUILD_UNIT FilesystemBuildUnit.cc
  command/ArgumentCompletionFunctions.cc
  config/ConfigFile.cc
  PathConfig.cc
END_BUILD_UNIT

BUILD_UNIT ThreadBuildUnit.cc
  command/TclThreadManager.cc
  Thread.cc
  ThreadPool.cc
END_BUILD_UNIT
)

ADD_SUBDIRECTORY(class)
ADD_SUBDIRECTORY(command)
ADD_SUBDIRECTORY(config)
ADD_SUBDIRECTORY(input)
ADD_SUBDIRECTORY(object)

ORXONOX_ADD_LIBRARY(core
  FIND_HEADER_FILES
  TOLUA_FILES
    command/CommandExecutor.h
    config/SettingsConfigFile.h
    Game.h
    GameMode.h
    GraphicsManager.h
    GUIManager.h
    Loader.h
    LuaState.h
    PathConfig.h
    input/InputManager.h
    input/KeyBinder.h
    input/KeyBinderManager.h
    ../util/output/OutputDefinitions.h
  PCH_FILE
    CorePrecompiledHeaders.h
  LINK_LIBRARIES
    ${OGRE_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY} # Filesystem dependency
    ${Boost_THREAD_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY} # Thread dependency
    ${CEGUI_LIBRARY}
    ${CEGUILUA_LIBRARY}
    ${CEGUI_TOLUA_LIBRARY}
    ${CEGUI_OGRE_RENDERER_LIBRARY}
    ${LUA5.1_LIBRARY}
    cpptcl_orxonox
    ois_orxonox
    tinyxml_orxonox
    util
  LINK_LIBS_UNIX
    dl
    pthread
  SOURCE_FILES
    ${CORE_SRC_FILES}
)
