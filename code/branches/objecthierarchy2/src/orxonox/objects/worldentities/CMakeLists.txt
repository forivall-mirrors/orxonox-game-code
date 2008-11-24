SET( SRC_FILES
  WorldEntity.cc
  PositionableEntity.cc
  MovableEntity.cc
  ControllableEntity.cc

  Backlight.cc
  Billboard.cc
  BlinkingBillboard.cc
  FadingBillboard.cc
  Light.cc
  Camera.cc
  CameraPosition.cc
  Model.cc
  ParticleEmitter.cc
  ParticleSpawner.cc
  SpawnPoint.cc
)

ADD_SOURCE_DIRECTORY(SRC_FILES pawns)
ADD_SOURCE_DIRECTORY(SRC_FILES triggers)

ADD_SOURCE_FILES(SRC_FILES)
