SET_SOURCE_FILES(PORTALS_SRC_FILES
  PortalEndPoint.cc
  PortalLink.cc
)

ORXONOX_ADD_LIBRARY(portals
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
  PCH_FILE
    PortalsPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
    objects
  SOURCE_FILES ${PORTALS_SRC_FILES}
)
