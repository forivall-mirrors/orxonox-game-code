SET_SOURCE_FILES(PICKUP_SRC_FILES
  DroppedItem.cc
  Pickup.cc
  PickupCollection.cc
  PickupCollectionIdentifier.cc
  PickupManager.cc
  PickupRepresentation.cc
  PickupSpawner.cc
)

ADD_SUBDIRECTORY(items)

ORXONOX_ADD_LIBRARY(pickup
  MODULE
  FIND_HEADER_FILES
  TOLUA_FILES
  DEFINE_SYMBOL
    "PICKUP_SHARED_BUILD"
  PCH_FILE
    PickupPrecompiledHeaders.h
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES ${PICKUP_SRC_FILES}
)
