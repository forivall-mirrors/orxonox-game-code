ADD_SOURCE_FILES(ORXONOX_SRC_FILES
  WorldEntity.cc
  PositionableEntity.cc
  MovableEntity.cc
  ControllableEntity.cc
  Model.cc
  Billboard.cc
  BlinkingBillboard.cc
  Light.cc
  Camera.cc
  CameraPosition.cc
  SpawnPoint.cc
  ParticleEmitter.cc
  ParticleSpawner.cc
#  Backlight.cc
)

ADD_SUBDIRECTORY(pawns)
ADD_SUBDIRECTORY(triggers)
