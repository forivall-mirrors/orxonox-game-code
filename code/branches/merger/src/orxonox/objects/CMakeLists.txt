PROJECT(Orxonox)

SET(SRC_FILES BaseObject.cc test1.cc test2.cc test3.cc)
SET(INC_FILES BaseObject.h Test.h test1.h test2.h test3.h)

ADD_LIBRARY(core ${SRC_FILES} ${INC_FILES})