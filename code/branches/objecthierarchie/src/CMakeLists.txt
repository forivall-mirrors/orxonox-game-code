PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES orxonox.cc IdentifierList.cc Identifier.cc Factory.cc OrxonoxClass.cc BaseObject.cc test1.cc test2.cc test3.cc)
SET(INC_FILES IdentifierIncludes.h Identifier.h Factory.h IdentifierList.h ObjectList.h Iterator.h OrxonoxClass.h BaseObject.h Test.h test1.h test2.h test3.h)

#Creates an executable
ADD_EXECUTABLE(../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_OGRE_LIBRARIES} ${CEGUI_LIBRARIES})
