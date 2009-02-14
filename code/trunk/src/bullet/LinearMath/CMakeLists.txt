SET(LinearMath_SRCS
		btConvexHull.cpp
		btQuickprof.cpp
		btGeometryUtil.cpp
		btAlignedAllocator.cpp
)

ADD_LIBRARY(LinearMath ${LinearMath_SRCS})
