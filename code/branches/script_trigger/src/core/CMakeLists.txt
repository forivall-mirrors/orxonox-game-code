SET(CORE_SRC_FILES
  BaseObject.cc
  ClassTreeMask.cc
  CommandExecutor.cc
  ConfigFileManager.cc
  ConfigValueContainer.cc
  CoreSettings.cc
  Error.cc
  Executor.cc
  Factory.cc
  Identifier.cc
  IdentifierDistributor.cc
  InputBuffer.cc
  InputHandler.cc
  InputManager.cc
  Language.cc
  Loader.cc
  MetaObjectList.cc
  Namespace.cc
  NamespaceNode.cc
  OrxonoxClass.cc
  OutputHandler.cc
  LuaBind.cc
  SignalHandler.cc
  TclBind.cc
  Tickable.cc
  XMLPort.cc
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
  tinyxml
  tolualib
  ois
  util
)
