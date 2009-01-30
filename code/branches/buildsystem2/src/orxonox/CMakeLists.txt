SET_SOURCE_FILES(ORXONOX_SRC_FILES
  CameraManager.cc
  GraphicsEngine.cc
  LevelManager.cc
  Main.cc
  PlayerManager.cc
  Settings.cc
)
ADD_SUBDIRECTORY(gamestates)
ADD_SUBDIRECTORY(gui)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(overlays)
ADD_SUBDIRECTORY(tools)
GET_ALL_HEADER_FILES(ORXONOX_HDR_FILES)
SET(ORXONOX_FILES ${ORXONOX_SRC_FILES} ${ORXONOX_HDR_FILES})

GENERATE_SOURCE_GROUPS(${ORXONOX_FILES})
GENERATE_TOLUA_BINDINGS(Orxonox ORXONOX_FILES INPUTFILES gui/GUIManager.h)

# Not using precompiled header files: Avoid dependencies
INCLUDE_DIRECTORIES(pch/nopch)

ADD_EXECUTABLE(orxonox ${ORXONOX_FILES})
GET_TARGET_PROPERTY(_exec_loc orxonox LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE INTERNAL "")

IF(NETWORKTRAFFIC_TESTING_ENABLED)
  SET(ORXONOXS_SRC_FILES
    GraphicsEngine.cc
    objects/Camera.cc
    CameraManager.cc
  )
  ADD_LIBRARY(orxonoxs SHARED ${ORXONOXS_SRC_FILES})
ENDIF(NETWORKTRAFFIC_TESTING_ENABLED)

TARGET_LINK_LIBRARIES( orxonox
  ${OGRE_LIBRARY}
  ${CEGUI_LIBRARY}
  ${LUA_LIBRARIES}
  ${CEGUILUA_LIBRARY}
  ogreceguirenderer_orxonox
  tinyxml++_orxonox
  tolua++_orxonox
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
  ELSE()
    SET(MSVC_PLATFORM "Win32")
  ENDIF()
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox.vcproj.user" "${CMAKE_CURRENT_BINARY_DIR}/orxonox.vcproj.user")
ENDIF(MSVC)

IF(NOT WIN32)
  INSTALL(TARGETS orxonox RUNTIME DESTINATION bin)
ENDIF()
