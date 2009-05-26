ADD_SOURCE_FILES(ORXONOX_SRC_FILES
  EventListener.cc
  EventDispatcher.cc
  EventTarget.cc
  GlobalShader.cc
  Level.cc
  Radar.cc
  RadarListener.cc
  RadarViewable.cc
  Teamcolourable.cc
  Tickable.cc
  Scene.cc
  Script.cc
)

ADD_SUBDIRECTORY(collisionshapes)
ADD_SUBDIRECTORY(controllers)
ADD_SUBDIRECTORY(gametypes)
ADD_SUBDIRECTORY(infos)
ADD_SUBDIRECTORY(items)
ADD_SUBDIRECTORY(pickup)
ADD_SUBDIRECTORY(quest)
ADD_SUBDIRECTORY(weaponsystem)
ADD_SUBDIRECTORY(worldentities)

