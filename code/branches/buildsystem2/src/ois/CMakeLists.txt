SET_SOURCE_FILES(OIS_FILES
  OIS.h
  OISConfig.h
  OISEffect.h
  OISEvents.h
  OISException.h
  OISFactoryCreator.h
  OISForceFeedback.h
  OISInputManager.h
  OISInterface.h
  OISJoyStick.h
  OISKeyboard.h
  OISMouse.h
  OISObject.h
  OISPrereqs.h

  OISEffect.cpp
  OISException.cpp
  OISForceFeedback.cpp
  OISInputManager.cpp
  OISJoyStick.cpp
  OISKeyboard.cpp
  OISObject.cpp
)
IF(WIN32)
  ADD_SUBDIRECTORY(win32)
ELSEIF(APPLE)
  ADD_SUBDIRECTORY(mac)
ELSEIF(UNIX)
  ADD_SUBDIRECTORY(linux)
ENDIF (WIN32)

GENERATE_SOURCE_GROUPS(${OIS_FILES})

REMOVE_COMPILER_FLAGS("-DUNICODE -D_UNICODE" MSVC)
INCLUDE_DIRECTORIES(.)

ADD_LIBRARY(ois_orxonox SHARED ${OIS_FILES} )
SET_TARGET_PROPERTIES(ois_orxonox PROPERTIES DEFINE_SYMBOL "OIS_NONCLIENT_BUILD")

IF(WIN32)
  TARGET_LINK_LIBRARIES( ois_orxonox ${DIRECTX_LIBRARIES} )
ENDIF(WIN32)

