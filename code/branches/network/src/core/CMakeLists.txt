SET(CORE_SRC_FILES
  BaseObject.cc
  ClassTreeMask.cc
  ConfigFileManager.cc
  ConfigValueContainer.cc
  CoreSettings.cc
  Error.cc
  Executor.cc
  Factory.cc
  Identifier.cc
  IdentifierDistributor.cc
  InputManager.cc
  KeyBinder.cc
  OutputBuffer.cc
  InputBuffer.cc
  Shell.cc
  CommandExecutor.cc
  CommandEvaluation.cc
  ConsoleCommand.cc
  ArgumentCompletionFunctions.cc
  ConsoleCommandCompilation.cc
  Language.cc
  Loader.cc
  MetaObjectList.cc
  Namespace.cc
  NamespaceNode.cc
  OrxonoxClass.cc
  OutputHandler.cc
  Script.cc
  SignalHandler.cc
  TclBind.cc
  Tickable.cc
  XMLPort.cc
  TclThreadManager.cc
  IRC.cc
  tolua/tolua_bind.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT tolua/tolua_bind.cc tolua/tolua_bind.h
  COMMAND ${TOLUA_EXE} -n core -o ../../src/core/tolua/tolua_bind.cc -H ../../src/core/tolua/tolua_bind.h ../../src/core/tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)

IF(WIN32)
  ADD_LIBRARY(core ${CORE_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(core
  ${Lua_LIBRARIES}
  ${OGRE_LIBRARIES}
  cpptcl
  tinyxml
  tolualib
  ois
  util
  ${Boost_thread_LIBRARIES}
  ${Boost_filesystem_LIBRARIES}
)
