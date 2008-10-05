SET( OIS_SRC_FILES
  OISEffect.cpp
  OISException.cpp
  OISForceFeedback.cpp
  OISInputManager.cpp
  OISJoyStick.cpp
  OISKeyboard.cpp
  OISObject.cpp
)

IF(WIN32)
  SET( OIS_SRC_FILES ${OIS_SRC_FILES}
    win32/Win32ForceFeedback.cpp
    win32/Win32InputManager.cpp
    win32/Win32JoyStick.cpp
    win32/Win32KeyBoard.cpp
    win32/Win32Mouse.cpp
  )
ELSEIF (APPLE)
  SET( OIS_SRC_FILES ${OIS_SRC_FILES}
    mac/MacHelpers.cpp
    mac/MacHIDManager.cpp
    mac/MacInputManager.cpp
    mac/MacKeyboard.cpp
    mac/MacMouse.cpp
  )
ELSEIF (UNIX)
  SET( OIS_SRC_FILES ${OIS_SRC_FILES}
    linux/EventHelpers.cpp
    linux/LinuxForceFeedback.cpp
    linux/LinuxInputManager.cpp
    linux/LinuxJoyStickEvents.cpp
    linux/LinuxKeyboard.cpp
    linux/LinuxMouse.cpp
  )
ENDIF (WIN32)

ADD_LIBRARY( ois_orxonox SHARED ${OIS_SRC_FILES} )

IF(WIN32)
  LINK_DIRECTORIES(${DirectX_LIB_DIR})
  TARGET_LINK_LIBRARIES( ois_orxonox
    dxguid
    dinput8
  )
ENDIF(WIN32)

