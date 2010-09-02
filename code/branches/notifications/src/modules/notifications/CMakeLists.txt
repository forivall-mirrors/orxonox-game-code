SET_SOURCE_FILES(NOTIFICATIONS_SRC_FILES
  Notification.cc
  NotificationDispatcher.cc
  NotificationManager.cc
  NotificationOverlay.cc
  NotificationQueue.cc
)

ADD_SUBDIRECTORY(dispatchers)

ORXONOX_ADD_LIBRARY(notifications
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
  PCH_FILE
    NotificationsPrecompiledHeaders.h
  LINK_LIBRARIES
    objects
    orxonox
    overlays
  SOURCE_FILES ${NOTIFICATIONS_SRC_FILES}
)
