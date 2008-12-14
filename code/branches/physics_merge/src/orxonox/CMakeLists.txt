SET( ORXONOX_SRC_FILES
  CameraManager.cc
  GraphicsEngine.cc
  LevelManager.cc
  Main.cc
  PlayerManager.cc
  Settings.cc
  
  tolua/tolua_bind.cc
)

ADD_SOURCE_DIRECTORY(ORXONOX_SRC_FILES gamestates)
ADD_SOURCE_DIRECTORY(ORXONOX_SRC_FILES gui)
ADD_SOURCE_DIRECTORY(ORXONOX_SRC_FILES objects)
ADD_SOURCE_DIRECTORY(ORXONOX_SRC_FILES overlays)
ADD_SOURCE_DIRECTORY(ORXONOX_SRC_FILES tools)

GET_TARGET_PROPERTY(TOLUA_EXE tolua_orxonox LOCATION)
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tolua/tolua_bind.cc ${CMAKE_CURRENT_SOURCE_DIR}/tolua/tolua_bind.h
  COMMAND ${TOLUA_EXE} -n Orxonox -o ../../src/orxonox/tolua/tolua_bind.cc -H ../../src/orxonox/tolua/tolua_bind.h ../../src/orxonox/tolua/tolua.pkg
  DEPENDS
    tolua_orxonox
    tolua/tolua.pkg
    gui/GUIManager.h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib
)


ADD_EXECUTABLE( orxonox ${ORXONOX_SRC_FILES} )

IF(NETWORKTRAFFIC_TESTING_ENABLED)

  SET( ORXONOXS_SRC_FILES
    GraphicsEngine.cc
    objects/Camera.cc
    CameraManager.cc
  )

  ADD_LIBRARY(orxonoxs SHARED ${ORXONOX_SRC_FILES})
ENDIF(NETWORKTRAFFIC_TESTING_ENABLED)

TARGET_LINK_LIBRARIES( orxonox
  ${OGRE_LIBRARIES}
  ${CEGUI_LIBRARIES}
  lua_orxonox
  ceguilua_orxonox
  tinyxml_orxonox
  tolualib_orxonox
  LibBulletDynamics
  LibBulletCollision
  LibLinearMath
  util
  core
  audio
  network
)

