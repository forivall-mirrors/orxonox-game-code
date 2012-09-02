ADD_SOURCE_FILES(BULLET_FILES

COMPILATION_BEGIN BulletDynamicsCompilation.cpp

	Character/btKinematicCharacterController.cpp

	ConstraintSolver/btConeTwistConstraint.cpp
	ConstraintSolver/btContactConstraint.cpp
	ConstraintSolver/btGeneric6DofConstraint.cpp
	ConstraintSolver/btGeneric6DofSpringConstraint.cpp
	ConstraintSolver/btHinge2Constraint.cpp
	ConstraintSolver/btHingeConstraint.cpp
	ConstraintSolver/btPoint2PointConstraint.cpp
	ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
	ConstraintSolver/btSliderConstraint.cpp
	ConstraintSolver/btSolve2LinearConstraint.cpp
	ConstraintSolver/btTypedConstraint.cpp
	ConstraintSolver/btUniversalConstraint.cpp

	Dynamics/btContinuousDynamicsWorld.cpp
	Dynamics/btDiscreteDynamicsWorld.cpp
	Dynamics/btRigidBody.cpp
	Dynamics/btSimpleDynamicsWorld.cpp
	Dynamics/Bullet-C-API.cpp
	Vehicle/btRaycastVehicle.cpp
	Vehicle/btWheelInfo.cpp

COMPILATION_END

	# Headers
	ConstraintSolver/btConeTwistConstraint.h
	ConstraintSolver/btConstraintSolver.h
	ConstraintSolver/btContactConstraint.h
	ConstraintSolver/btContactSolverInfo.h
	ConstraintSolver/btGeneric6DofConstraint.h
	ConstraintSolver/btGeneric6DofSpringConstraint.h
	ConstraintSolver/btHinge2Constraint.h
	ConstraintSolver/btHingeConstraint.h
	ConstraintSolver/btJacobianEntry.h
	ConstraintSolver/btPoint2PointConstraint.h
	ConstraintSolver/btSequentialImpulseConstraintSolver.h
	ConstraintSolver/btSliderConstraint.h
	ConstraintSolver/btSolve2LinearConstraint.h
	ConstraintSolver/btSolverBody.h
	ConstraintSolver/btSolverConstraint.h
	ConstraintSolver/btTypedConstraint.h
	ConstraintSolver/btUniversalConstraint.h
	Dynamics/btActionInterface.h
	Dynamics/btContinuousDynamicsWorld.h
	Dynamics/btDiscreteDynamicsWorld.h
	Dynamics/btDynamicsWorld.h
	Dynamics/btSimpleDynamicsWorld.h
	Dynamics/btRigidBody.h

	Vehicle/btRaycastVehicle.h
	Vehicle/btVehicleRaycaster.h
	Vehicle/btWheelInfo.h

	Character/btCharacterControllerInterface.h
	Character/btKinematicCharacterController.h
)
