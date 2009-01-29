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

ADD_LIBRARY(core SHARED ${CORE_FILES})

SET_TARGET_PROPERTIES(core PROPERTIES DEFINE_SYMBOL "CORE_SHARED_BUILD")
TARGET_LINK_LIBRARIES(core
  ${OGRE_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${LUA_LIBRARIES}
  cpptcl_orxonox
  ois_orxonox
  tinyxml++_orxonox
  tolua++_orxonox
  util
)

IF (NOT WIN32)
  INSTALL(TARGETS core LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
