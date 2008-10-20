SET( OGREBULLET_SRC_FILES
  OgreBulletCollisionsObject.cpp
  OgreBulletCollisionsObjectState.cpp
  OgreBulletCollisionsRay.cpp
  OgreBulletCollisionsShape.cpp
  OgreBulletCollisionsWorld.cpp
  
  Debug/OgreBulletCollisionsDebugContanct.cpp
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
  
  
  # Dynamic files depend on Collision source.
  # They could be handled as two libraries.
  OgreBulletDynamicsConstraint.cpp
  OgreBulletDynamicsObjectState.cpp
  OgreBulletDynamicsRigidBody.cpp
  OgreBulletDynamicsWorld.cpp
  
  Contraints/OgreBulletDynamics6DofContraints.cpp
  Contraints/OgreBulletDynamicsConeTwistContraint.cpp
  Contraints/OgreBulletDynamicsHingeConstraint.cpp
  Contraints/OgreBulletDynamicsPoint2pointContraint.cpp
  Contraints/OgreBulletDynamicsRaycastVehicle.cpp
  
  Prefab/OgreBulletDynamicsRagDoll.cpp
)

INCLUDE_DIRECTORIES(.)

ADD_LIBRARY(ogrebullet_orxonox STATIC ${OGREBULLET_SRC_FILES})
