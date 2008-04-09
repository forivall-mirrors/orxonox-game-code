SET( CORE_SRC_FILES
  OrxonoxClass.cc
  BaseObject.cc
  Factory.cc
  Identifier.cc
  IdentifierDistributor.cc
  MetaObjectList.cc
  ConfigFileManager.cc
  ConfigValueContainer.cc
  Error.cc
  SignalHandler.cc
  ArgReader.cc
  DebugLevel.cc
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
)

ADD_LIBRARY( core SHARED ${CORE_SRC_FILES})
SET_TARGET_PROPERTIES( core PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( core
  util
  ${OIS_LIBRARIES}
)
