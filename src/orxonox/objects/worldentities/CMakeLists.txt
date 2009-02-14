SET( SRC_FILES
  WorldEntity.cc
  StaticEntity.cc
  MovableEntity.cc
  MobileEntity.cc
  ControllableEntity.cc

  Backlight.cc
  Billboard.cc
  BlinkingBillboard.cc
  ExplosionChunk.cc
  FadingBillboard.cc
  Light.cc
  Camera.cc
  CameraPosition.cc
  Model.cc
  ParticleEmitter.cc
  ParticleSpawner.cc
  Planet.cc
  SpawnPoint.cc
)

ADD_SOURCE_DIRECTORY(SRC_FILES pawns)
ADD_SOURCE_DIRECTORY(SRC_FILES triggers)

ADD_SOURCE_FILES(SRC_FILES)
