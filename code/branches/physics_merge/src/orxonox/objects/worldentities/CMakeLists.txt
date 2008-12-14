SET( SRC_FILES
  WorldEntity.cc
  StaticEntity.cc
  MovableEntity.cc
  MobileEntity.cc
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
  Planet.cc
#  Backlight.cc
)

ADD_SOURCE_DIRECTORY(SRC_FILES pawns)
ADD_SOURCE_DIRECTORY(SRC_FILES triggers)

ADD_SOURCE_FILES(SRC_FILES)
