SET( OGREBULLET_COLLISIONS_SRC_FILES
  OgreBulletCollisionsObject.cpp
  OgreBulletCollisionsObjectState.cpp
  OgreBulletCollisionsRay.cpp
  OgreBulletCollisionsShape.cpp
  OgreBulletCollisionsWorld.cpp
  
  Debug/OgreBulletCollisionsDebugContact.cpp
  Debug/OgreBulletCollisionsDebugDrawer.cpp
  Debug/OgreBulletCollisionsDebugLines.cpp
  Debug/OgreBulletCollisionsDebugShape.cpp
  
  Shapes/OgreBulletCollisionsBoxShape.cpp
  Shapes/OgreBulletCollisionsCapsuleShape.cpp
  Shapes/OgreBulletCollisionsCompoundShape.cpp
  Shapes/OgreBulletCollisionsConeShape.cpp
  Shapes/OgreBulletCollisionsConvexHullShape.cpp
  Shapes/OgreBulletCollisionsCylinderShape.cpp
  Shapes/OgreBulletCollisionsGImpactShape.cpp
  Shapes/OgreBulletCollisionsMinkowskiSumShape.cpp
  Shapes/OgreBulletCollisionsMultiSphereShape.cpp
  Shapes/OgreBulletCollisionsSphereShape.cpp
  Shapes/OgreBulletCollisionsStaticPlaneShape.cpp
  Shapes/OgreBulletCollisionsTriangleShape.cpp
  Shapes/OgreBulletCollisionsTrimeshShape.cpp
  
  Utils/OgreBulletCollisionsMeshToShapeConverter.cpp
)

INCLUDE_DIRECTORIES(.)

ADD_LIBRARY(ogrebullet_collisions_orxonox STATIC ${OGREBULLET_COLLISIONS_SRC_FILES})
