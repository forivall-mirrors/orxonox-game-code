ADD_SOURCE_FILES(OIS_FILES
  Win32ForceFeedback.h
  Win32InputManager.h
  Win32JoyStick.h
  Win32KeyBoard.h
  Win32Mouse.h
  Win32Prereqs.h

BUILD_UNIT OISWin32BuildUnit.cpp
  Win32ForceFeedback.cpp
  Win32InputManager.cpp
  Win32JoyStick.cpp
  Win32KeyBoard.cpp
  Win32Mouse.cpp
END_BUILD_UNIT
)
