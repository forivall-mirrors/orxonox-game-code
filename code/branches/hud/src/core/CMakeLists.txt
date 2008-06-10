SET(CORE_SRC_FILES
  ConfigFileManager.cc
  ConfigValueContainer.cc
  Core.cc
  Error.cc
  Language.cc
  OrxonoxClass.cc
  OutputBuffer.cc
  OutputHandler.cc
  Script.cc
  SignalHandler.cc

  # command
  ArgumentCompletionFunctions.cc
  CommandEvaluation.cc
  CommandExecutor.cc
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
  input/HalfAxis.cc
  input/InputBuffer.cc
  input/InputCommands.cc
  input/InputManager.cc
  input/KeyBinder.cc
  input/KeyDetector.cc

  tolua/tolua_bind.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT tolua/tolua_bind.cc tolua/tolua_bind.h
  COMMAND ${TOLUA_EXE} -n core -o ../../src/core/tolua/tolua_bind.cc -H ../../src/core/tolua/tolua_bind.h ../../src/core/tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)

ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})

TARGET_LINK_LIBRARIES(core
  ${Lua_LIBRARIES}
  ${OGRE_LIBRARIES}
  cpptcl
  ois_orxonox
  tinyxml
  tolualib
  util
  ${Boost_thread_LIBRARIES}
  ${Boost_filesystem_LIBRARIES}
)
