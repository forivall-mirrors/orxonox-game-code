ADD_SOURCE_FILES(ORXONOX_SRC_FILES
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
  TeamSpawnPoint.cc
)

ADD_SUBDIRECTORY(pawns)
ADD_SUBDIRECTORY(triggers)
