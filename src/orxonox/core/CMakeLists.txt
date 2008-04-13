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
  ConfigValueContainer.cc
  Error.cc
  SignalHandler.cc
  DebugLevel.cc
  OutputHandler.cc
  Language.cc
  ClassTreeMask.cc
  Loader.cc
  Executor.cc
  XMLPort.cc
  Tickable.cc
  Script.cc
)

ADD_LIBRARY( core SHARED ${CORE_SRC_FILES})
SET_TARGET_PROPERTIES( core PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( core
  util
  ${Lua_LIBRARIES}
  ${OIS_LIBRARIES}
)
