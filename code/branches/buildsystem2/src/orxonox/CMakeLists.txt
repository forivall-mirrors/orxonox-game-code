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

# Not using precompiled header files: Avoid dependencies
INCLUDE_DIRECTORIES(pch/nopch)

# Enlarge heap size for precompiled header files
ADD_CXX_FLAGS("-Zm200" MSVC)

ADD_EXECUTABLE(orxonox ${ORXONOX_SRC_FILES})
GET_TARGET_PROPERTY(_exec_loc orxonox LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE STRING "")
MARK_AS_ADVANCED(ORXONOX_EXECUTABLE_NAME)

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
  ogreceguirenderer_orxonox
  tinyxml_orxonox
  tolualib_orxonox
  util
  core
  #audio
  network
)

# When using Visual Studio we want to use the output directory as working
# directory and we also want to specify where the external dlls
# (lua, ogre, etc.) are. The problem hereby is that these information cannot
# be specified in CMake because they are not stored in the actual project file.
# This workaround will create a configured *.vcproj.user file that holds the
# right values. When starting the solution for the first time,
# these get written to the *vcproj.yourPCname.yourname.user
IF(MSVC)
  IF(CMAKE_CL_64)
    SET(MSVC_PLATFORM "x64")
  ELSE(CMAKE_CL_64)
    SET(MSVC_PLATFORM "Win32")
  ENDIF(CMAKE_CL_64)
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox.vcproj.user" "${CMAKE_CURRENT_BINARY_DIR}/orxonox.vcproj.user")
ENDIF(MSVC)

IF (NOT WIN32)
  INSTALL(TARGETS orxonox RUNTIME DESTINATION bin)
ENDIF (NOT WIN32)
