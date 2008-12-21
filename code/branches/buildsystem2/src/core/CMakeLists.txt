ADD_SOURCE_FILES(
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

ADD_SOURCE_DIRECTORY(input)

WRITE_SOURCE_FILES(CORE_SRC_FILES)

INCLUDE(UseTolua)
TOLUA(Core CORE_SRC_FILES INPUTFILES LuaBind.h CommandExecutor.h)

ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})

TARGET_LINK_LIBRARIES(core
  ${OGRE_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${LUA_LIBRARIES}
  cpptcl_orxonox
  ois_orxonox
  tinyxml_orxonox
  tolualib_orxonox
  util
)

INSTALL(TARGETS core LIBRARY DESTINATION lib)
