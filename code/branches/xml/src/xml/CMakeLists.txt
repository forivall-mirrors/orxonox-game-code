PROJECT(Orxonox)

SET(SRC_FILES xmlParser.cc)
SET(INC_FILES xmlParser.h)

add_library(xml ${SRC_FILES} ${INC_FILES})