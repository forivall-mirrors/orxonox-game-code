PROJECT(Orxonox)

SET( OBJECTS_SRC_FILES
	BaseObject.cc
	test1.cc
	test2.cc
	test3.cc
)

ADD_LIBRARY(objects SHARED ${OBJECTS_SRC_FILES})
