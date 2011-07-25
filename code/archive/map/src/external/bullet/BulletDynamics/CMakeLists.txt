ADD_SOURCE_FILES(BULLET_FILES

COMPILATION_BEGIN BulletDynamicsCompilation.cpp

	ConstraintSolver/btContactConstraint.cpp
	ConstraintSolver/btConeTwistConstraint.cpp
	ConstraintSolver/btGeneric6DofConstraint.cpp
	ConstraintSolver/btHingeConstraint.cpp
	ConstraintSolver/btPoint2PointConstraint.cpp
	ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
	ConstraintSolver/btSliderConstraint.cpp
	ConstraintSolver/btSolve2LinearConstraint.cpp
	ConstraintSolver/btTypedConstraint.cpp

	Dynamics/Bullet-C-API.cpp
	Dynamics/btDiscreteDynamicsWorld.cpp
	Dynamics/btSimpleDynamicsWorld.cpp
	Dynamics/btRigidBody.cpp

	Vehicle/btRaycastVehicle.cpp
	Vehicle/btWheelInfo.cpp

	Character/btKinematicCharacterController.cpp

COMPILATION_END

	# Headers
	ConstraintSolver/btConstraintSolver.h
	ConstraintSolver/btContactConstraint.h
	ConstraintSolver/btContactSolverInfo.h
	ConstraintSolver/btConeTwistConstraint.h
	ConstraintSolver/btGeneric6DofConstraint.h
	ConstraintSolver/btHingeConstraint.h
	ConstraintSolver/btJacobianEntry.h
	ConstraintSolver/btPoint2PointConstraint.h
	ConstraintSolver/btSequentialImpulseConstraintSolver.h
	ConstraintSolver/btSliderConstraint.h
	ConstraintSolver/btSolve2LinearConstraint.h
	ConstraintSolver/btSolverBody.h
	ConstraintSolver/btSolverConstraint.h
	ConstraintSolver/btTypedConstraint.h

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
