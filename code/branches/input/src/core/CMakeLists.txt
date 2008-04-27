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
#tolua/tolua_bind.h
)

#SET_SOURCE_FILES_PROPERTIES(tolua/tolua_bind.h
#  PROPERTIES
#  OBJECT_DEPENDS tolua/tolua_bind.h
#  OBJECT_DEPENDS tolua/tolua_bind.cc
#  GENERATED true
#  HEADER_FILE_ONLY true
#)

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
  ois
  util
)
