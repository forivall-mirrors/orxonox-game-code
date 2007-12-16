PROJECT(Orxonox)

SET( OBJECTS_SRC_FILES
  BaseObject.cc
  WorldEntity.cc
  test1.cc
  test2.cc
  test3.cc
  Ambient.cc
  Skybox.cc
  SceneNode.cc
  Entity.cc
  Camera.cc
  SpaceshipSteeringObject.cc
)

ADD_LIBRARY(objects SHARED ${OBJECTS_SRC_FILES})

TARGET_LINK_LIBRARIES( objects
  xml
  orxonox
  ${WINDOWS_ENET_DEPENDENCIES}
)