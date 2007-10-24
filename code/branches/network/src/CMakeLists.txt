PROJECT(Orxonox)

# create a few variables to simplify life
SET(SRC_FILES orxonox.cc)
SET(INC_FILES ExampleApplication.h  ExampleFrameListener.h)
include_directories( "./tnl" )

#Creates an executable
ADD_EXECUTABLE(../bin/main ${SRC_FILES} ${INC_FILES})
#Links the executable against OGRE and OIS
#TARGET_LINK_LIBRARIES(../bin/main ${OGRE_LIBRARIES} ${OIS_LIBRARIES} /home/scheusso/orxonox/network/src/raklib/libraknet.so.2.4.5.2)
