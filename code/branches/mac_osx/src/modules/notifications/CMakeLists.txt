SET_SOURCE_FILES(NOTIFICATIONS_SRC_FILES
  Notification.cc
  NotificationDispatcher.cc
  NotificationManager.cc
  NotificationQueue.cc
)

ADD_SUBDIRECTORY(dispatchers)

ORXONOX_ADD_LIBRARY(notifications
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
    NotificationManager.h
    NotificationQueue.h
  PCH_FILE
    NotificationsPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${NOTIFICATIONS_SRC_FILES}
)
