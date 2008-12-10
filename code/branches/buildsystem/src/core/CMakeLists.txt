SET(CORE_SRC_FILES
  Clock.cc
  ConfigFileManager.cc
  ConfigValueContainer.cc
  Core.cc
  GameState.cc
  Language.cc
  ObjectListBase.cc
  OrxonoxClass.cc
  RootGameState.cc
  Script.cc

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
  XMLPort.cc

  # shell
  IRC.cc
  Shell.cc
  TclBind.cc
  TclThreadManager.cc

  # input
  input/Button.cc
  input/CalibratorCallback.cc
  input/ExtendedInputState.cc
  input/HalfAxis.cc
  input/InputBuffer.cc
  input/InputCommands.cc
  input/InputManager.cc
  input/KeyBinder.cc
  input/KeyDetector.cc
  input/SimpleInputState.cc
)

INCLUDE(UseTolua)
TOLUA(Core CORE_SRC_FILES INPUTFILES Script.h CommandExecutor.h)

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
