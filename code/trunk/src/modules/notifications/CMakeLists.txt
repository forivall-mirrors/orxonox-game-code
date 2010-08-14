SET_SOURCE_FILES(NOTIFICATIONS_SRC_FILES
  Notification.cc
  NotificationManager.cc
  NotificationOverlay.cc
  NotificationQueue.cc
)

ORXONOX_ADD_LIBRARY(notifications
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
  PCH_FILE
    NotificationsPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
    overlays
  SOURCE_FILES ${NOTIFICATIONS_SRC_FILES}
)
