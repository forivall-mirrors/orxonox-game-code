PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES
	orxonox/orxonox.cc
	loader/LevelLoader.cc
	xml/xmlParser.cc
	orxonox/core/IdentifierList.cc
	orxonox/core/Identifier.cc
	orxonox/core/MetaObjectList.cc
	orxonox/core/Factory.cc
	orxonox/core/OrxonoxClass.cc
	orxonox/core/ConfigValueContainer.cc
	orxonox/core/SignalHandler.cc
	orxonox/objects/BaseObject.cc
	orxonox/objects/WorldEntity.cc
	orxonox/objects/test1.cc
	orxonox/objects/test2.cc
	orxonox/objects/test3.cc
)

SET(INC_FILES
	loader/LevelLoader.h
	xml/xmlParser.h
	orxonox/core/CoreIncludes.h
	orxonox/core/Debug.h
	orxonox/core/Identifier.h
	orxonox/core/Factory.h
	orxonox/core/ClassFactory.h
	orxonox/core/IdentifierList.h
	orxonox/core/ObjectList.h
	orxonox/core/MetaObjectList.h
	orxonox/core/Iterator.h
	orxonox/core/OrxonoxClass.h
	orxonox/core/ConfigValueContainer.h
	orxonox/core/SignalHandler.h
	orxonox/objects/BaseObject.h
	orxonox/objects/WorldEntity.h
	orxonox/objects/Test.h
	orxonox/objects/test1.h
	orxonox/objects/test2.h
	orxonox/objects/test3.h
	orxonox/objects/Tickable.h
	orxonox/objects/Timer.h
)

#Creates an executable
ADD_EXECUTABLE(../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES})

