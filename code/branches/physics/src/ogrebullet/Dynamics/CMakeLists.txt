SET( OGREBULLET_DYNAMICS_SRC_FILES
  OgreBulletDynamicsConstraint.cpp
  OgreBulletDynamicsObjectState.cpp
  OgreBulletDynamicsRigidBody.cpp
  OgreBulletDynamicsWorld.cpp
  
  Constraints/OgreBulletDynamics6DofConstraint.cpp
  Constraints/OgreBulletDynamicsConeTwistConstraint.cpp
  Constraints/OgreBulletDynamicsHingeConstraint.cpp
  Constraints/OgreBulletDynamicsPoint2pointConstraint.cpp
  Constraints/OgreBulletDynamicsRaycastVehicle.cpp
  
  Prefab/OgreBulletDynamicsRagDoll.cpp
)

INCLUDE_DIRECTORIES(.)

ADD_LIBRARY(ogrebullet_dynamics_orxonox STATIC ${OGREBULLET_DYNAMICS_SRC_FILES})
