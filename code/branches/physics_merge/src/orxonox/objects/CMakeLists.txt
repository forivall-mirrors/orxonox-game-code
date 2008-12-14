SET( SRC_FILES
  EventListener.cc
  EventDispatcher.cc
  EventTarget.cc
  Level.cc
  Radar.cc
  RadarListener.cc
  RadarViewable.cc
  Tickable.cc
  Test.cc
  Scene.cc
  Script.cc
)

ADD_SOURCE_DIRECTORY(SRC_FILES collisionshapes)
ADD_SOURCE_DIRECTORY(SRC_FILES controllers)
ADD_SOURCE_DIRECTORY(SRC_FILES gametypes)
ADD_SOURCE_DIRECTORY(SRC_FILES infos)
#ADD_SOURCE_DIRECTORY(SRC_FILES pickup)
ADD_SOURCE_DIRECTORY(SRC_FILES quest)
ADD_SOURCE_DIRECTORY(SRC_FILES weaponSystem)
ADD_SOURCE_DIRECTORY(SRC_FILES worldentities)

ADD_SOURCE_FILES(SRC_FILES)
