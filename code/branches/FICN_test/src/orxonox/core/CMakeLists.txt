SET( CORE_SRC_FILES
  BaseObject.cc
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

ADD_LIBRARY(${Orxonox_BINARY_DIR}/core SHARED ${CORE_SRC_FILES})
SET_TARGET_PROPERTIES( ${Orxonox_BINARY_DIR}/core PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( ${Orxonox_BINARY_DIR}/core
  ${Orxonox_BINARY_DIR}/util
)