PROJECT(Orxonox)

SET( CORE_SRC_FILES
  Factory.cc
  IdentifierList.cc
  Identifier.cc
  MetaObjectList.cc
  OrxonoxClass.cc
  Error.cc
)

ADD_LIBRARY(core SHARED ${CORE_SRC_FILES})
