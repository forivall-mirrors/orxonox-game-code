SET_SOURCE_FILES(PICKUP_SRC_FILES
  CollectiblePickup.cc
  DroppedPickup.cc
  Pickup.cc
  PickupCollection.cc
  PickupManager.cc
  PickupRepresentation.cc
  PickupSpawner.cc
)

ADD_SUBDIRECTORY(items)

ORXONOX_ADD_LIBRARY(pickup
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
    PickupManager.h
    PickupRepresentation.h
  PCH_FILE
    PickupPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${PICKUP_SRC_FILES}
)
