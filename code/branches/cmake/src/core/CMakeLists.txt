SET(CORE_SRC_FILES
  OrxonoxClass.cc
  BaseObject.cc
  Factory.cc
  Identifier.cc
  IdentifierDistributor.cc
  InputHandler.cc
  InputManager.cc
  InputEventListener.cc
  MetaObjectList.cc
  ConfigFileManager.cc
  ConfigValueContainer.cc
  Error.cc
  SignalHandler.cc
  CoreSettings.cc
  OutputHandler.cc
  Language.cc
  ClassTreeMask.cc
  Loader.cc
  Executor.cc
  XMLPort.cc
  Namespace.cc
  NamespaceNode.cc
  CommandExecutor.cc
  InputBuffer.cc
  Tickable.cc
  Script.cc
  tolua/tolua_bind.cc
)

GET_TARGET_PROPERTY(TOLUA_EXE tolua LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT tolua/tolua_bind.cc
  COMMAND ${TOLUA_EXE} -n core -o core/tolua/tolua_bind.cc -H core/tolua/tolua_bind.h core/tolua/tolua.pkg
  DEPENDS tolua
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
)

ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})

TARGET_LINK_LIBRARIES(core
  ${Lua_LIBRARIES}
  ${OIS_LIBRARIES}
  ${OGRE_LIBRARIES}
  util
)
