ADD_LIBRARY(cpptcl_orxonox SHARED CppTcl.cc)

TARGET_LINK_LIBRARIES(cpptcl_orxonox
  ${TCL_LIBRARY}
)

INSTALL(TARGETS cpptcl_orxonox LIBRARY DESTINATION lib)
