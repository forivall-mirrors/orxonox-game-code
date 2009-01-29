SET(CPPTCL_FILES
  cpptcl.h
  cpptcl.cc

  details/callbacks.h
  details/callbacks_v.h
  details/constructors.h
  details/conversions.h
  details/dispatchers.h
  details/metahelpers.h
  details/methods.h
  details/methods_v.h
)
GENERATE_SOURCE_GROUPS(${CPPTCL_FILES})

IF(MSVC)
  ADD_LIBRARY(cpptcl_orxonox STATIC ${CPPTCL_FILES})
ELSE(MSVC)
  ADD_LIBRARY(cpptcl_orxonox SHARED ${CPPTCL_FILES})
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(cpptcl_orxonox
  ${TCL_LIBRARY}
)

IF (NOT WIN32)
  INSTALL(TARGETS cpptcl_orxonox LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
