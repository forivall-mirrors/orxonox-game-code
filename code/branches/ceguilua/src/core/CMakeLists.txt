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

  tolua/tolua_bind.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua_orxonox LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tolua/tolua_bind.cc
  COMMAND ${TOLUA_EXE} -n Core -o ../../src/core/tolua/tolua_bind.cc -H ../../src/core/tolua/tolua_bind.h ../../src/core/tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)

ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})

TARGET_LINK_LIBRARIES(core
  ${OGRE_LIBRARIES}
  lua_orxonox
  cpptcl_orxonox
  ois_orxonox
  tinyxml_orxonox
  tolualib_orxonox
  util
  ${Boost_thread_LIBRARIES}
  ${Boost_filesystem_LIBRARIES}
)
