#get the created files
AUX_SOURCE_DIRECTORY(tolua TOLUA_BIND_FILES) 

SET( CORE_SRC_FILES
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
  ${TOLUA_BIND_FILES}
)

ADD_LIBRARY( core SHARED ${CORE_SRC_FILES})

TARGET_LINK_LIBRARIES( core
  util
  ${Lua_LIBRARIES}
  ${Lua_LIBRARY}
  ${OIS_LIBRARIES}
)
