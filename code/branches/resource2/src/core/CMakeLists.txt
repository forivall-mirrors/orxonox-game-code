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
  Clock.cc
  ConfigFileManager.cc
  ConfigValueContainer.cc
  Core.cc
  Event.cc
  Game.cc
  GameMode.cc
  GameState.cc
  GraphicsManager.cc
  GUIManager.cc
  Language.cc
  LuaBind.cc
  ObjectListBase.cc
  OrxonoxClass.cc
  WindowEventListener.cc

  # command
  ArgumentCompletionFunctions.cc
  CommandEvaluation.cc
  CommandExecutor.cc
  CommandLine.cc
  ConsoleCommand.cc
  ConsoleCommandCompilation.cc
  Executor.cc

  # hierarchy
  Factory.cc
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

  # shell
  IRC.cc
  Shell.cc
  TclBind.cc
  TclThreadManager.cc
  
  # multithreading
  Thread.cc
  ThreadPool.cc
)
ADD_SUBDIRECTORY(input)

ORXONOX_ADD_LIBRARY(core
  FIND_HEADER_FILES
  TOLUA_FILES
    CommandExecutor.h
    LuaBind.h
  DEFINE_SYMBOL
    "CORE_SHARED_BUILD"
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
    tinyxml++_orxonox
    tolua++_orxonox
    util
  SOURCE_FILES
    ${CORE_SRC_FILES}
)
