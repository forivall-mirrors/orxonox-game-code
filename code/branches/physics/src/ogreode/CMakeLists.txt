SET( OGREODE_SRC_FILES
  OgreOdeBody.cpp
  OgreOdeCollision.cpp
  OgreOdeDebugContact.cpp
  OgreOdeDebugObject.cpp
  OgreOdeEigenSolver.cpp
  OgreOdeEntityInformer.cpp
  OgreOdeGeometry.cpp
  OgreOdeJoint.cpp
  OgreOdeMass.cpp
  OgreOdeSpace.cpp
  OgreOdeStepper.cpp
  OgreOdeTriangleMeshData.cpp
  OgreOdeTriangleMeshDataManager.cpp
  OgreOdeUtility.cpp
  OgreOdeWorld.cpp
)

ADD_LIBRARY(ogreode_orxonox SHARED ${OGREODE_SRC_FILES})
