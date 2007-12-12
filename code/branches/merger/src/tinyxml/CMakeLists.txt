PROJECT(Orxonox)

INCLUDE_DIRECTORIES(.)

SET (SRC_FILES
tinystr.cc
tinyxml.cc
tinyxmlerror.cc
tinyxmlparser
)

ADD_LIBRARY(xml SHARED ${SRC_FILES})
