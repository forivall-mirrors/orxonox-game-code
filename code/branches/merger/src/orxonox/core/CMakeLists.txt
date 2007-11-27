PROJECT(Orxonox)

SET(SRC_FILES IdentifierList.cc Identifier.cc MetaObjectList.cc Factory.cc OrxonoxClass.cc)
SET(INC_FILES IdentifierIncludes.h Identifier.h Factory.h ClassFactory.h IdentifierList.h ObjectList.h MetaObjectList.h Iterator.h OrxonoxClass.h)

ADD_LIBRARY(core ${SRC_FILES} ${INC_FILES})