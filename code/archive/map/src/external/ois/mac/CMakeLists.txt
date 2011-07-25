ADD_SOURCE_FILES(OIS_FILES
  MacHelpers.h
  MacHIDManager.h
  MacInputManager.h
  MacKeyboard.h
  MacMouse.h
  MacPrereqs.h

COMPILATION_BEGIN OISMacCompilation.cpp
  MacHelpers.cpp
  MacHIDManager.cpp
  MacInputManager.cpp
  MacKeyboard.cpp
  MacMouse.cpp
COMPILATION_END
)
