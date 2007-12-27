PROJECT(Orxonox)

SET( CORE_SRC_FILES
  Factory.cc
  IdentifierList.cc
  Identifier.cc
  MetaObjectList.cc
  OrxonoxClass.cc
  ConfigValueContainer.cc
  Error.cc
  SignalHandler.cc
  ArgReader.cc
  DebugLevel.cc
  OutputHandler.cc
  Language.cc
)

IF(WIN32)
  ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})
ELSE(WIN32)
  ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})
ENDIF(WIN32)
