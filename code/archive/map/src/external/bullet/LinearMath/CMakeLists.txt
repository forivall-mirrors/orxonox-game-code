ADD_SOURCE_FILES(BULLET_FILES

COMPILATION_BEGIN BulletLinearMathCompilation.cpp
		btConvexHull.cpp
		btQuickprof.cpp
		btGeometryUtil.cpp
		btAlignedAllocator.cpp
COMPILATION_END

		# Headers
		btAlignedObjectArray.h
		btList.h
		btPoolAllocator.h
		btRandom.h
		btVector3.h
		btDefaultMotionState.h
		btMatrix3x3.h
		btQuadWord.h
		btHashMap.h
		btScalar.h
		btAabbUtil2.h
		btConvexHull.h
		btMinMax.h
		btQuaternion.h
		btStackAlloc.h
		btGeometryUtil.h
		btMotionState.h
		btTransform.h
		btAlignedAllocator.h
		btIDebugDraw.h
		btQuickprof.h
		btTransformUtil.h
)
