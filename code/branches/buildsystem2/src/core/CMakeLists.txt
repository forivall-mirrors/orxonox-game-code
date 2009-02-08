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
  GameState.cc
  Language.cc
  LuaBind.cc
  ObjectListBase.cc
  OrxonoxClass.cc
  RootGameState.cc

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
)
ADD_SUBDIRECTORY(input)
GET_ALL_HEADER_FILES(CORE_HDR_FILES)
SET(CORE_FILES ${CORE_SRC_FILES} ${CORE_HDR_FILES})

GENERATE_SOURCE_GROUPS(${CORE_FILES})
GENERATE_TOLUA_BINDINGS(Core CORE_FILES INPUTFILES LuaBind.h CommandExecutor.h)

IF(GCC_NO_SYSTEM_HEADER_SUPPORT)
  # Get around displaying a few hundred lines of warning code
  SET_SOURCE_FILES_PROPERTIES(ArgumentCompletionFunctions.cc PROPERTIES COMPILE_FLAGS "-w")
ENDIF()

ADD_LIBRARY(core SHARED ${CORE_FILES})

SET_TARGET_PROPERTIES(core PROPERTIES DEFINE_SYMBOL "CORE_SHARED_BUILD")
TARGET_LINK_LIBRARIES(core
  ${OGRE_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}    # MSVC only
  ${Boost_DATE_TIME_LIBRARY} # MSVC only
  ${LUA_LIBRARIES}
  cpptcl_orxonox
  ois_orxonox
  tinyxml++_orxonox
  tolua++_orxonox
  util
)

ORXONOX_INSTALL(core)
