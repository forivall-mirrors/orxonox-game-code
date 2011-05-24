ADD_SOURCE_FILES(BULLET_FILES

BUILD_UNIT BulletLinearMathBuildUnit.cpp
	btAlignedAllocator.cpp
	btConvexHull.cpp
	btConvexHullComputer.cpp
	btGeometryUtil.cpp
	btQuickprof.cpp
	btSerializer.cpp
END_BUILD_UNIT

	# Headers
	btAabbUtil2.h
	btAlignedAllocator.h
	btAlignedObjectArray.h
	btConvexHull.h
	btConvexHullComputer.h
	btDefaultMotionState.h
	btGeometryUtil.h
	btHashMap.h
	btIDebugDraw.h
	btList.h
	btMatrix3x3.h
	btMinMax.h
	btMotionState.h
	btPoolAllocator.h
	btQuadWord.h
	btQuaternion.h
	btQuickprof.h
	btRandom.h
	btScalar.h
	btSerializer.h
	btStackAlloc.h
	btTransform.h
	btTransformUtil.h
	btVector3.h
)
