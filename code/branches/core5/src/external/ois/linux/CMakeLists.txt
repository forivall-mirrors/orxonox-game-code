ADD_SOURCE_FILES(OIS_FILES
  EventHelpers.h
  LinuxForceFeedback.h
  LinuxInputManager.h
  LinuxJoyStickEvents.h
  LinuxKeyboard.h
  LinuxMouse.h
  LinuxPrereqs.h

COMPILATION_BEGIN OISLinuxCompilation.cpp
  EventHelpers.cpp
  LinuxForceFeedback.cpp
  LinuxInputManager.cpp
  LinuxJoyStickEvents.cpp
  LinuxKeyboard.cpp
  LinuxMouse.cpp
COMPILATION_END
)
