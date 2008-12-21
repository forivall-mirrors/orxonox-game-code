ADD_SOURCE_FILES(
  CameraManager.cc
  GraphicsEngine.cc
  LevelManager.cc
  Main.cc
  PlayerManager.cc
  Settings.cc
)

ADD_SOURCE_DIRECTORY(gamestates)
ADD_SOURCE_DIRECTORY(gui)
ADD_SOURCE_DIRECTORY(objects)
ADD_SOURCE_DIRECTORY(overlays)
ADD_SOURCE_DIRECTORY(tools)

WRITE_SOURCE_FILES(ORXONOX_SRC_FILES)

TOLUA(Orxonox ORXONOX_SRC_FILES INPUTFILES gui/GUIManager.h)

ADD_EXECUTABLE(orxonox ${ORXONOX_SRC_FILES})

IF(NETWORKTRAFFIC_TESTING_ENABLED)

  SET( ORXONOXS_SRC_FILES
    GraphicsEngine.cc
    objects/Camera.cc
    CameraManager.cc
  )

  ADD_LIBRARY(orxonoxs SHARED ${ORXONOX_SRC_FILES})
ENDIF(NETWORKTRAFFIC_TESTING_ENABLED)

TARGET_LINK_LIBRARIES( orxonox
  ${OGRE_LIBRARY}
  ${CEGUI_LIBRARY}
  ${LUA_LIBRARIES}
  #${CEGUI_SCRIPT_LIBRARIES}
  ceguilua_orxonox
  tinyxml_orxonox
  tolualib_orxonox
  util
  core
  audio
  network
)

INSTALL(TARGETS orxonox RUNTIME DESTINATION bin)
