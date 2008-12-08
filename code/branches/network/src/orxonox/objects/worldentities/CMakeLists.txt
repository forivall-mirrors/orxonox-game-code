SET( SRC_FILES
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

ADD_SOURCE_DIRECTORY(SRC_FILES pawns)
ADD_SOURCE_DIRECTORY(SRC_FILES triggers)

ADD_SOURCE_FILES(SRC_FILES)
