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
  CommandLineParser.cc
  ConfigValueContainer.cc
  Core.cc
  DynLib.cc
  DynLibManager.cc
  Event.cc
  Game.cc
  GameMode.cc
  GameState.cc
  GraphicsManager.cc
  GUIManager.cc
  Language.cc
  LuaState.cc
  ObjectListBase.cc
  OrxonoxClass.cc
  Resource.cc
  WindowEventListener.cc

  # hierarchy
  Identifier.cc
  MetaObjectList.cc

  # level
  BaseObject.cc
  ClassTreeMask.cc
  Loader.cc
  Namespace.cc
  NamespaceNode.cc
  Template.cc
  XMLPort.cc
  XMLNameListener.cc

COMPILATION_BEGIN FilesystemCompilation.cc
  command/ArgumentCompletionFunctions.cc
  ConfigFileManager.cc
  MemoryArchive.cc
  PathConfig.cc
COMPILATION_END

  # multithreading
  ThreadPool.cc
COMPILATION_BEGIN ThreadCompilation.cc
  command/TclThreadManager.cc
  Thread.cc
COMPILATION_END
)

ADD_SUBDIRECTORY(command)
ADD_SUBDIRECTORY(input)

ORXONOX_ADD_LIBRARY(core
  FIND_HEADER_FILES
  TOLUA_FILES
    command/CommandExecutor.h
    ConfigFileManager.h
    Game.h
    GameMode.h
    GUIManager.h
    Loader.h
    LuaState.h
    PathConfig.h
    input/InputManager.h
    input/KeyBinder.h
    input/KeyBinderManager.h
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
    ${LUA_LIBRARIES}
    cpptcl_orxonox
    ogreceguirenderer_orxonox
    ois_orxonox
    tinyxml_orxonox
    tolua_orxonox
    util
  SOURCE_FILES
    ${CORE_SRC_FILES}
)